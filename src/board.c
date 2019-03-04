#include <stdio.h>
#include "board.h"

// board init
void board_init(Board board) {

    for (int i = 0; i < 256; i++) { // set border flag
        board[i] = BORDER;
    }

    // black figures
    board[68] = FIGURE_TYPE_ROOK | BLACK;
    board[69] = FIGURE_TYPE_KNIGHT | BLACK;
    board[70] = FIGURE_TYPE_BISHOP | BLACK;
    board[71] = FIGURE_TYPE_QUEEN | BLACK;
    board[72] = FIGURE_TYPE_KING | BLACK;
    board[73] = FIGURE_TYPE_BISHOP | BLACK;
    board[74] = FIGURE_TYPE_KNIGHT | BLACK;
    board[75] = FIGURE_TYPE_ROOK | BLACK;

    for (int i = 84; i < 92; i++) {
        board[i] = FIGURE_TYPE_PAWN | BLACK;
    }

    // white figures
    board[180] = FIGURE_TYPE_ROOK | WHITE;
    board[181] = FIGURE_TYPE_KNIGHT | WHITE;
    board[182] = FIGURE_TYPE_BISHOP | WHITE;
    board[183] = FIGURE_TYPE_QUEEN | WHITE;
    board[184] = FIGURE_TYPE_KING | WHITE;
    board[185] = FIGURE_TYPE_BISHOP | WHITE;
    board[186] = FIGURE_TYPE_KNIGHT | WHITE;
    board[187] = FIGURE_TYPE_ROOK | WHITE;

    for (int i = 164; i < 172; i++) {
        board[i] = FIGURE_TYPE_PAWN | WHITE;
    }

    // empty cell
    for (int i = 0; i < 4; i++) {
        for (int j = 100 + i * 16; j < 108 + i * 16; j++) {
            board[j] = 0;
        }
    }
}

// board print 16x16
void board_print(Board board) {

    for (int i = 0; i < 256; i++) {
        if (i % 16 == 0) {
            printf("\n");
        }

        printf("%d\t", board[i]);
    }
    printf("\n");
    printf("\n");
}

// user-friendly board print
void board_print2(Board board) {

    int cell, type, color;
    for (int i = 64, k = 1; i < 196; i++, k++) {
        if (i % 16 == 0) {
            printf("\n");
        }

        if (i % 8 == 0 && i % 16 == 0) {
            int number = i / 16 - 3;
            if (number != 9) {
                printf("\x1b[33m%d\x1b[0m ", 9 - number);
            }
            k++;
        }

        cell = board[i];
        type = cell & MASK_TYPE;
        color = cell & MASK_COLOR;

        if (cell != MASK_BORDER) {
            if (color == WHITE) {
                if (k % 2 == 0) {
                    printf("\x1b[1;31;47m");
                } else {
                    printf("\x1b[1;31;40m");
                }
            } else if (color == BLACK) {
                if (k % 2 == 0) {
                    printf("\x1b[1;34;47m");
                } else {
                    printf("\x1b[1;34;40m");
                }
            }

            switch (type) {
                case FIGURE_TYPE_KING:
                    printf("K");
                    break;

                case FIGURE_TYPE_QUEEN:
                    printf("Q");
                    break;

                case FIGURE_TYPE_ROOK:
                    printf("R");
                    break;

                case FIGURE_TYPE_KNIGHT:
                    printf("N");
                    break;

                case FIGURE_TYPE_BISHOP:
                    printf("B");
                    break;

                case FIGURE_TYPE_PAWN:
                    printf("P");
                    break;

                default:
                    if (k % 2 == 0) {
                        printf("\x1b[30;47m ");
                    } else {
                        printf("\x1b[30;40m ");
                    }
                    break;
            }

            printf("\x1b[0m ");
        }
    }

    printf("\x1b[33m  ");
    for (int j = 0; j < 8; j++) {
        printf("%c ", 'a' + j);
    }

    printf("\x1b[0m\n");
    fflush(stdout);
}

// what a piece on the board
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
    } else {
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