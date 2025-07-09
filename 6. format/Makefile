CXX := g++
CXXFLAGS := -std=c++20 -Wall -Wextra -Werror -pedantic
TARGET := test_runner
OBJS := test.o

.PHONY: all clean test

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

test.o: test.cpp format.h
	$(CXX) $(CXXFLAGS) -c $<

test: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET) $(OBJS)