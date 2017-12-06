#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "config.h"
#include "move.h"
#include "board.h"
#include "hash.h"

extern Board position; // main.c
MOVE moves[DEPTH][200]; // all moves to the tree

extern HASH_TABLE hash_table[MAX_HASH_TABLE_SIZE]; // hash.c
extern unsigned long current_hash; // hash.c
extern unsigned long current_hash; // hash.c
extern unsigned long zobrist_key[FIGURE_TYPE_PAWN + 1][2][256]; // hash.c
extern unsigned long zobrist_key_move; // hash.c // hash.c

// all moves
int KingMove[9] = {16, -16, 1, -1, 17, -17, 15, -15, 0};
int QueenMove[9] = {16, -16, 1, -1, 17, -17, 15, -15, 0};
int RookMove[5] = {16, -16, 1, -1, 0};
int BishopMove[5] = {17, -17, 15, -15, 0};
int KnightMove[9] = {32 + 1, 32 - 1, 16 + 2, 16 - 2, -(32 + 1), -(32 - 1), -(16 + 2), -(16 - 2), 0};

Board old_position[DEPTH]; // positions for rollback
unsigned long old_hash[DEPTH]; // hash for rollback

// count moves for current deep
int count_move[DEPTH];

// not necessary
void move_init() {

    for (int i = 0; i < DEPTH; i++) {
        for (int j = 0; j < 200; j++) {

            moves[i][j].MoveType = MOVE_TYPE_EMPTY;
        }
    }
}

// generate moves
void generate_moves(int depth, int current_player) {

    count_move[depth] = 0;

    for (int i = 0; i < 200; i++) {

        moves[depth][i].MoveType = MOVE_TYPE_EMPTY;
    }

    int cell, color, cell_color;

    // color of piece
    if (current_player) {

        color = WHITE;
    }
    else {

        color = BLACK;
    }

    // scan board
    for (int i = 0; i < 8; i++) {
        for (int j = 68 + i * 16; j < 76 + i * 16; j++) {
            if (position[j] != 0) {

                cell = position[j];
                cell_color = cell & MASK_COLOR;

                if (cell_color == color) {
                    get_moves(j, depth);
                }
            }
        }
    }

}

// get moves for each piece
void get_moves(int coord, int depth) {

    int n = 0;
    int cell = position[coord];

    int color = cell & MASK_COLOR;
    int opponent_color = color ^ MASK_COLOR;
    int type = cell & MASK_TYPE;

    int cell_color, is_moved, is_passed;

    // figure type is king
    if (type == FIGURE_TYPE_KING) {

        n = 0;
        while (KingMove[n] != 0) {

            int new_coord = coord + KingMove[n]; // Watch the cell
            cell = position[new_coord];
            cell_color = cell & MASK_COLOR;

            if (cell == CELL_EMPTY) {

                add_move(depth, coord, new_coord, FIGURE_TYPE_KING, MOVE_TYPE_SIMPLY);
            }
            else if (cell_color == opponent_color) { // eat

                add_move(depth, coord, new_coord, FIGURE_TYPE_KING, MOVE_TYPE_EAT);
            }

            n++;
        }

        // castlings
        cell = position[coord];
        is_moved = cell & MASK_MOVE;
        if (is_moved != IS_MOVE) { // if king did't move

            // short castling
            if (position[coord + 1] == CELL_EMPTY && position[coord + 2] == CELL_EMPTY) {
                int is_rook = position[coord + 3];
                int type_is_rook = is_rook & MASK_TYPE;
                int rook_moved = is_rook & MASK_MOVE;

                // if rook did't move
                if (type_is_rook == FIGURE_TYPE_ROOK && rook_moved != IS_MOVE) {
                    // If the fields for the king do not under attack
                    if (!king_is_checked(color) && is_legal_move(coord, coord + 1) && is_legal_move(coord, coord + 2)) {

                        add_move(depth, coord, coord + 2, FIGURE_TYPE_KING, MOVE_TYPE_CASTLING_O_O);
                    }
                }
            }

            // long castling
            if (position[coord - 1] == CELL_EMPTY && position[coord - 2] == CELL_EMPTY &&
                position[coord - 3] == CELL_EMPTY) {

                int is_rook = position[coord - 4];
                int type_is_rook = is_rook & MASK_TYPE;
                int rook_moved = is_rook & MASK_MOVE;

                if (type_is_rook == FIGURE_TYPE_ROOK && rook_moved != IS_MOVE) {
                    if (!king_is_checked(color) && is_legal_move(coord, coord - 1) && is_legal_move(coord, coord - 2)) {

                        add_move(depth, coord, coord - 2, FIGURE_TYPE_KING, MOVE_TYPE_CASTLING_O_O_0);
                    }
                }
            }
        }
    }

    if (type == FIGURE_TYPE_QUEEN) {

        n = 0;
        while (QueenMove[n] != 0) {

            int new_coord = coord + QueenMove[n];
            while (position[new_coord] == CELL_EMPTY) {

                add_move(depth, coord, new_coord, FIGURE_TYPE_QUEEN, MOVE_TYPE_SIMPLY);
                new_coord += QueenMove[n];
            }

            cell = position[new_coord];
            cell_color = cell & MASK_COLOR;
            if (cell_color == opponent_color) {

                add_move(depth, coord, new_coord, FIGURE_TYPE_QUEEN, MOVE_TYPE_EAT);
            }
            n++;
        }
        return;
    }

    if (type == FIGURE_TYPE_ROOK) {

        n = 0;
        while (RookMove[n] != 0) {

            int new_coord = coord + RookMove[n];
            while (position[new_coord] == CELL_EMPTY) {

                add_move(depth, coord, new_coord, FIGURE_TYPE_ROOK, MOVE_TYPE_SIMPLY);
                new_coord += RookMove[n];
            }

            cell = position[new_coord];
            cell_color = cell & MASK_COLOR;
            if (cell_color == opponent_color) {

                add_move(depth, coord, new_coord, FIGURE_TYPE_ROOK, MOVE_TYPE_EAT);
            }
            n++;
        }
        return;
    }

    if (type == FIGURE_TYPE_BISHOP) {

        n = 0;
        while (BishopMove[n] != 0) {

            int new_coord = coord + BishopMove[n];
            while (position[new_coord] == CELL_EMPTY) {

                add_move(depth, coord, new_coord, FIGURE_TYPE_BISHOP, MOVE_TYPE_SIMPLY);
                new_coord += BishopMove[n];
            }

            cell = position[new_coord];
            cell_color = cell & MASK_COLOR;
            if (cell_color == opponent_color) {

                add_move(depth, coord, new_coord, FIGURE_TYPE_BISHOP, MOVE_TYPE_EAT);
            }
            n++;
        }
        return;
    }

    if (type == FIGURE_TYPE_KNIGHT) {

        n = 0;
        while (KnightMove[n] != 0) {

            int new_coord = coord + KnightMove[n];
            cell = position[new_coord];

            if (cell == CELL_EMPTY) {

                add_move(depth, coord, new_coord, FIGURE_TYPE_KNIGHT, MOVE_TYPE_SIMPLY);
            }
            else {

                cell = position[new_coord];
                cell_color = cell & MASK_COLOR;

                if (cell_color == opponent_color) {

                    add_move(depth, coord, new_coord, FIGURE_TYPE_KNIGHT, MOVE_TYPE_EAT);
                }
            }

            n++;
        }
        return;
    }

    if (type == FIGURE_TYPE_PAWN) {

        is_moved = cell & MASK_MOVE;

        if (color == WHITE) { // if pawn is white

            // move pawn on 1 cell ahead
            if (position[coord - 16] == CELL_EMPTY) {

                if (coord > 80 && coord < 96) { // if the current line 7

                    add_move(depth, coord, coord - 16, FIGURE_TYPE_QUEEN, MOVE_TYPE_CONVERSION);
                    add_move(depth, coord, coord - 16, FIGURE_TYPE_ROOK, MOVE_TYPE_CONVERSION);
                    add_move(depth, coord, coord - 16, FIGURE_TYPE_BISHOP, MOVE_TYPE_CONVERSION);
                    add_move(depth, coord, coord - 16, FIGURE_TYPE_KNIGHT, MOVE_TYPE_CONVERSION);
                }
                else {

                    add_move(depth, coord, coord - 16, FIGURE_TYPE_PAWN, MOVE_TYPE_SIMPLY);
                    if (is_moved != IS_MOVE) { // if pawn did't move

                        if (position[coord - 32] == CELL_EMPTY) {

                            add_move(depth, coord, coord - 32, FIGURE_TYPE_PAWN, MOVE_TYPE_SIMPLY);
                        }
                    }
                }
            }

            // the left of the pawn
            cell = position[coord - 1 - 16];
            cell_color = cell & MASK_COLOR;

            if (cell_color == BLACK) {
                if (coord > 80 && coord < 96) { // if the current line 7

                    add_move(depth, coord, coord - 1 - 16, FIGURE_TYPE_QUEEN, MOVE_TYPE_CONVERSION);
                    add_move(depth, coord, coord - 1 - 16, FIGURE_TYPE_ROOK, MOVE_TYPE_CONVERSION);
                    add_move(depth, coord, coord - 1 - 16, FIGURE_TYPE_BISHOP, MOVE_TYPE_CONVERSION);
                    add_move(depth, coord, coord - 1 - 16, FIGURE_TYPE_KNIGHT, MOVE_TYPE_CONVERSION);
                }
                else {

                    add_move(depth, coord, coord - 1 - 16, FIGURE_TYPE_PAWN, MOVE_TYPE_EAT);
                }
            }

            // the right of the pawn
            cell = position[coord + 1 - 16];
            cell_color = cell & MASK_COLOR;
            if (cell_color == BLACK) {
                if (coord > 80 && coord < 96) { // if the current line 7

                    add_move(depth, coord, coord + 1 - 16, FIGURE_TYPE_QUEEN, MOVE_TYPE_CONVERSION);
                    add_move(depth, coord, coord + 1 - 16, FIGURE_TYPE_ROOK, MOVE_TYPE_CONVERSION);
                    add_move(depth, coord, coord + 1 - 16, FIGURE_TYPE_BISHOP, MOVE_TYPE_CONVERSION);
                    add_move(depth, coord, coord + 1 - 16, FIGURE_TYPE_KNIGHT, MOVE_TYPE_CONVERSION);
                }
                else {

                    add_move(depth, coord, coord + 1 - 16, FIGURE_TYPE_PAWN, MOVE_TYPE_EAT);
                }
            }

            // take a pawwed pawn
            if (coord > 112 && coord < 128) {

                cell = position[coord + 1];
                cell_color = cell & MASK_COLOR;
                is_passed = cell & MASK_PASSED_PAWN_UCI;
                if (cell_color == BLACK && is_passed == IS_PASSED_PAWN_UCI) {

                    add_move(depth, coord, coord + 1, FIGURE_TYPE_PAWN, MOVE_TYPE_PASSED_PAWN_BLACK);
                }

                cell = position[coord - 1];
                cell_color = cell & MASK_COLOR;
                is_passed = cell & MASK_PASSED_PAWN_UCI;
                if (cell_color == BLACK && is_passed == IS_PASSED_PAWN_UCI) {

                    add_move(depth, coord, coord - 1, FIGURE_TYPE_PAWN, MOVE_TYPE_PASSED_PAWN_BLACK);
                }
            }

        }
        else {

            if (position[coord + 16] == CELL_EMPTY) {

                if (coord > 160 && coord < 176) // if the current line 2
                {
                    add_move(depth, coord, coord + 16, FIGURE_TYPE_QUEEN, MOVE_TYPE_CONVERSION);
                    add_move(depth, coord, coord + 16, FIGURE_TYPE_ROOK, MOVE_TYPE_CONVERSION);
                    add_move(depth, coord, coord + 16, FIGURE_TYPE_BISHOP, MOVE_TYPE_CONVERSION);
                    add_move(depth, coord, coord + 16, FIGURE_TYPE_KNIGHT, MOVE_TYPE_CONVERSION);
                }
                else {

                    add_move(depth, coord, coord + 16, FIGURE_TYPE_PAWN, MOVE_TYPE_SIMPLY);

                    if (is_moved != IS_MOVE) {
                        if (position[coord + 32] == CELL_EMPTY) {
                            add_move(depth, coord, coord + 32, FIGURE_TYPE_PAWN, MOVE_TYPE_SIMPLY);
                        }
                    }
                }
            }

            cell = position[coord - 1 + 16];
            cell_color = cell & MASK_COLOR;
            if (cell_color == WHITE) {

                if (coord > 160 && coord < 176) { // if the current line 2

                    add_move(depth, coord, coord - 1 + 16, FIGURE_TYPE_QUEEN, MOVE_TYPE_CONVERSION);
                    add_move(depth, coord, coord - 1 + 16, FIGURE_TYPE_ROOK, MOVE_TYPE_CONVERSION);
                    add_move(depth, coord, coord - 1 + 16, FIGURE_TYPE_BISHOP, MOVE_TYPE_CONVERSION);
                    add_move(depth, coord, coord - 1 + 16, FIGURE_TYPE_KNIGHT, MOVE_TYPE_CONVERSION);
                }
                else {
                    add_move(depth, coord, coord - 1 + 16, FIGURE_TYPE_PAWN, MOVE_TYPE_EAT);
                }
            }

            cell = position[coord + 1 + 16];
            cell_color = cell & MASK_COLOR;
            if (cell_color == WHITE) {

                if (coord > 160 && coord < 176) // if the current line 2
                {
                    add_move(depth, coord, coord + 1 + 16, FIGURE_TYPE_QUEEN, MOVE_TYPE_CONVERSION);
                    add_move(depth, coord, coord + 1 + 16, FIGURE_TYPE_ROOK, MOVE_TYPE_CONVERSION);
                    add_move(depth, coord, coord + 1 + 16, FIGURE_TYPE_BISHOP, MOVE_TYPE_CONVERSION);
                    add_move(depth, coord, coord + 1 + 16, FIGURE_TYPE_KNIGHT, MOVE_TYPE_CONVERSION);
                }
                else {

                    add_move(depth, coord, coord + 1 + 16, FIGURE_TYPE_PAWN, MOVE_TYPE_EAT);
                }
            }

            if (coord > 128 && coord < 144) {

                cell = position[coord + 1];
                cell_color = cell & MASK_COLOR;
                is_passed = cell & MASK_PASSED_PAWN_UCI;
                if (cell_color == WHITE && is_passed == IS_PASSED_PAWN_UCI) {
                    add_move(depth, coord, coord + 1, FIGURE_TYPE_PAWN, MOVE_TYPE_PASSED_PAWN_WHITE);
                }

                cell = position[coord - 1];
                cell_color = cell & MASK_COLOR;
                is_passed = cell & MASK_PASSED_PAWN_UCI;
                if (cell_color == WHITE && is_passed == IS_PASSED_PAWN_UCI) {

                    add_move(depth, coord, coord - 1, FIGURE_TYPE_PAWN, MOVE_TYPE_PASSED_PAWN_WHITE);

                }
            }
        }
    }

}

// if king isset, not using now
int king_isset(int color) {

    int cell, type, cell_color;

    for (int i = 0; i < 8; i++) {
        for (int j = 68 + i * 16; j < 76 + i * 16; j++) {
            if (position[j] != 0) {

                cell = position[j];
                cell_color = cell & MASK_COLOR;
                type = cell & MASK_TYPE;

                if (cell_color == color && type == FIGURE_TYPE_KING) {
                    return 1;
                }
            }
        }
    }

    return 0;
}

// add move to array
void add_move(int depth, int current_coord, int new_coord, int figure_type, MOVE_TYPE type) {

    int cell = position[current_coord];
    int color = cell & MASK_COLOR;
    int cell_type = cell & MASK_TYPE;

    if (!is_legal_move(current_coord, new_coord)) {

        return;
    }

    moves[depth][count_move[depth]].current_position = current_coord;
    moves[depth][count_move[depth]].next_position = new_coord;

    moves[depth][count_move[depth]].MoveType = type;

    if (type == MOVE_TYPE_CONVERSION) {

        moves[depth][count_move[depth]].NewFigureType = figure_type;
    }

    count_move[depth]++;
}

// MVV-LVA move sorting
void sort_move(int depth) {

    int max = count_move[depth];

    int cell, type;
    int cell2, type2;
    // very bad sort
    for (int i = 0; i < max - 1; i++) {

        for (int j = 0; j < max - i - 1; j++) {

            if (moves[depth][j].MoveType < moves[depth][j + 1].MoveType) {

                MOVE tmp = moves[depth][j];
                moves[depth][j] = moves[depth][j + 1];
                moves[depth][j + 1] = tmp;
            }
        }
    }

    int start;
    for (start = 0; moves[depth][start].MoveType != MOVE_TYPE_EAT && start < max; start++);

    int stop, test;
    for (stop = start, test = start; moves[depth][test].MoveType == MOVE_TYPE_EAT; stop++, test++);

    for (int i = start; i < stop - 1; i++) {
        for (int j = start; j < stop - 1; j++) {

            if (moves[depth][j].MoveType == MOVE_TYPE_EAT && moves[depth][j + 1].MoveType == MOVE_TYPE_EAT) {

                cell = position[moves[depth][j].next_position];
                type = cell & MASK_TYPE;

                cell2 = position[moves[depth][j + 1].next_position];
                type2 = cell2 & MASK_TYPE;

                if (type > type2) {

                    MOVE tmp = moves[depth][j];
                    moves[depth][j] = moves[depth][j + 1];
                    moves[depth][j + 1] = tmp;
                }

            }

        }
    }

    for (int i = start; i < stop - 1; i++) {
        for (int j = start; j < stop - 1; j++) {


            cell = position[moves[depth][j].next_position];
            type = cell & MASK_TYPE;

            cell2 = position[moves[depth][j + 1].next_position];
            type2 = cell2 & MASK_TYPE;

            if (cell == cell2) {

                cell = position[moves[depth][j].current_position];
                type = cell & MASK_TYPE;

                cell2 = position[moves[depth][j + 1].current_position];
                type2 = cell2 & MASK_TYPE;

                if (type < type2) {

                    MOVE tmp = moves[depth][j];
                    moves[depth][j] = moves[depth][j + 1];
                    moves[depth][j + 1] = tmp;
                }

            }

        }
    }

    HASH_TABLE *hash_ptr;
    hash_ptr = &hash_table[current_hash % (MAX_HASH_TABLE_SIZE)];

    if(hash_ptr->level <= depth && hash_ptr->key == current_hash) {

        int count = 0;
        for (count = 0; moves[depth][count].MoveType != MOVE_TYPE_EMPTY; count++);

        for (int i = count; i > 0; i--) {

            moves[depth][count] = moves[depth][count - 1];
        }
        memcpy(&moves[depth][0], &hash_ptr->move, sizeof(MOVE));
    }
}

// the move is made according to the rules, there are no checks and ligaments
int is_legal_move(int coord1, int coord2) {

    int cell = position[coord1];
    int cell2 = position[coord2];

    int color = cell & MASK_COLOR;
    int opponent_color = color ^MASK_COLOR;

    position[coord1] = 0;
    position[coord2] = cell;


    if (king_is_checked(color)) {

        position[coord1] = cell;
        position[coord2] = cell2;

        return 0;
    }
    else {

        position[coord1] = cell;
        position[coord2] = cell2;

        return 1;
    }
}

// king is checked?
int king_is_checked(int color) {

    int cell, cell_color;
    color = color ^ MASK_COLOR; // opponent color

    // scan board
    for (int i = 0; i < 8; i++) {
        for (int j = 68 + i * 16; j < 76 + i * 16; j++) {
            if (position[j] != 0) {
                cell = position[j];
                cell_color = cell & MASK_COLOR;
                if (cell_color == color) {
                    if (check_king(j)) {

                        return 1;
                    }
                }
            }
        }
    }
    return 0;
}

// realisation of check test
int check_king(int coord) {

    int n = 0;
    int cell = position[coord];

    int color = cell & MASK_COLOR;
    int opponent_color = color ^MASK_COLOR;
    int type = cell & MASK_TYPE;

    int check_type, cell_color;

    if (type == FIGURE_TYPE_KING) {

        n = 0;
        while (KingMove[n] != 0) {

            int new_coord = coord + KingMove[n];
            cell = position[new_coord];

            if (cell != CELL_EMPTY) {

                check_type = cell & MASK_TYPE;
                if (check_type == FIGURE_TYPE_KING) {

                    return 1;
                }
            }
            n++;
        }

        return 0;
    }

    if (type == FIGURE_TYPE_QUEEN) {

        n = 0;
        while (QueenMove[n] != 0) {

            int new_coord = coord + QueenMove[n];

            while (position[new_coord] == CELL_EMPTY) {

                new_coord += QueenMove[n];
            }

            cell = position[new_coord];
            check_type = cell & MASK_TYPE;
            cell_color = cell & MASK_COLOR;

            if (check_type == FIGURE_TYPE_KING && cell_color == opponent_color) {

                return 1;
            }

            n++;
        }

        return 0;
    }

    if (type == FIGURE_TYPE_ROOK) {

        n = 0;
        while (RookMove[n] != 0) {

            int new_coord = coord + RookMove[n];

            while (position[new_coord] == CELL_EMPTY) {

                new_coord += RookMove[n];
            }

            cell = position[new_coord];
            check_type = cell & (MASK_TYPE);
            cell_color = cell & MASK_COLOR;

            if (check_type == FIGURE_TYPE_KING && cell_color == opponent_color) {

                return 1;
            }

            n++;
        }

        return 0;
    }

    if (type == FIGURE_TYPE_BISHOP) {

        n = 0;
        while (BishopMove[n] != 0) {

            int new_coord = coord + BishopMove[n];
            while (position[new_coord] == CELL_EMPTY) {

                new_coord += BishopMove[n];
            }

            cell = position[new_coord];
            check_type = cell & (MASK_TYPE);
            cell_color = cell & MASK_COLOR;

            if (check_type == FIGURE_TYPE_KING && cell_color == opponent_color) {

                return 1;
            }

            n++;
        }

        return 0;
    }

    if (type == FIGURE_TYPE_KNIGHT) {

        n = 0;
        while (KnightMove[n] != 0) {

            int new_coord = coord + KnightMove[n];
            cell = position[new_coord];
            check_type = cell & (MASK_TYPE);
            cell_color = cell & MASK_COLOR;

            if (check_type == FIGURE_TYPE_KING && cell_color == opponent_color) {

                return 1;
            }

            n++;
        }

        return 0;
    }

    if (type == FIGURE_TYPE_PAWN) {

        if (color == BLACK) {

            cell = position[coord - 1 + 16];
            check_type = cell & (MASK_TYPE);
            cell_color = cell & MASK_COLOR;

            if (check_type == FIGURE_TYPE_KING && cell_color == opponent_color) {

                return 1;
            }

            cell = position[coord + 1 + 16];
            check_type = cell & (MASK_TYPE);
            cell_color = cell & MASK_COLOR;

            if (check_type == FIGURE_TYPE_KING && cell_color == opponent_color) {

                return 1;
            }
        }
        else {

            cell = position[coord - 1 - 16];
            check_type = cell & (MASK_TYPE);
            cell_color = cell & MASK_COLOR;

            if (check_type == FIGURE_TYPE_KING && cell_color == opponent_color) {

                return 1;
            }

            cell = position[coord + 1 - 16];
            check_type = cell & (MASK_TYPE);
            cell_color = cell & MASK_COLOR;

            if (check_type == FIGURE_TYPE_KING && cell_color == opponent_color) {

                return 1;
            }
        }

        return 0;
    }

    return 0;
}

// make move, change positions and zobrist
void make_move(MOVE move, int depth) {

    memcpy(old_position[depth], position, 256 * sizeof(int)); // save position
    old_hash[depth] = current_hash;

    current_hash ^= zobrist_key_move; // update zobrist

    int cell = position[move.current_position];
    int cell_type = cell & MASK_TYPE;
    int cell_color = cell & MASK_COLOR;

    int new_cell = position[move.next_position];
    int new_cell_type = new_cell & MASK_TYPE;
    int new_cell_color = new_cell & MASK_COLOR;

    int zobrist_cell_color = ZOBRIST_WHITE;
    if (cell_color == BLACK) {

        zobrist_cell_color = ZOBRIST_BLACK;
    }

    position[move.current_position] = cell | IS_MOVE;

    if (cell_type == FIGURE_TYPE_PAWN) {

        cell = cell ^ MASK_PASSED_PAWN_UCI; // if pawn is passed, unset flag

        // if a move is made on 2 cells, pawn maybe passed
        if (move.current_position - move.next_position == 32 || move.current_position - move.next_position == -32) {

            cell = cell | MASK_PASSED_PAWN_UCI;
        }
    }

    if (move.MoveType == MOVE_TYPE_SIMPLY || move.MoveType == MOVE_TYPE_EAT) {

        if (move.MoveType == MOVE_TYPE_EAT) {

            current_hash ^= zobrist_key[new_cell_type][!zobrist_cell_color][move.next_position]; // remove opponent piece
        }

        current_hash ^= zobrist_key[cell_type][zobrist_cell_color][move.current_position];
        current_hash ^= zobrist_key[cell_type][zobrist_cell_color][move.next_position];

        position[move.current_position] = 0;
        position[move.next_position] = cell;
    }

    // take black passed pawn
    if (move.MoveType == MOVE_TYPE_PASSED_PAWN_BLACK) {

        current_hash ^= zobrist_key[cell_type][zobrist_cell_color][move.current_position]; // remove 1 pawn
        current_hash ^= zobrist_key[cell_type][!zobrist_cell_color][move.next_position]; // remove 2 pawn
        current_hash ^= zobrist_key[cell_type][zobrist_cell_color][move.next_position - 16]; // set 1 pawn

        position[move.current_position] = 0;
        position[move.next_position] = 0;
        position[move.next_position - 16] = cell;
    }

    // take white passed pawn
    if (move.MoveType == MOVE_TYPE_PASSED_PAWN_WHITE) {

        current_hash ^= zobrist_key[cell_type][zobrist_cell_color][move.current_position]; // remove 1 pawn
        current_hash ^= zobrist_key[cell_type][!zobrist_cell_color][move.next_position]; // remove 2 pawn
        current_hash ^= zobrist_key[cell_type][zobrist_cell_color][move.next_position + 16]; // set 1 pawn

        position[move.current_position] = 0;
        position[move.next_position] = 0;
        position[move.next_position + 16] = cell;
    }

    // if pawn conversion, Ailey is alive?
    if (move.MoveType == MOVE_TYPE_CONVERSION) {

        if (new_cell_type != CELL_EMPTY) {

            current_hash ^= zobrist_key[new_cell_type][!zobrist_cell_color][move.next_position];
        }

        current_hash ^= zobrist_key[cell_type][zobrist_cell_color][move.current_position];
        current_hash ^= zobrist_key[move.NewFigureType][zobrist_cell_color][move.next_position];

        position[move.current_position] = 0;
        position[move.next_position] = move.NewFigureType | cell_color | IS_MOVE;
    }

    if (move.MoveType == MOVE_TYPE_CASTLING_O_O) {

        int king_cell = position[move.current_position];
        int rook_cell = position[move.current_position + 3];

        current_hash ^= zobrist_key[FIGURE_TYPE_KING][zobrist_cell_color][move.current_position]; // remove king
        current_hash ^= zobrist_key[FIGURE_TYPE_ROOK][zobrist_cell_color][move.current_position + 3]; // remove rook

        current_hash ^= zobrist_key[FIGURE_TYPE_KING][zobrist_cell_color][move.current_position + 2]; // set king
        current_hash ^= zobrist_key[FIGURE_TYPE_ROOK][zobrist_cell_color][move.current_position + 1]; // set rook

        position[move.current_position] = 0;
        position[move.current_position + 3] = 0;
        position[move.current_position + 2] = king_cell;
        position[move.current_position + 1] = rook_cell | IS_MOVE;
    }

    if (move.MoveType == MOVE_TYPE_CASTLING_O_O_0) {

        int king_cell = position[move.current_position];
        int rook_cell = position[move.current_position - 4];

        current_hash ^= zobrist_key[FIGURE_TYPE_KING][zobrist_cell_color][move.current_position]; // remove king
        current_hash ^= zobrist_key[FIGURE_TYPE_ROOK][zobrist_cell_color][move.current_position - 4]; // remove rook

        current_hash ^= zobrist_key[FIGURE_TYPE_KING][zobrist_cell_color][move.current_position - 2]; // set king
        current_hash ^= zobrist_key[FIGURE_TYPE_ROOK][zobrist_cell_color][move.current_position - 1]; // set rook

        position[move.current_position] = 0;
        position[move.current_position - 4] = 0;
        position[move.current_position - 2] = king_cell;
        position[move.current_position - 1] = rook_cell | IS_MOVE;
    }
}

// roll back move
void rollback_move(MOVE move, int depth) {

    current_hash = old_hash[depth]; // load zobrist hash
    memcpy(position, &old_position[depth], 256 * sizeof(int)); // load position
}

int get_max_count_move(int current_player) {

    int count = 0;
    int cell, color, cell_color;

    if (current_player) {

        color = WHITE;
    }
    else {

        color = BLACK;
    }

    // scan board
    for (int i = 0; i < 8; i++) {
        for (int j = 68 + i * 16; j < 76 + i * 16; j++) {
            if (position[j] != 0) {

                cell = position[j];
                cell_color = cell & MASK_COLOR;

                if (cell_color == color) {

                    count += get_count_moves(j);
                }
            }
        }
    }

    return count;
}

int get_count_moves(int coord) {

    int count = 0;

    int n = 0;
    int cell = position[coord];

    int color = cell & MASK_COLOR;
    int opponent_color = color ^MASK_COLOR;
    int type = cell & MASK_TYPE;

    int cell_color, is_moved, is_passed;

    if (type == FIGURE_TYPE_KING) {

        n = 0;
        while (KingMove[n] != 0) {

            int new_coord = coord + KingMove[n];
            cell = position[new_coord];
            cell_color = cell & MASK_COLOR;

            if (cell == CELL_EMPTY) {

                count++;
            }
            else if (cell_color == opponent_color) {

                count++;
            }

            n++;
        }

        cell = position[coord];
        is_moved = cell & MASK_MOVE;
        if (is_moved != IS_MOVE) {

            if (position[coord + 1] == CELL_EMPTY && position[coord + 2] == CELL_EMPTY) {
                int is_rook = position[coord + 3];
                int type_is_rook = is_rook & MASK_TYPE;
                int rook_moved = is_rook & MASK_MOVE;

                if (type_is_rook == FIGURE_TYPE_ROOK && rook_moved != IS_MOVE) {

                    if (is_legal_move(coord, coord + 1) && is_legal_move(coord, coord + 2)) {

                        count++;
                    }
                }
            }

            if (position[coord - 1] == CELL_EMPTY && position[coord - 2] == CELL_EMPTY &&
                position[coord - 3] == CELL_EMPTY) {

                int is_rook = position[coord - 4];
                int type_is_rook = is_rook & MASK_TYPE;
                int rook_moved = is_rook & MASK_MOVE;

                if (type_is_rook == FIGURE_TYPE_ROOK && rook_moved != IS_MOVE) {

                    if (is_legal_move(coord, coord - 1) && is_legal_move(coord, coord - 2)) {

                        count++;
                    }
                }
            }
        }
    }

    if (type == FIGURE_TYPE_QUEEN) {

        n = 0;
        while (QueenMove[n] != 0) {

            int new_coord = coord + QueenMove[n];
            while (position[new_coord] == CELL_EMPTY) {

                count++;
                new_coord += QueenMove[n];
            }

            cell = position[new_coord];
            cell_color = cell & MASK_COLOR;
            if (cell_color == opponent_color) {

                count++;
            }
            n++;
        }
    }

    if (type == FIGURE_TYPE_ROOK) {

        n = 0;
        while (RookMove[n] != 0) {

            int new_coord = coord + RookMove[n];
            while (position[new_coord] == CELL_EMPTY) {

                count++;
                new_coord += RookMove[n];
            }

            cell = position[new_coord];
            cell_color = cell & MASK_COLOR;
            if (cell_color == opponent_color) {

                count++;
            }
            n++;
        }
    }

    if (type == FIGURE_TYPE_BISHOP) {

        n = 0;
        while (BishopMove[n] != 0) {

            int new_coord = coord + BishopMove[n];
            while (position[new_coord] == CELL_EMPTY) {

                count++;
                new_coord += BishopMove[n];
            }

            cell = position[new_coord];
            cell_color = cell & MASK_COLOR;
            if (cell_color == opponent_color) {

                count++;
            }
            n++;
        }
    }

    if (type == FIGURE_TYPE_KNIGHT) {

        n = 0;
        while (KnightMove[n] != 0) {

            int new_coord = coord + KnightMove[n];
            cell = position[new_coord];

            if (cell == CELL_EMPTY) {

                count++;
            }
            else {

                cell = position[new_coord];
                cell_color = cell & MASK_COLOR;

                if (cell_color == opponent_color) {

                    count++;
                }
            }

            n++;
        }
    }

    if (type == FIGURE_TYPE_PAWN) {

        is_moved = cell & MASK_MOVE;

        if (color == WHITE) {

            if (position[coord - 16] == CELL_EMPTY) {

                if (coord > 80 && coord < 96) {

                    count += 4;
                }
                else {

                    count++;
                    if (is_moved != IS_MOVE) {

                        if (position[coord - 32] == CELL_EMPTY) {

                            count++;
                        }
                    }
                }
            }

            cell = position[coord - 1 - 16];
            cell_color = cell & MASK_COLOR;

            if (cell_color == BLACK) {
                if (coord > 80 && coord < 96) {

                    count += 4;
                }
                else {

                    count++;
                }
            }

            cell = position[coord + 1 - 16];
            cell_color = cell & MASK_COLOR;
            if (cell_color == BLACK) {
                if (coord > 80 && coord < 96) {

                    count += 4;
                }
                else {

                    count++;
                }
            }

            if (coord > 112 && coord < 128) {

                cell = position[coord + 1];
                cell_color = cell & MASK_COLOR;
                is_passed = cell & MASK_PASSED_PAWN_UCI;
                if (cell_color == BLACK && is_passed == IS_PASSED_PAWN_UCI) {

                    count++;
                }

                cell = position[coord - 1];
                cell_color = cell & MASK_COLOR;
                is_passed = cell & MASK_PASSED_PAWN_UCI;
                if (cell_color == BLACK && is_passed == IS_PASSED_PAWN_UCI) {

                    count++;
                }
            }

        }
        else {

            if (position[coord + 16] == CELL_EMPTY) {

                if (coord > 160 && coord < 176) {
                    count += 4;
                }
                else {

                    count++;

                    if (is_moved != IS_MOVE) {
                        if (position[coord + 32] == CELL_EMPTY) {
                            count++;
                        }
                    }
                }
            }

            cell = position[coord - 1 + 16];
            cell_color = cell & MASK_COLOR;
            if (cell_color == WHITE) {

                if (coord > 160 && coord < 176) {

                    count += 4;
                }
                else {
                    count++;
                }
            }

            cell = position[coord + 1 + 16];
            cell_color = cell & MASK_COLOR;
            if (cell_color == WHITE) {

                if (coord > 160 && coord < 176) {
                    count += 4;
                }
                else {

                    count++;
                }
            }

            if (coord > 128 && coord < 144) {

                cell = position[coord + 1];
                cell_color = cell & MASK_COLOR;
                is_passed = cell & MASK_PASSED_PAWN_UCI;
                if (cell_color == WHITE && is_passed == IS_PASSED_PAWN_UCI) {
                    count++;
                }

                cell = position[coord - 1];
                cell_color = cell & MASK_COLOR;
                is_passed = cell & MASK_PASSED_PAWN_UCI;
                if (cell_color == WHITE && is_passed == IS_PASSED_PAWN_UCI) {

                    count++;

                }
            }
        }
    }

    return count;
}