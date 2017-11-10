//
// Created by valera on 08.11.17.
//

#ifndef CHESS_EVALUATE_H
#define CHESS_EVALUATE_H

// пока мегамакс
int negamax(int depth, int alpha, int beta, int current_player);

// анализ листьев
int evaluate(Board position, int player);

#endif //CHESS_EVALUATE_H
