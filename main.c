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
    //fen_to_board("k7/7R/6R1/8/8/8/K7/8 w - -");


    fen_to_board("8/8/1r2k3/8/8/8/8/K6R w - -");

    board_print2(position);

    uci_listen();

    return 0;

}