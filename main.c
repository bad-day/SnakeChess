#include <stdio.h>

#include "board.h"
#include "move.h"
#include "uci.h"
#include "evaluate.h"

Board position;

extern MOVE out_move2[2];
int main() {

    move_init();
    board_init(position);
    //test_board(position);
    // position fen 5rr1/2p1q1bk/3p4/pPpPp1R1/P3P3/4B2P/1P3P2/1K4R1 w - - 0 31
    fen_to_board("5rrk/2p1q1bp/3p4/pPpPp1RQ/P3P3/4B2P/1P3P2/1K4R1 w - - 5 30");

    board_print2(position);


//    make_move(out_move2[0], 0);
//    board_print2(position);

   // printf("%d %d %d %d", test, out_move2[0].MoveType, out_move2[0].current_position, out_move2[0].next_position);
    uci_listen();




    //int test = mini(3, 0);
    //make_move(out_move2[1], 1);
    //board_print2(position);


    //printf("score %d", test);

    //fen_to_board("8/k7/8/6RR/7p/8/8/6K1 w - -");
    //fen_to_board("8/8/8/r2k4/8/7R/4K3/8 w - -");
    //fen_to_board("8/8/8/8/8/3pk3/8/2K5 w - -");

//    fen_to_board("1k6/7R/8/6R1/8/8/3K4/8 w - -");
//
//    board_print2(position);
//    int test = my_score(2, 1);
//
//
//
//   // who_is_cell(position, out_move2[1].current_position);
//    printf("%d %d %d %d", test, out_move2[1].MoveType, out_move2[1].current_position, out_move2[1].next_position);
//
//    make_move(out_move2[1], 0);
//    board_print2(position);

    // что-то не так с расстоновкой фигур, move


//    Сделать что-то с оценочной функцией
//    Возможно внутри нее уже проверять матовые идеи
//    съедание короля, это как-то может улучшить ситуацию
//    переделать? взятие проходной пешки, или сделать баг фикс
//    эффект горизонта

    // не допускать съедание короля
    // подумать над патом и матом

    // проверить

    // Если нету короля, то это конечная точка
    // Если король не может ходить, то тоже конечная
    return 0;
}