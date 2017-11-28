#include "board.h"
#include "move.h"
#include "uci.h"

Board position;

int main() {

    move_init();
    board_init(position);

    // обновить оценку
    // шгахи в тихий поиск
    // qsort
    // хэш таблица
    // многопоточность
    // попробуй как-нибудь передавать позицию по указателю
    // доделать мат и uci

    fen_to_board("r6r/1bppkpbN/pp2p2p/n3PP2/3P4/2PB3P/PP1N2P1/R4RK1 b - -");


    //fen_to_board("1k6/7R/8/7R/8/8/8/K7 w - -");

    board_print2(position);

    uci_listen();

    // не робит альфа бета
    return 0;

}