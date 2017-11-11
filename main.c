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



    //board_print2(position);


    //int test = mini(3, 0);
    //make_move(out_move2[1], 1);
    //board_print2(position);


    //printf("score %d", test);

    fen_to_board("8/8/r2k4/8/8/7R/4K3/8 w - - 1 1");

    board_print(position);
    int test = maxi(5, 1);

    make_move(out_move2[1], 1);
    board_print(position);

   // who_is_cell(position, out_move2[1].current_position);
    printf("%d %d %d %d", test, out_move2[1].MoveType, out_move2[1].current_position, out_move2[1].next_position);
    //uci_listen();

    // что-то не так с расстоновкой фигур, move


//    Сделать что-то с оценочной функцией
//    Возможно внутри нее уже проверять матовые идеи
//    съедание короля, это как-то может улучшить ситуацию
//    переделать? взятие проходной пешки, или сделать баг фикс
//    эффект горизонта

    return 0;
}