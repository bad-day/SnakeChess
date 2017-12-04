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
extern unsigned long zobrist_key_move; // hash.c // hash.c
extern unsigned long zobrist_key_null_move; // hash.c

int alpha_beta(int alpha, int beta, int depth, int level, int current_player, int null_move) {

    if (!uci_work_status) {

        return -INF;
    }

    int score, king_is_check = 0;

    current_hash ^= zobrist_key_move;

    HASH_TABLE *hash_ptr;
    hash_ptr = &hash_table[current_hash % (MAX_HASH_TABLE_SIZE)];

    if (hash_ptr->level <= level && hash_ptr->key == current_hash) {

        score = hash_ptr->score;

        if (hash_ptr->type == HASH_TABLE_TYPE_EXACT) {

            return score;
        }

        if (hash_ptr->type == HASH_TABLE_TYPE_ALPHA && score <= alpha) {

            return score;
        }

        if (hash_ptr->type == HASH_TABLE_TYPE_BETA && score >= beta) {

            return score;
        }
    }

    // Force the checks
    if (king_is_checked(WHITE) || king_is_checked(BLACK)) {

        depth += 1;
        king_is_check = 1;
    }

    if (depth <= 0 || level >= max_current_deep + 15) {

        return quiesce(alpha, beta, current_player, DEPTH - 1); // DEPTH - 1 for quiet search;
    }

    // try null move
    if (level != 0 && !null_move) {

        int ev = evaluate(current_player);

        if (ev >= beta && !king_is_check) {

            current_hash ^= zobrist_key_null_move;
            score = -alpha_beta(-beta, -beta + 1, depth - 1 - 2, level + 1, !current_player, 1);
            current_hash ^= zobrist_key_null_move;

            if (score >= beta) {

                return beta;
            }
        }
    }

    generate_moves(level, current_player);
    sort_move(level);

    if (moves[level][0].MoveType == MOVE_TYPE_EMPTY) {

        if (king_is_check) { // checkmate

            hash_to_table(current_hash, -CHECKMATE + level, level, HASH_TABLE_TYPE_EXACT);
            return -CHECKMATE + level;
        }

        hash_to_table(current_hash, 0, level, HASH_TABLE_TYPE_EXACT);
        return 0; // pat
    }

    int flag = HASH_TABLE_TYPE_ALPHA;
    for (int i = 0; i < 200; i++) {

        if (moves[level][i].MoveType != MOVE_TYPE_EMPTY) {

            make_move(moves[level][i], level);
            score = -alpha_beta(-beta, -alpha, depth - 1, level + 1, !current_player, null_move);
            rollback_move(moves[level][i], level);

            if (score >= beta) {

                hash_to_table(current_hash, beta, level, HASH_TABLE_TYPE_BETA);
                move_to_table(current_hash, level, moves[level][i]);
                return beta;
            }
            if (score > alpha) {

                alpha = score;

                flag = HASH_TABLE_TYPE_EXACT;
                move_to_table(current_hash, level, moves[level][i]);

                if (level == 0) {

                    out_move[0] = moves[level][i];
                }
            }
        }
    }

    hash_to_table(current_hash, alpha, level, flag);
    return alpha;
}