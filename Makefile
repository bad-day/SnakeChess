all:
	gcc -o chess main.c board.c move.c evaluate.c uci.c -pthread
	./chess
clean:
	- rm -f *.o chess