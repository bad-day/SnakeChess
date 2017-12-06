#ifndef CHESS_HASH_H
#define CHESS_HASH_H

#define MAX_HASH_TABLE_SIZE (1<<21)
#define MAX_HASH_MOVE_TABLE_SIZE (1<<20)

#define ZOBRIST_BLACK 0
#define ZOBRIST_WHITE 1

#define HASH_TABLE_TYPE_EMPTY 0
#define HASH_TABLE_TYPE_EXACT 1
#define HASH_TABLE_TYPE_ALPHA 2
#define HASH_TABLE_TYPE_BETA  3

typedef struct HASH_TABLE HASH_TABLE;
typedef struct HASH_MOVE_TABLE HASH_MOVE_TABLE;
typedef struct HASH_BEST_MOVE_TABLE HASH_BEST_MOVE_TABLE;

struct HASH_TABLE {
    unsigned long key;
    int level;
    int type;
    int score;
};

struct HASH_MOVE_TABLE {
    unsigned long key;
    int level;
    MOVE move;
};

struct HASH_BEST_MOVE_TABLE {
    unsigned long key;
    int level;
    MOVE move;
};

// initialize zobrist hash
void hash_init();

// unsigned long random
unsigned long randomize_hash();

// get hash for current position
unsigned long get_hash();

// record information in a table by zobrist key
void hash_to_table(unsigned long hash_key, int score, int depth, int type);

// record move information in a table by zobrist key
void move_to_table(unsigned long hash_key, int level, MOVE move);

//
void best_move_to_table(unsigned long hash_key, int level, MOVE move);
#endif //CHESS_HASH_H
