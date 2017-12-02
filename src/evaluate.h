#ifndef CHESS_EVALUATE_H
#define CHESS_EVALUATE_H

#define CHECKMATE 50000

// analysis terminated nodes
int evaluate(int player);

// quiescence Search
int quiesce(int alpha, int beta, int current_player, int depth);

#endif //CHESS_EVALUATE_H
