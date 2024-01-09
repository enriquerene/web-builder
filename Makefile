CXX=g++
CXXFLAGS=-std=c++11 -Wall -Wextra
LDFLAGS=
TEST_CXXFLAGS=$(CXXFLAGS) -I/usr/include/catch2

APP_NAME=web-builder
TEST_NAME=web-builder-tests

APP_SRC=web-builder.cpp
TEST_SRC=web-builder-tests.cpp

all: build tests

app: build_app

build: build_app build_tests

build_app: $(APP_SRC)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $(APP_NAME) $(APP_SRC)

build_tests: $(TEST_SRC)
	$(CXX) $(TEST_CXXFLAGS) $(LDFLAGS) -o $(TEST_NAME) $(TEST_SRC)

tests: build_tests
	./$(TEST_NAME)

clean:
	rm -f $(APP_NAME) $(TEST_NAME)

.PHONY: all build build_app build_tests tests clean
