#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

struct Selector {
    std::string tag;
    std::string className;
    std::string content;
    std::vector<Selector> children;
};

std::vector<Selector> parseSelectors(std::istream& in) {
    std::vector<Selector> selectors;
    std::string line;
    int currentIndent = -1;
    std::vector<Selector*> stack;

    while (std::getline(in, line)) {
        int indent = line.find_first_not_of(' ');
        line = line.substr(indent);
        if (indent <= currentIndent) {
            while (indent <= currentIndent && !stack.empty()) {
                stack.pop_back();
                currentIndent -= 2;
            }
        }

        std::istringstream iss(line);
        std::string tag, className, content;
        iss >> tag;
        getline(iss, content);

        size_t classPos = tag.find('.');
        size_t idPos = tag.find('#');
        size_t attrPos = tag.find('#');
        if (classPos != std::string::npos) {
            className = tag.substr(classPos + 1);
            tag = tag.substr(0, classPos);
            if (tag.empty()) {
                tag = "div";
            }
        } else if (idPos != std::string::npos || attrPos != std::string::npos) {
            if (tag.empty() || tag[0] == '#' || tag[0] == '[') {
                tag = "div";
            }
        }

        Selector sel{tag, className, content, {}};
        if (stack.empty()) {
            selectors.push_back(sel);
        } else {
            stack.back()->children.push_back(sel);
        }
        stack.push_back(&selectors.back());
        currentIndent = indent;
    }

    return selectors;
}

void generateHTML(std::ostream& out, const std::vector<Selector>& selectors, int indent = 0) {
    std::string indentStr(indent, ' ');
    for (const auto& sel : selectors) {
        out << indentStr << "<" << sel.tag;
        if (!sel.className.empty()) out << " class=\"" << sel.className << "\"";
        out << ">" << sel.content;
        if (!sel.children.empty()) {
            out << "\n";
            generateHTML(out, sel.children, indent + 2);
            out << indentStr;
        }
        out << "</" << sel.tag << ">\n";
    }
}

void generateSCSS(std::ostream& out, const std::vector<Selector>& selectors, const std::string& parent = "") {
    std::string indentStr = parent.empty() ? "" : " ";
    for (const auto& sel : selectors) {
        out << indentStr << sel.tag;
        if (!sel.className.empty()) out << "." << sel.className;
        out << " {\n";
        generateSCSS(out, sel.children, indentStr + "  ");
        out << indentStr << "}\n";
    }
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <selector-file> <component-name>\n";
        return 1;
    }

    std::ifstream inFile(argv[1]);
    if (!inFile) {
        std::cerr << "Error: Unable to open file " << argv[1] << "\n";
        return 1;
    }

    std::vector<Selector> selectors = parseSelectors(inFile);

    std::string htmlFilename = std::string(argv[2]) + ".html";
    std::ofstream htmlFile(htmlFilename);
    generateHTML(htmlFile, selectors);

    std::string scssFilename = std::string(argv[2]) + ".scss";
    std::ofstream scssFile(scssFilename);
    generateSCSS(scssFile, selectors);

    return 0;
}

