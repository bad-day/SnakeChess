#include <stdio.h>

#include "config.h"
#include "board.h"
#include "evaluate.h"
#include "move.h"
#include "hash.h"

extern Board position; // main.c
extern MOVE moves[DEPTH][200]; // move.c
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
    int black_bonus = 0, white_bonus = 0;
    int count_pieces = 0, endspiel = 0;

    // scan the board in search of a pieces
    for (int i = 0; i < 8; i++) {
        for (int j = 68 + i * 16; j < 76 + i * 16; j++) {
            if (position[j] != 0) {

                cell = position[j];
                color = cell & MASK_COLOR;
                type = cell & MASK_TYPE;

                if (type != CELL_EMPTY) {

                    count_pieces++;
                }

                if (color == WHITE) {
                    switch (type) {

                        case FIGURE_TYPE_QUEEN:
                            white += QUEEN_WEIGH;
                            break;

                        case FIGURE_TYPE_ROOK:
                            white += ROOK_WEIGH;
                            break;

                        case FIGURE_TYPE_BISHOP:
                            white += BISHOP_WEIGH;
                            white_bonus += additional_points(FIGURE_TYPE_BISHOP, WHITE, j);
                            break;

                        case FIGURE_TYPE_KNIGHT:
                            white += KNIGHT_WEIGH;
                            white_bonus += additional_points(FIGURE_TYPE_KNIGHT, WHITE, j);
                            break;

                        case FIGURE_TYPE_PAWN:
                            white += PAWN_WEIGH;
                            white_bonus += additional_points(FIGURE_TYPE_PAWN, WHITE, j);
                            break;
                    }
                }
                else {
                    switch (type) {

                        case FIGURE_TYPE_QUEEN:
                            black += QUEEN_WEIGH;
                            break;

                        case FIGURE_TYPE_ROOK:
                            black += ROOK_WEIGH;
                            break;

                        case FIGURE_TYPE_BISHOP:
                            black += BISHOP_WEIGH;
                            black_bonus += additional_points(FIGURE_TYPE_BISHOP, BLACK, j);
                            break;

                        case FIGURE_TYPE_KNIGHT:
                            black += KNIGHT_WEIGH;
                            black_bonus += additional_points(FIGURE_TYPE_KNIGHT, BLACK, j);
                            break;

                        case FIGURE_TYPE_PAWN:
                            black += PAWN_WEIGH;
                            black_bonus += additional_points(FIGURE_TYPE_PAWN, BLACK, j);
                            break;
                    }
                }
            }
        }
    }

    if(count_pieces <= 16)
        endspiel = 1;

    for (int i = 0; i < 8; i++) {
        for (int j = 68 + i * 16; j < 76 + i * 16; j++) {
            if (position[j] != 0) {

                cell = position[j];
                color = cell & MASK_COLOR;
                type = cell & MASK_TYPE;

                if (color == WHITE && type == FIGURE_TYPE_KING) {

                    white_bonus += king_additional_points(j, endspiel);
                }

                if (color == BLACK && type == FIGURE_TYPE_KING) {

                    black_bonus += king_additional_points(j, endspiel);
                }
            }
        }
    }
    int mobility = 0, material = 0, bonus = 0;

    material = white - black;
    mobility = get_max_count_move(1) - get_max_count_move(0);
    bonus = white_bonus - black_bonus;

    int score = material + 4 * mobility + bonus;
    if (player) {

        return score;
    }
    else {

        return -score;
    }
}


// bonus points
int additional_points(int type, int color, int coord) {

    int pawn_black_add[64] =
            {0, 0, 0, 0, 0, 0, 0, 0,
             4, 4, 4, 0, 0, 4, 4, 4,
             6, 8, 2, 10, 10, 2, 8, 6,
             6, 8, 12, 16, 16, 12, 8, 6,
             8, 12, 16, 24, 24, 16, 12, 8,
             12, 16, 24, 32, 32, 24, 16, 12,
             12, 16, 24, 32, 32, 24, 16, 12,
             0, 0, 0, 0, 0, 0, 0, 0};

    int pawn_white_add[64] =
            {0, 0, 0, 0, 0, 0, 0, 0,
             12, 16, 24, 32, 32, 24, 16, 12,
             12, 16, 24, 32, 32, 24, 16, 12,
             8, 12, 16, 24, 24, 16, 12, 8,
             6, 8, 12, 16, 16, 12, 8, 6,
             6, 8, 2, 10, 10, 2, 8, 6,
             4, 4, 4, 0, 0, 4, 4, 4,
             0, 0, 0, 0, 0, 0, 0, 0};

    int knight[64] =
            {0, 4, 8, 10, 10, 8, 4, 0,
             4, 8, 16, 20, 20, 16, 8, 4,
             8, 16, 24, 28, 28, 24, 16, 8,
             10, 20, 28, 32, 32, 28, 20, 10,
             10, 20, 28, 32, 32, 28, 20, 10,
             8, 16, 24, 28, 28, 24, 16, 8,
             4, 8, 16, 20, 20, 16, 8, 4,
             0, 4, 8, 10, 10, 8, 4, 0};

    int bishop[64] =
            {14, 14, 14, 14, 14, 14, 14, 14,
             14, 22, 18, 18, 18, 18, 22, 14,
             14, 18, 22, 22, 22, 22, 18, 14,
             14, 18, 22, 22, 22, 22, 18, 14,
             14, 18, 22, 22, 22, 22, 18, 14,
             14, 18, 22, 22, 22, 22, 18, 14,
             14, 22, 18, 18, 18, 18, 22, 14,
             14, 14, 14, 14, 14, 14, 14, 14};

    coord -= 64;
    int x = coord % 16 - 4;
    int y = 8 - coord / 16;
    int arr_coord = (8 - y) * 8 + x;

    if (type == FIGURE_TYPE_PAWN) {

        if (color == WHITE) {

            return pawn_white_add[arr_coord];
        }
        else {

            return pawn_black_add[arr_coord];
        }
    }

    if (type == FIGURE_TYPE_KNIGHT) {

        return knight[arr_coord];
    }

    if (type == FIGURE_TYPE_BISHOP) {

        return bishop[arr_coord];
    }

    return 0;
}

// bonus for king
int king_additional_points(int coord, int endspiel) {

    int king1[64] =
            {0, 0, -4, -10, -10, -4, 0, 0,
             -4, -4, -8, -12, -12, -8, -4, -4,
             -12, -16, -20, -20, -20, -20, -16, -12,
             -16, -20, -24, -24, -24, -24, -20, -16,
             -16, -20, -24, -24, -24, -24, -20, -16,
             -12, -16, -20, -20, -20, -20, -16, -12,
             -4, -4, -8, -12, -12, -8, -4, -4,
             0, 0, -4, -10, -10, -4, 0, 0};

    int king2[64] =
            {0, 6, 12, 18, 18, 12, 6, 0,
             6, 12, 18, 24, 24, 18, 12, 6,
             12, 18, 24, 30, 30, 24, 18, 12,
             18, 24, 30, 36, 36, 30, 24, 18,
             18, 24, 30, 36, 36, 30, 24, 18,
             12, 18, 24, 30, 30, 24, 18, 12,
             6, 12, 18, 24, 24, 18, 12, 6,
             0, 6, 12, 18, 18, 12, 6, 0};

    coord -= 64;
    int x = coord % 16 - 4;
    int y = 8 - coord / 16;
    int arr_coord = (8 - y) * 8 + x;

    if(endspiel) {

        return king2[arr_coord];
    }

    return king1[arr_coord];
}