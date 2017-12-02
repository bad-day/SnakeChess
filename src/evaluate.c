#include <stdio.h>

#include "board.h"
#include "evaluate.h"
#include "move.h"
#include "hash.h"

extern Board position; // main.c
extern MOVE moves[DEPTH][200]; // move.c

extern HASH_TABLE hash_table_white[MAX_HASH_TABLE_SIZE]; // hash.c
extern HASH_TABLE hash_table_black[MAX_HASH_TABLE_SIZE]; // hash.c

extern unsigned long current_hash; // hash.c

extern unsigned int count_nodes; // uci.c count of nodes

int quiesce(int alpha, int beta, int current_player, int depth) {

    // if static evaluate, uncomment
    //return evaluate(current_player);

    count_nodes++;
    int stand_pat = evaluate(current_player);

    if (stand_pat >= beta) {

        return beta;
    }

    if (alpha < stand_pat) {

        alpha = stand_pat;
    }


    generate_moves(depth, current_player);
    sort_move(depth);

    for (int i = 0; i < 200; i++) {

        if (moves[depth][i].MoveType == MOVE_TYPE_EAT) {

            make_move(moves[depth][i], depth);

            int score = -quiesce(-beta, -alpha, !current_player, depth - 1);

            rollback_move(moves[depth][i], depth);

            if (score >= beta) {

                return beta;
            }

            if (score > alpha) {

                alpha = score;
            }
        }
    }

    return alpha;
}

// evaluate function
int evaluate(int player) {

    int cell, type, color;
    int black = 0, white = 0;

    // scan the board in search of a pieces
    for (int i = 0; i < 8; i++) {
        for (int j = 68 + i * 16; j < 76 + i * 16; j++) {
            if (position[j] != 0) {

                cell = position[j];
                color = cell & MASK_COLOR;
                type = cell & MASK_TYPE;

                if (color == WHITE) {
                    switch (type) {

                        case FIGURE_TYPE_QUEEN:
                            white += 900;
                            break;

                        case FIGURE_TYPE_ROOK:
                            white += 500;
                            break;

                        case FIGURE_TYPE_BISHOP:
                            white += 300;
                            break;

                        case FIGURE_TYPE_KNIGHT:
                            white += 300;
                            break;

                        case FIGURE_TYPE_PAWN:
                            white += 100;
                            break;
                    }
                }
                else {
                    switch (type) {

                        case FIGURE_TYPE_QUEEN:
                            black += 900;
                            break;

                        case FIGURE_TYPE_ROOK:
                            black += 500;
                            break;

                        case FIGURE_TYPE_BISHOP:
                            black += 300;
                            break;

                        case FIGURE_TYPE_KNIGHT:
                            black += 300;
                            break;

                        case FIGURE_TYPE_PAWN:
                            black += 100;
                            break;
                    }
                }
            }
        }
    }

    int mobility, material;

    mobility = get_max_count_move(1) - get_max_count_move(0);
    material = white - black;

    //return material + 5 * mobility;
    if (player) {

        return material + 5 * mobility;
    }
    else {

        return -(material + 5 * mobility);
    }

}
