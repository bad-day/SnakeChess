#ifndef CHESS_EVALUATE_H
#define CHESS_EVALUATE_H

// analysis terminated nodes
int evaluate(int player);

// quiescence Search
int quiesce(int alpha, int beta, int current_player, int depth);

// bonus points
int additional_points(int type, int color, int coord);

// bonus for king
int king_additional_points(int coord, int endspiel);
#endif //CHESS_EVALUATE_H
