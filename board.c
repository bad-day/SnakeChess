//
// Created by valera on 02.11.17.
//

#include <stdio.h>
#include "board.h"

void board_init(Board board) {

    for (int i = 0; i < 256; i++) // set border flag
        board[i] = BORDER;

    // black figures
    board[68] = FIGURE_TYPE_ROOK | BLACK;
    board[69] = FIGURE_TYPE_KNIGHT | BLACK;
    board[70] = FIGURE_TYPE_BISHOP | BLACK;
    board[71] = FIGURE_TYPE_QUEEN | BLACK;
    board[72] = FIGURE_TYPE_KING | BLACK;
    board[73] = FIGURE_TYPE_BISHOP | BLACK;
    board[74] = FIGURE_TYPE_KNIGHT | BLACK;
    board[75] = FIGURE_TYPE_ROOK | BLACK;

    for (int i = 84; i < 92; i++)
        board[i] = FIGURE_TYPE_PAWN | BLACK;

    // white figures
    board[180] = FIGURE_TYPE_ROOK | WHITE;
    board[181] = FIGURE_TYPE_KNIGHT | WHITE;
    board[182] = FIGURE_TYPE_BISHOP | WHITE;
    board[183] = FIGURE_TYPE_QUEEN | WHITE;
    board[184] = FIGURE_TYPE_KING | WHITE;
    board[185] = FIGURE_TYPE_BISHOP | WHITE;
    board[186] = FIGURE_TYPE_KNIGHT | WHITE;
    board[187] = FIGURE_TYPE_ROOK | WHITE;

    for (int i = 164; i < 172; i++)
        board[i] = FIGURE_TYPE_PAWN | WHITE;

    // empty cell
    for (int i = 0; i < 4; i++) {
        for (int j = 100 + i * 16; j < 108 + i * 16; j++)
            board[j] = 0;
    }

}

void test_board(Board board) {
    for (int i = 0; i < 256; i++) // set border flag
        board[i] = BORDER;

    // empty cell
    for (int i = 0; i < 8; i++) {
        for (int j = 68 + i * 16; j < 76 + i * 16; j++)
            board[j] = 0;
    }


    // board[168] = FIGURE_TYPE_PAWN | BLACK | IS_MOVE;

    board[103] = FIGURE_TYPE_KING | BLACK;
    board[100] = FIGURE_TYPE_ROOK | BLACK;

    board[170] = FIGURE_TYPE_KING | WHITE;
    board[155] = FIGURE_TYPE_ROOK | WHITE;
    //board[107] = FIGURE_TYPE_ROOK | WHITE;
    //board[90] = FIGURE_TYPE_PAWN | WHITE | IS_MOVE;

}

void test_board2(Board board) {
    for (int i = 0; i < 256; i++) // set border flag
        board[i] = BORDER;

    // empty cell
    for (int i = 0; i < 8; i++) {
        for (int j = 68 + i * 16; j < 76 + i * 16; j++)
            board[j] = 0;
    }

    board[72] = FIGURE_TYPE_KING | BLACK;
    board[71] = FIGURE_TYPE_PAWN | BLACK;
    board[73] = FIGURE_TYPE_BISHOP | BLACK;

    board[87] = FIGURE_TYPE_PAWN | BLACK;
    board[88] = FIGURE_TYPE_PAWN | BLACK;
    //board[91] = FIGURE_TYPE_PAWN|BLACK;

    board[120] = FIGURE_TYPE_KING | WHITE;
    board[121] = FIGURE_TYPE_QUEEN | WHITE;

}

void board_print(Board board) {
    for (int i = 0; i < 256; i++) {

        if (i % 16 == 0)
            printf("\n");

        printf("%d\t", board[i]);
    }
    printf("\n");
    printf("\n");
}

void board_print2(Board board) {
    int cell, type, color;
    for (int i = 64; i < 196; i++) {

        if (i % 16 == 0)
            printf("\n");

        cell = board[i];
        type = cell & MASK_TYPE;
        color = cell & MASK_COLOR;

        if(color == WHITE) {
            if (cell != CELL_EMPTY) {

                switch (type) {

                    case FIGURE_TYPE_KING:
                        printf("K ");
                        break;

                    case FIGURE_TYPE_QUEEN:
                        printf("Q ");
                        break;

                    case FIGURE_TYPE_ROOK:
                        printf("R ");
                        break;

                    case FIGURE_TYPE_KNIGHT:
                        printf("N ");
                        break;

                    case FIGURE_TYPE_BISHOP:
                        printf("B ");
                        break;

                    case FIGURE_TYPE_PAWN:
                        printf("P ");
                        break;
                }
            }

            else {
                printf("%d ", board[i]);
            }
        }
        else {
            if (cell != CELL_EMPTY) {

                switch (type) {

                    case FIGURE_TYPE_KING:
                        printf("k ");
                        break;

                    case FIGURE_TYPE_QUEEN:
                        printf("q ");
                        break;

                    case FIGURE_TYPE_ROOK:
                        printf("r ");
                        break;

                    case FIGURE_TYPE_KNIGHT:
                        printf("n ");
                        break;

                    case FIGURE_TYPE_BISHOP:
                        printf("b ");
                        break;

                    case FIGURE_TYPE_PAWN:
                        printf("p ");
                        break;
                }
            }

            else {
                printf("%d ", board[i]);
            }
        }


    }
    printf("\n");
}

void who_is_cell(Board board, int coord) {

    int cell = board[coord];

    if (cell == CELL_EMPTY) {
        printf("Cell is empty");
        return;
    }

    int is_border = cell & MASK_BORDER;
    if (is_border == BORDER) {
        printf("Cell is border");
    }

    int color = cell & MASK_COLOR;
    if (color == WHITE) {
        printf("White ");
    }
    else {
        printf("Black ");
    }

    int type = cell & MASK_TYPE;

    switch (type) {

        case FIGURE_TYPE_KING:
            printf("king");
            break;

        case FIGURE_TYPE_QUEEN:
            printf("queen");
            break;

        case FIGURE_TYPE_ROOK:
            printf("rook");
            break;

        case FIGURE_TYPE_KNIGHT:
            printf("knight");
            break;

        case FIGURE_TYPE_BISHOP:
            printf("bishop");
            break;

        case FIGURE_TYPE_PAWN:
            printf("pawn");
            break;
    }


}