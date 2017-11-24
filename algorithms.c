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

int alpha_beta(int alpha, int beta, int depth, int current_player) {

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

    if (depth - 50 <= 0) { // дошли до листка

        return evaluate(current_player);
        score = evaluate(current_player);

        if (current_player) { // если белые, максимимзируем

            if (score > beta) {

                return score;
            }

            if (score > score_best) {

                score_best = score;
                if (score > alpha) {

                    alpha = score;
                }

            }
        }
        else { // минимизируем

            if (score < alpha) {

                return score;
            }

            if (score < score_best) {

                score_best = score;
                if (score < beta) {

                    beta = score;
                }
            }
        }

        generate_moves(depth, current_player); // поулчаем все ходы в moves[depth]

        if (moves[depth][0].MoveType == -1) {

            if (current_player && king_is_checked(WHITE)) {

                return -CHECKMATE;
            }

            if (!current_player && king_is_checked(BLACK)) {

                return CHECKMATE;
            }

            return 0;
        }

        for (int i = 0; moves[depth][i].MoveType != -1; i++) {

            if(moves[depth][i].MoveType == 1) {

                make_move(moves[depth][i], depth); // тут меняется состояние доски

                score = alpha_beta(alpha, beta, depth - 1, !current_player);

                rollback_move(moves[depth][i], depth); // возвращаем прежнюю доску

//                if (current_player) { // если белые, максимимзируем
//
//                    if (score > beta) {
//
//                        return score;
//                    }
//
//                    if (score > score_best) {
//
//                        score_best = score;
//                        if (score > alpha) {
//
//                            alpha = score;
//                        }
//
//                    }
//                }
//                else { // минимизируем
//
//                    if (score < alpha) {
//
//                        return score;
//                    }
//
//                    if (score < score_best) {
//
//                        score_best = score;
//                        if (score < beta) {
//
//                            beta = score;
//                        }
//
//                    }
//                }


            }
            moves[depth][0].MoveType = -1;
        }

        return score_best;
    }

    //moves[depth][0].MoveType = -1;
    generate_moves(depth, current_player); // поулчаем все ходы в moves[depth]
    sort_move(depth);


    if (moves[depth][0].MoveType == -1) {

        if (current_player && king_is_checked(WHITE)) {

            return -CHECKMATE;
        }

        if (!current_player && king_is_checked(BLACK)) {

            return CHECKMATE;
        }

        return 0;
    }


    for (int i = 0; moves[depth][i].MoveType != -1; i++) {
        //

        make_move(moves[depth][i], depth); // тут меняется состояние доски

        score = alpha_beta(alpha, beta, depth - 1, !current_player);

        rollback_move(moves[depth][i], depth); // возвращаем прежнюю доску



        if (current_player) { // если белые, максимимзируем

            if (score > beta) {

                return score;
            }

            if (score > score_best) {

                score_best = score;
                if (score > alpha) {

                    alpha = score;
                }

                if (depth == max_current_deep + 50) {

                    out_move[1] = moves[depth][i];
                }
            }
        }
        else { // минимизируем



            if (score < alpha) {

                return score;
            }

            if (score < score_best) {

                score_best = score;
                if (score < beta) {

                    beta = score;
                }


                if (depth == max_current_deep + 50) {

                    //if(moves[depth][i].current_position == 104 && moves[depth][i].next_position == 104 + 17 ) {
                        //printf("%d %d %d\n", moves[depth][i].current_position, moves[depth][i].next_position, moves[depth][i].MoveType);
                        //printf("score: %d score_best %d alpha %d depth %d\n", score, score_best, alpha, depth);
                    //}
                    out_move[0] = moves[depth][i];
                }
            }
        }

        moves[depth][i].MoveType = -1;
    }

    return score_best;
}


int alphaBeta( int alpha, int beta, int depth , int current_player) {

    int bestscore = -99999999;
    int score;


    if (!uci_work_status) {

        return UCI_EXIT;
    }

    if( depth == 0 ) return evaluate(current_player);

    generate_moves(depth, current_player); // поулчаем все ходы в moves[depth]
    sort_move(depth);

    for (int i = 0; i < 200; i++) {

        if(moves[depth][i].MoveType != -1) {
            make_move(moves[depth][i], depth); // тут меняется состояние доски

            score = -alphaBeta( -beta, -alpha, depth - 1, !current_player );

            rollback_move(moves[depth][i], depth); // возвращаем прежнюю доску


            if( score >= beta )
                return score;  // fail-soft beta-cutoff

            if( score > bestscore ) {

                if (depth == max_current_deep) {

                    out_move[0] = moves[depth][i];
                }

                bestscore = score;
                if( score > alpha ) {}
                    alpha = score;
            }
        }
        moves[depth][i].MoveType = -1;
    }

    return bestscore;
}

int my_alpha(int alpha, int beta, int depth , int current_player) {

    int v;

    if( depth == 0 )
        return evaluate(current_player);


    if(current_player) {

        v = -1000000;

        generate_moves(depth, current_player); // поулчаем все ходы в moves[depth]
        sort_move(depth);
        for (int i = 0; i < 200; i++) {

            if (moves[depth][i].MoveType != -1) {

                make_move(moves[depth][i], depth);
                int score = my_alpha(alpha, beta, depth - 1, !current_player);
                rollback_move(moves[depth][i], depth);

                if(score > v) {

                    v = score;
                    if (depth == max_current_deep) {

                        out_move[1] = moves[depth][i];
                    }
                }

                if(v > alpha) {
                    alpha = v;
                }

                if (beta <= alpha) {

                    break;
                }

            }


        }
        return v;
    }
    else {

        v = 1000000;

        generate_moves(depth, current_player); // поулчаем все ходы в moves[depth]
        sort_move(depth);
        for (int i = 0; i < 200; i++) {

            if (moves[depth][i].MoveType != -1) {

                make_move(moves[depth][i], depth);
                int score = my_alpha(alpha, beta, depth - 1, !current_player);
                rollback_move(moves[depth][i], depth);

                if(score < v) {

                    v = score;
                    if (depth == max_current_deep) {

                        out_move[0] = moves[depth][i];
                    }
                }


                if(v < beta) {
                    beta = v;
                }

                if (beta <= alpha) {

                    break;
                }

            }

        }

        return v;
    }
}

