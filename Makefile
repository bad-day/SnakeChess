all:
	gcc -o chess main.c board.c move.c
	./chess
clean:
	- rm -f *.o chess