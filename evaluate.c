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
int ply;
MOVE out_move;
MOVE out_move2[2];

int mini( int depth, int current_player);

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

    return white - black;

    if (player)
        return white - black;
    else
        return -1 * (white - black);
}

// Негамакс пока что
int negamax(int depth, int current_player) {

    if (!uci_status) {

        if (current_player) {
            return 1000;
        }
        return -1000;
    }


    if (depth == 0) { // дошли до листка
        // count_end_pos++;

        return evaluate(position, current_player);

    }

    int score_best = -300;
    int score;


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
        ++ply;

        score = -negamax(depth - 1, !current_player);

        rollback_move(moves[depth][i], depth); // возвращаем прежнюю доску

        --ply;

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

int AlphaBetaPruning(int alpha, int beta, int depth, int current_player) {

    int value;
    generate_moves(depth, current_player);

    for (int i = 0; moves[depth][i].MoveType != -1; i++) {

        make_move(moves[depth][i], depth); // тут меняется состояние доски


        if (depth > 1)

            value = -AlphaBetaPruning(-beta, -alpha, depth - 1, current_player);
        else
            value = evaluate(position, current_player);


        rollback_move(moves[depth][i], depth); // возвращаем прежнюю доску

        if (value > alpha) {
            out_move = moves[depth][i];
            // This move is so good that caused a cutoff of rest tree
            if (value >= beta)
                return beta;
            alpha = value;
        }

        moves[depth][i].MoveType = -1;
    }

    if (moves[depth][0].MoveType == -1) {

        if (king_is_checked(WHITE)) {

            return (-200);
        }
        else if (king_is_checked(BLACK)) {

            return (200);
        }
        else {

            return 0;
        }
    }

    return alpha;
}

int test(int depth, int current_player) {

    if (!uci_status) {

        if (current_player) {
            return 1000;
        }
        return -1000;
    }


}

int maxi( int depth, int current_player) {

    int score;
    if ( depth == 0 ) return evaluate(position, current_player);
    int max = -300;

    moves[depth][0].MoveType = -1;
    generate_moves(depth, current_player);


    int count;
    for (count = 0; moves[depth][count].MoveType != -1; count++);
    //printf("count maxi %d\n", count);

//    if (moves[depth][0].MoveType == -1) {
//
//        if (king_is_checked(WHITE)) {
//
//            return -200;
//        }
//        else if (king_is_checked(BLACK)) {
//
//            return 200;
//        }
//        else {
//
//            return 0;
//        }
//    }

    for (int i = 0; moves[depth][i].MoveType != -1; i++) {

        make_move(moves[depth][i], depth);
        score = mini( depth - 1, !current_player);
        rollback_move(moves[depth][i], depth);

        if( score > max ) {
            out_move2[1] = moves[depth][i];
            max = score;
        }

    }

    return max;
}

int mini( int depth, int current_player) {
    int score;
    if ( depth == 0 ) return -evaluate(position, current_player);
    int min = 300;


    moves[depth][0].MoveType = -1;
    generate_moves(depth, current_player);


    int count;
    for (count = 0; moves[depth][count].MoveType != -1; count++);
    //printf("count mini %d\n", count);

//    if (moves[depth][0].MoveType == -1) {
//
//        if (king_is_checked(WHITE)) {
//
//            return -200;
//        }
//        else if (king_is_checked(BLACK)) {
//
//            return 200;
//        }
//        else {
//
//            return 0;
//        }
//    }

    for (int i = 0; moves[depth][i].MoveType != -1; i++) {

        make_move(moves[depth][i], depth);
        score = maxi( depth - 1, !current_player);
        rollback_move(moves[depth][i], depth);

        if( score < min ) {
            out_move2[0] = moves[depth][i];
            min = score;
        }

    }

    return min;
}