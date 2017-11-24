//
// Created by valera on 08.11.17.
//

#ifndef CHESS_EVALUATE_H
#define CHESS_EVALUATE_H

#define CHECKMATE 50000

// анализ листьев
int evaluate(int player);

int minimax(int depth, int current_player);

int alpha_beta(int alpha, int beta, int depth, int current_player);

#endif //CHESS_EVALUATE_H
