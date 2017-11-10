//
// Created by valera on 08.11.17.
//
#include <stdio.h>

#include "board.h"
#include "move.h"

extern Board position;
extern MOVE moves[DEPTH][200];

int uci_status;
int current_deep;
MOVE out_move;

int evaluate(Board position, int player) {

    int cell, type, color;
    int black = 0, white = 0;
    // сканируем доску на поисках фигуры
    for (int i = 0; i < 8; i++) {
        for (int j = 68 + i * 16; j < 76 + i * 16; j++) {
            if (position[j] != 0) {

                cell = position[j];
                color = cell & MASK_COLOR;
                type = cell & MASK_TYPE;

                if (color == WHITE) {
                    switch (type) {
                        case FIGURE_TYPE_QUEEN:
                            white += 9;
                            break;

                        case FIGURE_TYPE_ROOK:
                            white += 5;
                            break;

                        case FIGURE_TYPE_BISHOP:
                            white += 3;
                            break;

                        case FIGURE_TYPE_KNIGHT:
                            white += 3;
                            break;

                        case FIGURE_TYPE_PAWN:
                            white += 1;
                            break;
                    }
                }
                else {
                    switch (type) {
                        case FIGURE_TYPE_QUEEN:
                            black += 9;
                            break;

                        case FIGURE_TYPE_ROOK:
                            black += 5;
                            break;

                        case FIGURE_TYPE_BISHOP:
                            black += 3;
                            break;

                        case FIGURE_TYPE_KNIGHT:
                            black += 3;
                            break;

                        case FIGURE_TYPE_PAWN:
                            black += 1;
                            break;
                    }
                }
            }
        }
    }

    if(player)
        return white - black;
    else
        return -1*(white - black);
}

// Негамакс пока что
int negamax(int depth, int alpha, int beta, int current_player) {

    if(!uci_status) {

        if(current_player)
            return 1000;

        return -1000;
    }


    if (depth == 0) { // дошли до листка
        // count_end_pos++;
        return evaluate(position, current_player);
    }

    int score_best;
    int score;

    score_best = -300;

    generate_moves(depth, current_player); // поулчаем все ходы в moves[depth]

    // checkmate
    if (moves[depth][0].MoveType == -1) {

        if (king_is_checked(WHITE)) {

            return 200;
        }
        else if (king_is_checked(BLACK)) {

            return -200;
        }
        else {

            return 0;
        }
    }

    // проходимся по новым ходам
    for (int i = 0; moves[depth][i].MoveType != -1; i++) {

        make_move(moves[depth][i], depth); // тут меняется состояние доски

        score = -negamax(depth - 1, -beta, -alpha, !current_player);

        rollback_move(moves[depth][i], depth); // возвращаем прежнюю доску

        if (score > score_best) {
            score_best = score;              // обновляем  лучшую оценку
            if (depth == current_deep) {
                out_move = moves[depth][i];
            }

        }

        moves[depth][i].MoveType = -1;
    }

    return score_best;
}