//
// Created by valera on 02.11.17.
//

#ifndef CHESS_MOVE_H
#define CHESS_MOVE_H

#define DEPTH 10

typedef enum MOVE_TYPE MOVE_TYPE;
typedef struct MOVE MOVE;
//Типы ходов
enum MOVE_TYPE
{
    MOVE_TYPE_EMPTY=-1,//хода нет
    MOVE_TYPE_SIMPLY=0,//простой ход
    MOVE_TYPE_EAT=1,//вхятие
    MOVE_TYPE_CASTLING=2,//рокировка
    MOVE_TYPE_WHITE_PASSED_PAWN_EAT=3,//взятие проходной пешки
    MOVE_TYPE_BLACK_PASSED_PAWN_EAT=4,//взятие проходной пешки
    MOVE_TYPE_CONVERSION=5,//превращение пешки
};

struct MOVE
{
    //начальна
    // я позиция
    int current_position;
    //конечная позиция
    int next_position;
    MOVE_TYPE MoveType;//тип хода
    int NewFigureType;//новый тип фигуры, если она получилась из пешки
    int Coord256_PassedPawn;//ход проходной пешкой (если он есть. 0- проходной пешки нет)
    int IsEat;//ход-взятие
    //изменение веса хода (используем для сортировки ходов)
    int Score;
    //указатель на следующий элемент
    MOVE *Move_Next;
};

void generate_moves(int depth, int current_player);
void get_moves(int coord, int depth);
void add_move(int depth, int current_coord, int new_coord, MOVE_TYPE type);
void make_move(MOVE move, int depth);
void rollback_move(MOVE move, int depth);
int check_king(int coord);
int king_is_checked(int color);
int is_legal_move(int coord1, int coord2);
#endif //CHESS_MOVE_H
