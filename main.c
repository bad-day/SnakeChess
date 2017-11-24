#include "board.h"
#include "move.h"
#include "uci.h"

Board position;

int main() {

    move_init();
    board_init(position);

    //position fen rn1qkbnr/ppp2ppp/4p3/3pNb2/8/N7/PPPPPPPP/R1BKQBR1 w KQkq -
    // r1bqkb1r/2pp1ppp/p1n2n2/1p2p3/4P3/1B3N2/PPPP1PPP/RNBQ1RK1 b kq - 1 6

    // r1bqkbnr/1pp2ppp/p1np4/8/2B5/2N5/PPPP2PP/R1BQK1NR b KQkq - 0 1  странный 4 глубина, не ест фигуру
    //fen_to_board("r1bqkbnr/1pp2ppp/p1np4/8/2B5/2N5/PPPP2PP/R1BQK1NR b KQkq - 0 1");
    //fen_to_board("r6r/1bppkpbN/pp2p2p/n3PP2/3P4/2PB3P/PP1N2P1/R4RK1 b - - 0 21");

    //fen_to_board("r6r/1bppkpbN/pp2p2p/n3PP2/3P4/2PB3P/PP1N2P1/R4RK1 b - -");

    fen_to_board("r6r/1bppkpbN/pp2p2p/n3PP2/3P4/2PB3P/PP1N2P1/R4RK1 b - - 0 21");


    //fen_to_board("8/5b2/3k1b2/8/1p2PP2/P7/8/1K2R3 b - -");
    // 8/7p/3k1b1p/8/4PP2/8/8/1K2R3 b - -

    uci_listen();

    // не робит альфа бета
    return 0;

    // убрать везде depth
    // добавить тихий поиск
    // добавить сортировку
    // добавить многопоточность

    // эвристики, отсечения
    // поменьше байт копировать или пределать rollback
}