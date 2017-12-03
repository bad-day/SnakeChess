#ifndef CHESS_MOVE_H
#define CHESS_MOVE_H

// max deep
#define DEPTH 100
// infinity value
#define INF 999999

typedef enum MOVE_TYPE MOVE_TYPE;
typedef struct MOVE MOVE;

// move types
enum MOVE_TYPE {

    MOVE_TYPE_EMPTY = -1,
    MOVE_TYPE_SIMPLY = 0,

    MOVE_TYPE_CASTLING_O_O = 1,
    MOVE_TYPE_CASTLING_O_O_0 = 2,
    MOVE_TYPE_PASSED_PAWN_WHITE = 3,
    MOVE_TYPE_PASSED_PAWN_BLACK = 4,

    MOVE_TYPE_EAT = 5,
    MOVE_TYPE_CONVERSION = 6,
};

struct MOVE {

    int current_position;
    int next_position;

    MOVE_TYPE MoveType; // move type
    int NewFigureType; // A new type of figure, if it came out of a pawn
    int is_check; // move is check ?
};

// init move
void move_init();

// generate all move to moves[DEPTH]
void generate_moves(int depth, int current_player);

// get moves for each piece
void get_moves(int coord, int depth);

// add move to array
void add_move(int depth, int current_coord, int new_coord, int figure_type, MOVE_TYPE type);

// MVV-LVA move sorting
void sort_move(int depth);

// make move, change positions and zobrist
void make_move(MOVE move, int depth);

// take back move and zobrist keys
void rollback_move(MOVE move, int depth);

// realisation of check test
int check_king(int coord);

// if king isset, not using now
int king_is_checked(int color);

// the move is made according to the rules, there are no checks and ligaments
int is_legal_move(int coord1, int coord2);

// king is checked?
int king_isset(int color);

// return count of moves for current position
int get_max_count_move(int current_player);

// return count of moves for each figure
int get_count_moves(int coord);

#endif //CHESS_MOVE_H
