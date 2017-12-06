#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "board.h"
#include "move.h"
#include "hash.h"

extern Board position; //main.c

HASH_TABLE hash_table[MAX_HASH_TABLE_SIZE];
HASH_MOVE_TABLE hash_move_table[MAX_HASH_MOVE_TABLE_SIZE];
HASH_BEST_MOVE_TABLE hash_best_move_table[MAX_HASH_MOVE_TABLE_SIZE];

unsigned long current_hash;

//hash-table [count of type piece][count of piece][position on 16x16]
unsigned long zobrist_key[7][2][256];

unsigned long zobrist_key_move; // key for change move
unsigned long zobrist_key_null_move; // key for null move

// unsigned long random
unsigned long randomize_hash() {

    return ((unsigned long) rand() ^ ((unsigned long) rand() << 15) ^ ((unsigned long) rand() << 31) ^
            ((unsigned long) rand() << 47) ^ ((unsigned long) rand() << 59));
}

// initialize zobrist hash
void hash_init() {

    current_hash = get_hash();

    zobrist_key_move = randomize_hash();
    zobrist_key_null_move = randomize_hash();

    for (int i = 0; i < 7; i++) {
        for (int j = 0; j < 2; j++) {
            for (int k = 0; k < 256; k++) {

                zobrist_key[i][j][k] = randomize_hash();
            }
        }
    }

    for (int i = 0; i < MAX_HASH_TABLE_SIZE; i++) {

        hash_table[i].type = HASH_TABLE_TYPE_EMPTY;
        hash_table[i].level = -1;
    }

    for (int i = 0; i < MAX_HASH_MOVE_TABLE_SIZE; i++) {

        hash_move_table[i].move.MoveType = MOVE_TYPE_EMPTY;
        hash_move_table[i].level = -1;

        hash_best_move_table[i].move.MoveType = MOVE_TYPE_EMPTY;
        hash_best_move_table[i].level = -1;
    }
}

// get hash for current position
unsigned long get_hash() {

    unsigned long key = 0;
    int cell, type, color;

    for (int i = 0; i < 8; i++) {
        for (int j = 68 + i * 16; j < 76 + i * 16; j++) {
            if (position[j] != 0) {

                cell = position[j];
                color = cell & MASK_COLOR;
                type = cell & MASK_TYPE;

                if (color == WHITE)
                    key ^= zobrist_key[type][ZOBRIST_WHITE][j];

                if (color == BLACK)
                    key ^= zobrist_key[type][ZOBRIST_BLACK][j];
            }
        }
    }

    return key;
}

// record information in a table by zobrist key
void hash_to_table(unsigned long hash_key, int score, int level, int type) {

    // number in the table by the remainder of the division
    HASH_TABLE *ptr = &hash_table[hash_key % (MAX_HASH_TABLE_SIZE)];

    if (ptr->level <= level) {

        ptr->type = type;
        ptr->level = level;
        ptr->key = hash_key;
        ptr->score = score;
    }
}

// record move information in a table by zobrist key
void move_to_table(unsigned long hash_key, int level, MOVE move) {

    // number in the table by the remainder of the division
    HASH_MOVE_TABLE *ptr = &hash_move_table[hash_key % (MAX_HASH_MOVE_TABLE_SIZE)];

    if (ptr->level <= level) {

        ptr->key = hash_key;
        ptr->level = level;
        memcpy(&ptr->move, &move, sizeof(MOVE));
    }
}

// record best move for out
void best_move_to_table(unsigned long hash_key, int level, MOVE move) {

    // number in the table by the remainder of the division
    HASH_BEST_MOVE_TABLE *ptr = &hash_best_move_table[hash_key % (MAX_HASH_MOVE_TABLE_SIZE)];

    if (ptr->level <= level) {

        ptr->key = hash_key;
        ptr->level = level;
        memcpy(&ptr->move, &move, sizeof(MOVE));
    }
}