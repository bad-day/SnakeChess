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
    MOVE_TYPE_EAT = 1,
    MOVE_TYPE_CASTLING_O_O = 2,
    MOVE_TYPE_CASTLING_O_O_0 = 3,
    MOVE_TYPE_PASSED_PAWN_WHITE = 4,
    MOVE_TYPE_PASSED_PAWN_BLACK = 5,
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

void generate_moves(int depth, int current_player);

void get_moves(int coord, int depth);

void add_move(int depth, int current_coord, int new_coord, int figure_type, MOVE_TYPE type);

void make_move(MOVE move, int depth);

void rollback_move(MOVE move, int depth);

int check_king(int coord);

int king_is_checked(int color);

int is_legal_move(int coord1, int coord2);

void print_all_tree(int deep);

void move_init();

#endif //CHESS_MOVE_H
