//
// Created by valera on 30.10.17.
//
#define DEPTH 10

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

#define CASTLING_O_O 32
#define CASTLING_O_O_O 64

#define BORDER 128
#define MASK_BORDER 127

#define CELL_EMPTY 0


typedef int Board[256];

//void board_set(Board *board, int sq, int piece);
void board_init(Board board);
void test_board(Board board);
void board_print(Board board);

extern Board position;
#endif //CHESS_BOARD_H