CC = gcc
CFLAGS = -c -O3 -Wall -g

EXE = main
SRC_DIR = src
OBJ_DIR = obj
INC_DIR = include

INCFLAG = -I include/

SRC = $(wildcard $(SRC_DIR)/*.c)
OBJ = $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

all: $(EXE)

$(EXE): $(OBJ)
	$(CC) $^ -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) $(INCFLAG) -c $< -o $@

# test: $(EXE) # valgrind --leak-check=yes --track-origins=yes ./project2 $(args)

.PHONY: clean
clean:
	$(RM) $(OBJ)

.PHONY: image
image:
	docker build . -t zerver-test

.PHONY: run
run: image
	docker run -it -p 5051:5051 zerver-test:latest
