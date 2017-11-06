//
// Created by valera on 02.11.17.
//

#include <stdio.h>
#include "board.h"

extern int FigureWhiteCoord256[16];
extern int FigureBlackCoord256[16];
extern int *KingWhitePointer;
extern int *KingBlackPointer;

void board_init(Board board) {

    for(int i = 0; i < 256; i++) // set border flag
        board[i] = BORDER;

    // black figures
    board[69] = FIGURE_TYPE_ROOK|BLACK;
    board[70] = FIGURE_TYPE_KNIGHT|BLACK;
    board[71] = FIGURE_TYPE_BISHOP|BLACK;
    board[72] = FIGURE_TYPE_QUEEN|BLACK;
    board[73] = FIGURE_TYPE_KING|BLACK;
    board[74] = FIGURE_TYPE_BISHOP|BLACK;
    board[75] = FIGURE_TYPE_KNIGHT|BLACK;
    board[76] = FIGURE_TYPE_ROOK|BLACK;

    for(int i = 85; i < 93; i++)
        board[i] = FIGURE_TYPE_PAWN|BLACK;

    // white figures
    board[181] = FIGURE_TYPE_ROOK|WHITE;
    board[182] = FIGURE_TYPE_KNIGHT|WHITE;
    board[183] = FIGURE_TYPE_BISHOP|WHITE;
    board[184] = FIGURE_TYPE_QUEEN|WHITE;
    board[185] = FIGURE_TYPE_KING|WHITE;
    board[186] = FIGURE_TYPE_BISHOP|WHITE;
    board[187] = FIGURE_TYPE_KNIGHT|WHITE;
    board[188] = FIGURE_TYPE_ROOK|WHITE;

    for(int i = 165; i < 173; i++)
        board[i] = FIGURE_TYPE_PAWN|WHITE;

    // empty cell
    for(int i = 0; i < 4; i++) {
        for(int j = 101 + i*16; j < 109 + i*16; j++)
            board[j] = 0;
    }

}

void test_board(Board board) {
    for(int i = 0; i < 256; i++) // set border flag
        board[i] = BORDER;

    // empty cell
    for(int i = 0; i < 8; i++) {
        for(int j = 68 + i*16; j < 76 + i*16; j++)
            board[j] = 0;
    }

    board[72] = FIGURE_TYPE_KING|WHITE;

    board[104] = FIGURE_TYPE_KING|BLACK;
    board[107] = FIGURE_TYPE_QUEEN|BLACK;


    for(int i = 0; i < 16; i++) {
        FigureWhiteCoord256[i] = 0;
        FigureBlackCoord256[i] = 0;
    }

    FigureWhiteCoord256[0] = 72;
    FigureBlackCoord256[0] = 104;
    FigureBlackCoord256[1] = 107;
}

void board_print(Board board) {
    for(int i = 0; i < 256; i++) {

        if(i % 16 == 0)
            printf("\n");

        printf("%d\t", board[i]);
    }
    printf("\n");
    printf("\n");
}