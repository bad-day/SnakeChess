//
// Created by valera on 08.11.17.
//

#ifndef CHESS_EVALUATE_H
#define CHESS_EVALUATE_H

#define CHECKMATE 500
// пока мегамакс
int negamax(int depth, int current_player);

// анализ листьев
int evaluate(int player);

int AlphaBetaPruning (int alpha, int beta, int depth, int current_player);

int mini( int depth, int current_player);

int maxi( int depth, int current_player);

int my_score(int depth, int current_player);

#endif //CHESS_EVALUATE_H
