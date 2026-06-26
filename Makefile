TARGET = bin/project
SRC = $(wildcard src/*.c)
OBJ = $(patsubst src/%.c, bin/%.o, $(SRC))

$(TARGET): $(OBJ)
	gcc $^ -o $@

bin/%.o: src/%.c
	gcc -g -c $^ -o $@

clean:
	rm -f $(TARGET)
	rm -f bin/%.o
