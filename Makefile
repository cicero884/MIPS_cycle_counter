INST := instruction_input
OUT := out.log

OUT_DIR := build
SRC_DIR := src
TEST_DIR := test

FORMAT := clang-format
CC := g++
CFLAG := -O2 -g -Wall -I

SRC := $(wildcard ./$(SRC_DIR)/*.cpp)
SRC2 := $(notdir $(SRC))
OBJ := $(SRC2:%.cpp=%.o)


test: all
	$(OUT_DIR)/reram_simulator < $(TEST_DIR)/$(INST) > $(OUT_DIR)/$(OUT)


all: init $(OBJ)
	$(CC) -o $(OUT_DIR)/reram_simulator $(OUT_DIR)/$(OBJ)

format:
	@echo "format all files"
	@find . -name '*.h' -or -name '*.hpp' -or -name '*.cpp' | xargs clang-format -i -style=file $1


