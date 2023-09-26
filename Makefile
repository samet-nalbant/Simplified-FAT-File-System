CC = g++
CFLAGS = -std=c++11 -g
INCLUDE_DIR = include
SRC_DIR = src

DEPS = $(wildcard $(INCLUDE_DIR)/*.h)
SRC = $(wildcard $(SRC_DIR)/*.cpp)
OBJ = $(patsubst $(SRC_DIR)/%.cpp, %.o, $(SRC))

.PHONY: all clean

all: main

main: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

%.o: $(SRC_DIR)/%.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

clean:
	rm -f *.o main
