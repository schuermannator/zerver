CC = gcc
CFLAGS = -c -O3 -Wall -g

EXE = main
SRC_DIR = src
OBJ_DIR = obj

SRC = $(wildcard $(SRC_DIR)/*.c)
OBJ = $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

all: $(EXE)

$(EXE): $(OBJ)
	$(CC) $^ -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# test: $(EXE) # valgrind --leak-check=yes --track-origins=yes ./project2 $(args)

.PHONY: clean
clean:
	$(RM) $(OBJ)
