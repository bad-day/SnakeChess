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
extern int uci_work_status; // uci.c статус работы от GUI
extern int max_current_deep; // uci.c главная глубина, надо потом переделать все на итеративный режим
extern MOVE out_move[2]; // uci.c сохраняем данные для uci

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

    moves[depth][0].MoveType = -1;
    generate_moves(depth, current_player);

    if (moves[depth][0].MoveType == -1) {

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
    for (int i = 0; moves[depth][i].MoveType != -1; i++) {

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

        moves[depth][i].MoveType = -1;
    }


    return score_best;
}

int alpha_beta(int alpha, int beta, int depth , int current_player) {

    if (!uci_work_status) {

        return UCI_EXIT;
    }

    int score_best;

    if( depth == 0 ) {

        return quiesce(alpha, beta, current_player, 99 );
    }

    generate_moves(depth, current_player); //  все ходы в moves[depth]
    sort_move(depth);

    if(current_player) {

        if (moves[depth][0].MoveType == -1) {

            if (king_is_checked(WHITE)) {

                return -CHECKMATE;
            }

            return 0;
        }

        score_best = -1000000;

        for (int i = 0; i < 200; i++) {

            if (moves[depth][i].MoveType != -1) {

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

        if (moves[depth][0].MoveType == -1) {

            if (king_is_checked(BLACK)) {

                return CHECKMATE;
            }

            return 0;
        }

        score_best = 1000000;

        for (int i = 0; i < 200; i++) {

            if (moves[depth][i].MoveType != -1) {

                make_move(moves[depth][i], depth);
                int score = alpha_beta(alpha, beta, depth - 1, !current_player);
                rollback_move(moves[depth][i], depth);

                if(score < score_best) {

                    score_best = score;
                    if (depth == max_current_deep) {

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