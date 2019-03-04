#ifndef CHESS_ALGORITHMS_H
#define CHESS_ALGORITHMS_H

#include "move.h"

// minimax algorithm
int minimax(int depth, int current_player);

// alpha-beta pruning algorithm
int alpha_beta(int alpha, int beta, int depth, int level, int current_player, MOVE_TYPE last_move_type, int null_move,
               int extended);

// thread to infinity analyze
void *search();

// print three of best moves
void print_best_moves(int depth, char *best);

#endif //CHESS_ALGORITHMS_H
