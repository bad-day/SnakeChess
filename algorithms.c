//
// Created by valera on 14.11.17.
//

#include "algorithms.h"
#include "evaluate.h"
#include "board.h"
#include "move.h"
#include "uci.h"
#include <stdio.h>
#include <stdlib.h>

extern Board position; // main.c

extern MOVE moves[DEPTH][200]; // move.c
extern int uci_work_status; // uci.c working status from GUI
extern int max_current_deep; // uci.c the main deep, need change algo to iteration mode
extern unsigned int count_nodes; // uci.c count of nodes
extern MOVE out_move[2]; // uci.c save best move for the UCI


int minimax(int depth, int current_player) {

    int score_best;
    int score;

    if (!uci_work_status) {

        return UCI_EXIT;
    }

    if (current_player) {

        score_best = -CHECKMATE + depth;
    }
    else {

        score_best = CHECKMATE - depth;
    }

    if (depth == 0) { // дошли до листка

        return evaluate(current_player);
    }

    generate_moves(depth, current_player);

    if (moves[depth][0].MoveType == MOVE_TYPE_EMPTY) {

        if (current_player && king_is_checked(WHITE)) {

            return -CHECKMATE;
        }

        if (!current_player && king_is_checked(BLACK)) {

            return CHECKMATE;
        }

        // пат
        return 0;
    }

    // проходимся по новым ходам
    for (int i = 0; moves[depth][i].MoveType != MOVE_TYPE_EMPTY; i++) {

        make_move(moves[depth][i], depth); // тут меняется состояние доски

        score = minimax(depth - 1, !current_player);

        rollback_move(moves[depth][i], depth); // возвращаем прежнюю доску


        if (current_player) { // если белые, максимимзируем

            if (score > score_best) {

                if (depth == max_current_deep) {

                    out_move[1] = moves[depth][i];
                }

                score_best = score; // максимальное в данном поддереве
            }
        }
        else { // минимизируем

            if (score < score_best) {

                if (depth == max_current_deep) {

                    out_move[0] = moves[depth][i];
                }
                score_best = score;
            }
        }

    }


    return score_best;
}

int alpha_beta(int alpha, int beta, int depth , int current_player) {

    if (!uci_work_status) {

        return UCI_EXIT;
    }

    int score_best;

    if( depth == 0 ) {

        count_nodes++;
        return quiesce(alpha, beta, current_player, DEPTH - 1 ); // DEPTH - 1 for quiet search
    }

    generate_moves(depth, current_player); //  all moves to moves[depth]
    sort_move(depth);

    if(current_player) {

        if (moves[depth][0].MoveType == MOVE_TYPE_EMPTY) {

            if (king_is_checked(WHITE)) {

                return -CHECKMATE;
            }

            return 0;
        }

        score_best = -1000000;

        for (int i = 0; i < 200; i++) {

            if (moves[depth][i].MoveType != MOVE_TYPE_EMPTY) {

                make_move(moves[depth][i], depth);
                int score = alpha_beta(alpha, beta, depth - 1, !current_player);
                rollback_move(moves[depth][i], depth);

                if(score > score_best) {

                    score_best = score;
                    if (depth == max_current_deep) {

                        out_move[1] = moves[depth][i];


                    }
                }

                if(score_best > alpha) {

                    alpha = score_best;
                }

                if (beta <= alpha) {

                    break;
                }
            }
        }

        return score_best;
    }
    else {

        if (moves[depth][0].MoveType == MOVE_TYPE_EMPTY) {

            if (king_is_checked(BLACK)) {

                return CHECKMATE;
            }

            return 0;
        }

        score_best = 1000000;

        for (int i = 0; i < 200; i++) {

            if (moves[depth][i].MoveType != MOVE_TYPE_EMPTY) {

                make_move(moves[depth][i], depth);
                int score = alpha_beta(alpha, beta, depth - 1, !current_player);
                rollback_move(moves[depth][i], depth);

                if(score < score_best) {

                    score_best = score;
                    if (depth == max_current_deep) {
                        //who_is_cell(position, moves[depth][i].current_position);
                        //printf(" %d %d", moves[depth][i].current_position, moves[depth][i].next_position);
                        out_move[0] = moves[depth][i];
                    }
                }

                if(score_best < beta) {

                    beta = score_best;
                }

                if (beta <= alpha) {

                    break;
                }
            }
        }

        return score_best;
    }
}