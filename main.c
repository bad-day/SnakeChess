#include "board.h"
#include "move.h"
#include "uci.h"
#include "hash.h"

Board position;
extern unsigned long current_hash;
int main() {

    move_init();
    board_init(position);
    hash_init();


    // перепроверь проходные
    // обновить оценку
    // шгахи в тихий поиск
    // qsort
    // хэш таблица
    // многопоточность
    // попробуй как-нибудь передавать позицию по указателю
    // доделать  uci

    //fen_to_board("r6r/1bppkpbN/pp2p2p/n3PP2/3P4/2PB3P/PP1N2P1/R4RK1 b - -");
    //fen_to_board("k7/5R2/6P1/8/8/8/8/K7 w - -");


    fen_to_board("k5r1/1b4b1/4R3/3P4/1p2N1P1/1p3P2/1P5P/6K1 w - - 0 1");

    board_print2(position);

    uci_listen();

    return 0;

}