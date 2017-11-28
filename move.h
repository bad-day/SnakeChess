//
// Created by valera on 02.11.17.
//

#ifndef CHESS_MOVE_H
#define CHESS_MOVE_H

#define DEPTH 100

typedef enum MOVE_TYPE MOVE_TYPE;
typedef struct MOVE MOVE;
//Типы ходов
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
    MOVE_TYPE MoveType; //тип хода
    int NewFigureType; //новый тип фигуры, если она получилась из пешки
    int Coord256_PassedPawn; //ход проходной пешкой (если он есть. 0- проходной пешки нет)
    int IsEat; //ход-взятие
     //изменение веса хода (используем для сортировки ходов)
    int Score;
    //указатель на следующий элемент
    MOVE *Move_Next;
};

// generete all move to moves[DEPTH]
void generate_moves(int depth, int current_player);


void get_moves(int coord, int depth);

void add_move(int depth, int current_coord, int new_coord, int figure_type, MOVE_TYPE type);

void sort_move(int depth);

// make move
void make_move(MOVE move, int depth);

// roolback move
void rollback_move(MOVE move, int depth);


int check_king(int coord);

int king_is_checked(int color);

int is_legal_move(int coord1, int coord2);


// init moves, can be deleted
void move_init();

int king_isset(int color);

// return count of moves for current position
int get_max_count_move(int current_player);

// return count of moves for each figure
int get_count_moves(int coord);

#endif //CHESS_MOVE_H
