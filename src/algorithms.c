#include "config.h"
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

extern HASH_TABLE hash_table[MAX_HASH_TABLE_SIZE]; // hash.c
extern unsigned long current_hash; // hash.c
extern unsigned long zobrist_key_null_move; // hash.c

int alpha_beta(int alpha, int beta, int depth, int level, int current_player, MOVE_TYPE last_move_type, int null_move,
               int extended) {

    if (!uci_work_status) {

        return -INF;
    }

    count_nodes++;
    int score, king_is_check = 0;

    HASH_TABLE *hash_ptr;
    hash_ptr = &hash_table[current_hash % (MAX_HASH_TABLE_SIZE)];

    if (hash_ptr->type != HASH_TABLE_TYPE_EMPTY && hash_ptr->level <= level && hash_ptr->key == current_hash) {

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

        depth += CHECK_DEEPENING;
        extended = 1;
        king_is_check = 1;
    }

    if (level != 0 && !king_is_check && !extended && last_move_type != MOVE_TYPE_EAT) {

        int ev = evaluate(current_player);
        int margin;

        // Razoring pruning
/*        if (!null_move && depth >= 0 && depth <= RAZORING_DEPTH) {

            int razoring_margin_table[5] = {2 * PAWN_WEIGH, 2 * PAWN_WEIGH, 3 * PAWN_WEIGH, 4 * PAWN_WEIGH,
                                            5 * PAWN_WEIGH};
            margin = razoring_margin_table[depth];
            if (ev < beta - margin) {

                return ev;
            }
        }*/

        // Futility pruning
        if (!null_move && depth >= 0 && depth <= FUTILITY_DEPTH) {

            int futility_margin_table[5] = {2 * PAWN_WEIGH, 2 * PAWN_WEIGH, 3 * PAWN_WEIGH, 4 * PAWN_WEIGH,
                                            5 * PAWN_WEIGH};
            margin = futility_margin_table[depth];
            if (ev >= beta + margin) {

                return ev;
            }
        }

        if (!null_move && ev >= beta && depth != 0) {

            current_hash ^= zobrist_key_null_move;
            score = -alpha_beta(-beta, -beta + 1, depth - 1 - NULL_MOVE_DEPTH, level + 1, !current_player,
                                last_move_type, 1, extended);
            current_hash ^= zobrist_key_null_move;

            if (score >= beta) {

                return beta;
            }
        }
    }

    if (depth <= 0 || level >= max_current_deep + FORCED_MATE_DEEP) {

        return quiesce(alpha, beta, current_player, DEPTH - 1); // DEPTH - 1 for quiet search;
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
    for (int i = 0; i < 200 && moves[level][i].MoveType != MOVE_TYPE_EMPTY; i++) {

        make_move(moves[level][i], level);
        score = -alpha_beta(-beta, -alpha, depth - 1, level + 1, !current_player, moves[level][i].MoveType,
                            null_move, extended);
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
            best_move_to_table(current_hash, level, moves[level][i]);
        }

    }

    hash_to_table(current_hash, alpha, level, flag);
    return alpha;
}

/*
int search() {

    int score = 0;
    count_nodes = 0;

    for (int depth = 1; depth < DEPTH; depth++) {

        int lo = 20;
        int hi = 20;

        while (1) {

            int alpha = score - lo;
            int beta = score + hi;
            score = root_search(alpha, beta, depth, 0, current_player);

            if (score == alpha) {

                lo *= 5;
            }
            else if (score == beta) {

                hi *= 5;
            }
            else {

                break;
            }
            if(!uci_work_status) {

                break;
            }
        }

    }
}*/
