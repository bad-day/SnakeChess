#ifndef CHESS_ALGORITHMS_H
#define CHESS_ALGORITHMS_H

// minimax algorithm
int minimax(int depth, int current_player);

// alpha-beta pruning algorithm
int alpha_beta(int alpha, int beta, int depth, int level, int current_player, int null_move);

#endif //CHESS_ALGORITHMS_H
