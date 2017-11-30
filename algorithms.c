//
// Created by valera on 14.11.17.
//

#include "algorithms.h"
#include "evaluate.h"
#include "board.h"
#include "move.h"
#include "uci.h"
#include "hash.h"
#include <stdio.h>
#include <stdlib.h>

extern Board position; // main.c

extern MOVE moves[DEPTH][200]; // move.c
extern int uci_work_status; // uci.c working status from GUI
extern int max_current_deep; // uci.c the main deep, need change algorithm to iteration mode
extern unsigned int count_nodes; // uci.c count of nodes
extern MOVE out_move[2]; // uci.c save best move for the UCI

extern HASH_TABLE hash_table_white[MAX_HASH_TABLE_SIZE]; // hash.c
extern HASH_TABLE hash_table_black[MAX_HASH_TABLE_SIZE]; // hash.c

extern unsigned long current_hash; // hash.c
extern unsigned long zobrist_key_move; // hash.c

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

    HASH_TABLE *hash_ptr;

    if (current_player)
        hash_ptr = &hash_table_white[current_hash % (MAX_HASH_TABLE_SIZE)];
    else
        hash_ptr = &hash_table_black[current_hash % (MAX_HASH_TABLE_SIZE)];

    if (hash_ptr->type == HASH_TABLE_TYPE_EXACT && hash_ptr->deep >= depth && hash_ptr->key == current_hash) {

        return hash_ptr->score;
    }

    if (depth == 0) { // terminated node

        // count of node
        count_nodes++;

        int last_score = evaluate(current_player);

        hash_to_table(current_hash, last_score, depth, current_player); // write hash to table

        return last_score;
    }

    generate_moves(depth, current_player);
    sort_move(depth);

    if (moves[depth][0].MoveType == MOVE_TYPE_EMPTY) {

        if (current_player && king_is_checked(WHITE)) {

            hash_to_table(current_hash, -CHECKMATE, depth, current_player);
            return -CHECKMATE;
        }

        if (!current_player && king_is_checked(BLACK)) {

            hash_to_table(current_hash, CHECKMATE, depth, current_player);
            return CHECKMATE;
        }

        hash_to_table(current_hash, 0, depth, current_player);
        return 0;
    }

    // going from the moves
    for (int i = 0; moves[depth][i].MoveType != MOVE_TYPE_EMPTY; i++) {

        make_move(moves[depth][i], depth); // make a move, change board

        score = minimax(depth - 1, !current_player);

        rollback_move(moves[depth][i], depth); // rollback and board


        if (current_player) { // maxi

            if (score > score_best) {

                if (depth == max_current_deep) {

                    out_move[1] = moves[depth][i];
                }

                score_best = score; // max from under three
            }
        }
        else { // mini

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

int alpha_beta(int alpha, int beta, int depth, int current_player) {

    if (!uci_work_status) {

        return UCI_EXIT;
    }

    int score_best;

    HASH_TABLE *hash_ptr;

    if (current_player)
        hash_ptr = &hash_table_white[current_hash & (MAX_HASH_TABLE_SIZE - 1)];
    else
        hash_ptr = &hash_table_black[current_hash & (MAX_HASH_TABLE_SIZE - 1)];

    if (hash_ptr->type == HASH_TABLE_TYPE_EXACT && hash_ptr->deep >= depth && hash_ptr->key == current_hash) {

        //if(hash_ptr->score > alpha && hash_ptr->score < beta)
        return hash_ptr->score;
    }


    if (depth == 0) {

        // count of processed nodes
        count_nodes++;

        int score_out = quiesce(alpha, beta, current_player, DEPTH - 1); // DEPTH - 1 for quiet search

        hash_to_table(current_hash, score_out, depth, current_player); // write hash to table

        return score_out;
    }

    generate_moves(depth, current_player); //  all moves to moves[depth]
    sort_move(depth);

    if (current_player) {

        if (moves[depth][0].MoveType == MOVE_TYPE_EMPTY) {

            if (king_is_checked(WHITE)) {

                hash_to_table(current_hash, -CHECKMATE - depth, depth, current_player);
                return -CHECKMATE - depth;
            }

            hash_to_table(current_hash, 0, depth, current_player);
            return 0;
        }

        score_best = -1000000;

        for (int i = 0; i < 200; i++) {

            if (moves[depth][i].MoveType != MOVE_TYPE_EMPTY) {

                make_move(moves[depth][i], depth);
                int score = alpha_beta(alpha, beta, depth - 1, !current_player);
                rollback_move(moves[depth][i], depth);

                if (score > score_best) {

                    score_best = score;
                    if (depth == max_current_deep) {

                        out_move[1] = moves[depth][i];
                    }
                }

                if (score_best > alpha) {

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

                hash_to_table(current_hash, CHECKMATE + depth, depth, current_player);
                return CHECKMATE + depth;
            }

            hash_to_table(current_hash, 0, depth, current_player);
            return 0;
        }

        score_best = 1000000;

        for (int i = 0; i < 200; i++) {

            if (moves[depth][i].MoveType != MOVE_TYPE_EMPTY) {

                make_move(moves[depth][i], depth);
                int score = alpha_beta(alpha, beta, depth - 1, !current_player);
                rollback_move(moves[depth][i], depth);

                if (score < score_best) {

                    score_best = score;
                    if (depth == max_current_deep) {

                        out_move[0] = moves[depth][i];
                    }
                }

                if (score_best < beta) {

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