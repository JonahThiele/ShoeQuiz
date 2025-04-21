#we should really consider changing this over to a cmake file

CXX = g++
#needs c++20 because I don't know the value for a parameter list and needed to pass an auto type
CXXFLAGS = -std=c++20 -Wall -g

TARGET = ShoeSeeker

SRC_DIR = ./src
BUILD_DIR = ./build

SRC = $(wildcard $(SRC_DIR)/*.cpp)

OBJ = $(SRC:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(OBJ) -o $(BUILD_DIR)/$(TARGET)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm $(BUILD_DIR)$(TARGET)

.PHONY: all clean

