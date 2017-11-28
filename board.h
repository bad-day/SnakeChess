//
// Created by valera on 30.10.17.
//

#ifndef CHESS_BOARD_H
#define CHESS_BOARD_H

// type of figures
#define FIGURE_TYPE_KING   1
#define FIGURE_TYPE_QUEEN  2
#define FIGURE_TYPE_ROOK   3
#define FIGURE_TYPE_BISHOP 4
#define FIGURE_TYPE_KNIGHT 5
#define FIGURE_TYPE_PAWN   6

#define MASK_TYPE 7

#define WHITE 8
#define BLACK 16
#define MASK_COLOR (WHITE|BLACK)

#define BORDER 32
#define MASK_BORDER 32

#define IS_MOVE 64
#define MASK_MOVE 64

#define IS_PASSED_PAWN_UCI 128
#define MASK_PASSED_PAWN_UCI 128

#define CELL_EMPTY 0

typedef int Board[256];

void board_init(Board board);

void board_print(Board board);

void board_print2(Board board);

void who_is_cell(Board board, int coord);

#endif //CHESS_BOARD_H