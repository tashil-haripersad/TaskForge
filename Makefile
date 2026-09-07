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

clean:
	rm -f $(OBJ) $(TARGET)