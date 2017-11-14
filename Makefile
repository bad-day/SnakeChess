CXX			= gcc
FLAGS		= -pthread

SOURCES		= main.c board.c move.c evaluate.c uci.c algorithms.c
OBJECTS		= $(SOURCES:.c=.o)

chess: $(OBJECTS)
	$(CC) $(FLAGS) $(OBJECTS) -o $@
	./chess

clean:
	-rm -f $(OBJECTS) chess