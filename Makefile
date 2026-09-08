CXX := g++
CXXFLAGS := -std=c++11 -Wall -Wextra -g
SRC := $(wildcard *.cpp)
OBJ := $(SRC:.cpp=.o)
TARGET := taskforge

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJ)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

valgrind: $(TARGET)
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./$(TARGET)

clean:
	rm -f $(OBJ) $(TARGET)