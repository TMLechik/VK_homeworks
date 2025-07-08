CXX := g++
CXXFLAGS := -std=c++20 -Wall -Wextra -Werror -pedantic
TARGET := test_runner
OBJS := serializer.o test.o

.PHONY: all clean test

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

serializer.o: serializer.cpp serializer.h
	$(CXX) $(CXXFLAGS) -c $<

test.o: test.cpp serializer.h
	$(CXX) $(CXXFLAGS) -c $<

test: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET) $(OBJS)