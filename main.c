#include "board.h"
#include "move.h"
#include "uci.h"

Board position;

int main() {

    move_init();
    board_init(position);

    //position fen rn1qkbnr/ppp2ppp/4p3/3pNb2/8/N7/PPPPPPPP/R1BKQBR1 w KQkq -
    //fen_to_board("1rbqkb1r/pppppppp/7n/8/2BnP3/3P4/PPP2PPP/RNBQK1NR b KQkq - 0 1");

    uci_listen();

    return 0;
}