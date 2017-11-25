//
// Created by valera on 08.11.17.
//

#ifndef CHESS_EVALUATE_H
#define CHESS_EVALUATE_H

#define CHECKMATE 50000

// анализ листьев
int evaluate(int player);

int quiesce(int alpha, int beta, int current_player, int depth);

#endif //CHESS_EVALUATE_H
