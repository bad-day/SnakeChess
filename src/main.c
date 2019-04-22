#include "board.h"
#include "move.h"
#include "uci.h"
#include "hash.h"

Board position;

int main() {

    move_init(); // initialize move
    board_init(position); // load default position
    hash_init(); // initialize zobrist hash

    uci_listen(); // listen command

    return 0;

    // TODO Test, benchmark, front? extern to headers?, move array to list ?, algo to search, license?
}