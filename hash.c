//
// Created by valera on 28.11.17.
//

#include "hash.h"

extern Board position; //main.c

HASH_TABLE hash_table[MAX_HASH_TABLE_SIZE];
unsigned long current_hash;

//hash-table [count of type piece][count of piece][position on 16x16]
unsigned long zobrist_key[7][2][256];

unsigned long zobrist_key_move; // key for change move
unsigned long zobrist_key_null_move; // key for null move

unsigned long randomize_hash() {

    return ((unsigned long) rand() ^ ((unsigned long) rand() << 15) ^ ((unsigned long) rand() << 31) ^
            ((unsigned long) rand() << 47) ^ ((unsigned long) rand() << 59));
}

void hash_init() {

    current_hash = get_hash();

    zobrist_key_move = randomize_hash();
    zobrist_key_move = randomize_hash();

    for(int i = 0; i < 7; i++) {
        for(int j = 0; j < 2; j++) {
            for(int k = 0; k < 256; k++) {

                zobrist_key[i][j][k] = randomize_hash();
            }
        }
    }

    for (int i = 0; i < MAX_HASH_TABLE_SIZE; i++) {

        hash_table[i].type = HASH_TABLE_TYPE_EMPTY;
    }
}

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

void hash_to_table(unsigned long hash_key, int score, int depth, int type) {

    // number in the table by the remainder of the division
    HASH_TABLE *ptr = &hash_table[hash_key % (MAX_HASH_TABLE_SIZE)];

    if (ptr->deep <= depth) {

        ptr->type = type;
        ptr->deep = depth;
        ptr->key = hash_key;
        ptr->score = score;
    }
}

void move_to_table(unsigned long hash_key, int depth, MOVE move) {

    HASH_TABLE *ptr = &hash_table[hash_key % (MAX_HASH_TABLE_SIZE)];

    if (ptr->deep <= depth) {

        ptr->move = move;
    }
}