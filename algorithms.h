//
// Created by valera on 14.11.17.
//

#ifndef CHESS_ALGORITHMS_H
#define CHESS_ALGORITHMS_H

int minimax(int depth, int current_player);

int alpha_beta(int alpha, int beta, int depth, int current_player);

int alphaBeta(int alpha, int beta, int depth, int current_player);

int my_alpha(int alpha, int beta, int depth , int current_player);

int score_alphabeta(int depth, int alpha, int beta, int current_player);
#endif //CHESS_ALGORITHMS_H
