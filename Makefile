CXX			= gcc
FLAGS		= -pthread

SOURCES		= src/main.c src/board.c src/move.c src/evaluate.c src/uci.c src/algorithms.c src/hash.c
OBJECTS		= $(SOURCES:.c=.o)

SnakeChess: $(OBJECTS)
	$(CC) $(FLAGS) $(OBJECTS) -o $@

clean:
	-rm -f $(OBJECTS) SnakeChess