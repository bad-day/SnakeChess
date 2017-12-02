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

extern HASH_TABLE hash_table[MAX_HASH_TABLE_SIZE]; // hash.c
extern unsigned long current_hash; // hash.c
extern unsigned long zobrist_key_move; // hash.c
extern unsigned long zobrist_key_null_move; // hash.c

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

    current_hash ^= zobrist_key_move;

    HASH_TABLE *hash_ptr;
    hash_ptr = &hash_table[current_hash % (MAX_HASH_TABLE_SIZE)];

    if (hash_ptr->type == HASH_TABLE_TYPE_EXACT && hash_ptr->deep >= depth && hash_ptr->key == current_hash) {

        return hash_ptr->score;
    }

    if (depth == 0) { // terminated node

        // count of node
        count_nodes++;

        int last_score = evaluate(current_player);

        hash_to_table(current_hash, last_score, depth, HASH_TABLE_TYPE_EXACT); // write hash to table

        return last_score;
    }

    generate_moves(depth, current_player);
    sort_move(depth);

    if (moves[depth][0].MoveType == MOVE_TYPE_EMPTY) {

        if (current_player && king_is_checked(WHITE)) {

            hash_to_table(current_hash, -CHECKMATE, depth, HASH_TABLE_TYPE_EXACT);
            return -CHECKMATE;
        }

        if (!current_player && king_is_checked(BLACK)) {

            hash_to_table(current_hash, CHECKMATE, depth, HASH_TABLE_TYPE_EXACT);
            return CHECKMATE;
        }

        hash_to_table(current_hash, 0, depth, HASH_TABLE_TYPE_EXACT);
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

    hash_to_table(current_hash, score_best, depth, HASH_TABLE_TYPE_EXACT);
    return score_best;
}

int alpha_beta(int alpha, int beta, int depth, int current_player) {

    if (!uci_work_status) {

        return -INF;
    }

    int score;
    current_hash ^= zobrist_key_move;

    HASH_TABLE *hash_ptr;
    hash_ptr = &hash_table[current_hash % (MAX_HASH_TABLE_SIZE)];

    if (hash_ptr->deep >= depth && hash_ptr->key == current_hash) {

        // I can't understand why && (hash_ptr->score >= beta) working more faster, which without this
        if (hash_ptr->type == HASH_TABLE_TYPE_EXACT && (hash_ptr->score >= beta)) {

            return hash_ptr->score;
        }

        if (hash_ptr->type == HASH_TABLE_TYPE_ALPHA && hash_ptr->score <= alpha) {

            return hash_ptr->score;
        }

        if (hash_ptr->type == HASH_TABLE_TYPE_BETA && hash_ptr->score >= beta) {

            return hash_ptr->score;
        }
    }

    if (depth <= 0) {

        count_nodes++;

        score = quiesce(alpha, beta, current_player, DEPTH - 1); // DEPTH - 1 for quiet search
        hash_to_table(current_hash, score, depth, HASH_TABLE_TYPE_EXACT); // write hash to table

        return score;
    }

    // try null move
//    if(depth != max_current_deep ) {
//
//        current_hash ^= zobrist_key_null_move;
//        score = -alpha_beta(-beta, -beta + 1, depth - 1 - 2, current_player);
//        current_hash ^= zobrist_key_null_move;
//
//        if (score >= beta) {
//            hash_to_table(current_hash, score, depth, HASH_TABLE_TYPE_BETA);
//            return beta;
//        }
//    }

    generate_moves(depth, current_player);
    sort_move(depth);

    if (moves[depth][0].MoveType == MOVE_TYPE_EMPTY) {

        if (king_is_checked(WHITE) || king_is_checked(BLACK)) {

            return -CHECKMATE - depth;
        }

        return 0;
    }

    int flag = HASH_TABLE_TYPE_ALPHA;
    for (int i = 0; i < 200; i++) {

        if (moves[depth][i].MoveType != MOVE_TYPE_EMPTY) {

            make_move(moves[depth][i], depth);
            score = -alpha_beta(-beta, -alpha, depth - 1, !current_player);
            rollback_move(moves[depth][i], depth);

            if (score >= beta) {

                hash_to_table(current_hash, beta, depth, HASH_TABLE_TYPE_BETA);
                //move_to_table(current_hash, depth, moves[depth][i]);
                return beta;
            }
            if (score > alpha) {

                flag = HASH_TABLE_TYPE_EXACT;
                alpha = score;
                //move_to_table(current_hash, depth, moves[depth][i]);

                if (depth == max_current_deep) {
                    out_move[0] = moves[depth][i];
                }
            }
        }
    }

    hash_to_table(current_hash, alpha, depth, flag);
    return alpha;
}