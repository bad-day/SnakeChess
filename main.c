#include "board.h"
#include "move.h"
#include "uci.h"
#include "hash.h"

Board position;
int main() {

    move_init(); // initialize move
    board_init(position); // load default position
    hash_init(); // initialize zobrist hash

    // отсечения в кэше
    // update eval
    // checks to qul search
    // qsort
    // multi threading
    // update uci

    fen_to_board("r6r/1bppkpbN/pp2p2p/n3PP2/3P4/2PB3P/PP1N2P1/R4RK1 b - -");
    //fen_to_board("rn1q1k1r/p1p1b1p1/bp2pnQ1/3p4/3P1B2/2P2N2/PP4PP/RN3RK1 w - - 1 16");

    //fen_to_board("k5r1/1b4b1/4R3/3P4/1p2N1P1/1p3P2/1P5P/6K1 w - - 0 1");

    board_print2(position);

    uci_listen(); // listen command

    return 0;

}