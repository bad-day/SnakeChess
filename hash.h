//
// Created by valera on 28.11.17.
//

#ifndef CHESS_HASH_H
#define CHESS_HASH_H

#include <stdio.h>

#include "board.h"
#include "move.h"

#define MAX_HASH_TABLE_SIZE (1<<20)
//типы отсечений

#define ZOBRIST_BLACK 0
#define ZOBRIST_WHITE 1


//поле пустое
#define HASH_TABLE_TYPE_EMPTY 0
//оценка точная
#define HASH_TABLE_TYPE_EXACT 1
//отсечение по альфа
#define HASH_TABLE_TYPE_ALPHA 2
//отсечение по бета
#define HASH_TABLE_TYPE_BETA  3
//поле устарело, но содержит ход
#define HASH_TABLE_TYPE_OLD   4

typedef struct HASH_TABLE HASH_TABLE;
//описатель таблицы
struct HASH_TABLE
{
    unsigned long key;//хэш-ключ
    int deep;//глубина узла
    int type;//тип узла
    int score;//оценка узла
    MOVE move;//лучший ход (если он есть)
};

void hash_init();

unsigned long get_hash();

void hash_to_table(unsigned long hash_key, int score, int depth, int color);

#endif //CHESS_HASH_H
