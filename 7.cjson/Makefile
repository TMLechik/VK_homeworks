CXX := g++
CXXFLAGS := -std=c++20 -O3 -Wall -Wextra -Werror -pedantic -fPIC -shared
CXXFLAGS += -Wno-unused-parameter -Wno-missing-field-initializers
INCLUDES := -I/usr/include/python3.12

MODULE := cjson.cpython-312-x86_64-linux-gnu.so
SOURCE := cjson.cpp
TEST := test_cjson.py

.PHONY: all build install test clean

all: build install

build: $(MODULE)

$(MODULE): $(SOURCE)
	$(CXX) $(CXXFLAGS) $(INCLUDES) $< -o $@
	@echo "Module built: $@"

install: $(MODULE)
	@echo "Installing module..."
	python3 setup.py install --user
	@echo "Installation complete"

test: install
	@echo "Running tests..."
	python3 $(TEST)
	@echo "Tests completed"

clean:
	rm -f $(MODULE)
	rm -rf build
	rm -rf lib.linux-*
	rm -rf temp.linux-*
	rm -f *.pyc
	@echo "Clean complete"
