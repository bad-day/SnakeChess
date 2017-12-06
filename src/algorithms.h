#ifndef CHESS_ALGORITHMS_H
#define CHESS_ALGORITHMS_H

#include "move.h"
// minimax algorithm
int minimax(int depth, int current_player);

// alpha-beta pruning algorithm
int alpha_beta(int alpha, int beta, int depth, int level, int current_player, MOVE_TYPE last_move_type, int null_move, int extended);

#endif //CHESS_ALGORITHMS_H
