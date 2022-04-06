CXX := gcc
CXX_FLAGS := -g -Wextra -Wall

SRC := ./src
INCLUDE := ./include
LIB := ./lib

LIBRARIES := -lglfw3dll
EXECUTABLE := main

$(EXECUTABLE):$(SRC)/*
	$(CXX) $(CXX_FLAGS) -I$(INCLUDE) -L$(LIB) $^ -o "output\$@.exe" $(LIBRARIES)