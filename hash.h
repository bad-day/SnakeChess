//
// Created by valera on 28.11.17.
//

#ifndef CHESS_HASH_H
#define CHESS_HASH_H

#include <stdio.h>
#include <stdlib.h>

#include "board.h"
#include "move.h"

#define MAX_HASH_TABLE_SIZE (1<<21)

#define ZOBRIST_BLACK 0
#define ZOBRIST_WHITE 1

#define HASH_TABLE_TYPE_EMPTY 0
#define HASH_TABLE_TYPE_EXACT 1
#define HASH_TABLE_TYPE_ALPHA 2
#define HASH_TABLE_TYPE_BETA  3

typedef struct HASH_TABLE HASH_TABLE;

struct HASH_TABLE
{
    unsigned long key;
    int deep;
    int type;
    int score;
    MOVE move;
};

unsigned long randomize_hash();

void hash_init();

unsigned long get_hash();

void hash_to_table(unsigned long hash_key, int score, int depth, int type);

void move_to_table(unsigned long hash_key, int depth, MOVE move);

#endif //CHESS_HASH_H
