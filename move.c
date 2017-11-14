//
// Created by valera on 02.11.17.
//

#include <stdio.h>
#include <string.h>

#include "move.h"
#include "board.h"

extern Board position; // main.c
MOVE moves[DEPTH][200]; // все ходы фигурой для дерева

// Все возможные ходы фигур
int KingMove[9] = {16, -16, 1, -1, 17, -17, 15, -15, 0};
int QueenMove[9] = {16, -16, 1, -1, 17, -17, 15, -15, 0};
int RookMove[5] = {16, -16, 1, -1, 0};
int BishopMove[5] = {17, -17, 15, -15, 0};
int KnightMove[9] = {32 + 1, 32 - 1, 16 + 2, 16 - 2, -(32 + 1), -(32 - 1), -(16 + 2), -(16 - 2), 0};

// храним позиции для каждой глубины, по сути нужны только для rollback
Board old_position[DEPTH];

// счетчих хранимых ходов на текущей глубине moves[DEPTH]
int current_move[DEPTH];

// инициализируем
void move_init() {

    for (int i = 0; i < DEPTH; i++) {
        for (int j = 0; j < 200; j++) {

            moves[i][j].MoveType = -1;
        }
    }
}

// Получаем все ходы
void generate_moves(int depth, int current_player) {

    current_move[depth] = 0;

    int cell, color, cell_color;

    // какой цвет фигуры ещем
    if (current_player) {

        color = WHITE;
    }
    else {

        color = BLACK;
    }

    // сканируем доску на поисках фигуры
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

// Есть ли король, пока не надо
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

// Получаем ходы для каждой фигуры
void get_moves(int coord, int depth) {

    int n = 0;
    int cell = position[coord];

    int color = cell & MASK_COLOR;
    int opponent_color = color ^MASK_COLOR;
    int type = cell & MASK_TYPE;

    int cell_color, is_moved, is_passed;
    // король
    if (type == FIGURE_TYPE_KING) {

        n = 0;
        while (KingMove[n] != 0) {

            int new_coord = coord + KingMove[n]; // Смотрим клетку
            cell = position[new_coord];
            cell_color = cell & MASK_COLOR;

            if (cell == CELL_EMPTY) {

                add_move(depth, coord, new_coord, FIGURE_TYPE_KING, MOVE_TYPE_SIMPLY);
            }
            else if (cell_color == opponent_color) {  // кушаем

                add_move(depth, coord, new_coord, FIGURE_TYPE_KING, MOVE_TYPE_EAT);
            }

            n++;
        }

        // рокировки
        cell = position[coord];
        is_moved = cell & MASK_MOVE;
        if (is_moved != IS_MOVE) { // если король не ходил

            // короткая рокировка
            if (position[coord + 1] == CELL_EMPTY && position[coord + 2] == CELL_EMPTY) {
                int is_rook = position[coord + 3];
                int type_is_rook = is_rook & MASK_TYPE;
                int rook_moved = is_rook & MASK_MOVE;

                // Если ладья не ходила
                if (type_is_rook == FIGURE_TYPE_ROOK && rook_moved != IS_MOVE) {
                    // Если поля для короля не бьют
                    if (is_legal_move(coord, coord + 1) && is_legal_move(coord, coord + 2)) {

                        add_move(depth, coord, coord + 2, FIGURE_TYPE_KING, MOVE_TYPE_CASTLING_O_O);
                    }
                }
            }

            // длинная рокировка
            if (position[coord - 1] == CELL_EMPTY && position[coord - 2] == CELL_EMPTY &&
                position[coord - 3] == CELL_EMPTY) { // если клетки пустые

                int is_rook = position[coord - 4];
                int type_is_rook = is_rook & MASK_TYPE;
                int rook_moved = is_rook & MASK_MOVE;

                // Если ладья не ходила
                if (type_is_rook == FIGURE_TYPE_ROOK && rook_moved != IS_MOVE) {
                    // Если поля для короля не бьют
                    if (is_legal_move(coord, coord - 1) && is_legal_move(coord, coord - 2)) {

                        add_move(depth, coord, coord - 3, FIGURE_TYPE_KING, MOVE_TYPE_CASTLING_O_O_0);
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

                //position[coord] = cell | IS_MOVE; // ладья ходила
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

        if (color == WHITE) { //если это белая пешка

            //ход пешкой на одну клетку вперёд
            if (position[coord - 16] == CELL_EMPTY) {

                if (coord > 80 && coord < 96) { // если текущий ряд 7

                    add_move(depth, coord, coord - 16, FIGURE_TYPE_QUEEN, MOVE_TYPE_CONVERSION);
                    add_move(depth, coord, coord - 16, FIGURE_TYPE_ROOK, MOVE_TYPE_CONVERSION);
                    add_move(depth, coord, coord - 16, FIGURE_TYPE_BISHOP, MOVE_TYPE_CONVERSION);
                    add_move(depth, coord, coord - 16, FIGURE_TYPE_KNIGHT, MOVE_TYPE_CONVERSION);
                }
                else {

                    add_move(depth, coord, coord - 16, FIGURE_TYPE_PAWN, MOVE_TYPE_SIMPLY);
                    if (is_moved != IS_MOVE) { // если пешка не ходила

                        if (position[coord - 32] == CELL_EMPTY) {

                            add_move(depth, coord, coord - 32, FIGURE_TYPE_PAWN, MOVE_TYPE_SIMPLY); // тут сделай скачок
                        }
                    }
                }
            }

            //проверим, можно ли есть
            cell = position[coord - 1 - 16];
            cell_color = cell & MASK_COLOR;

            if (cell_color == BLACK) {
                if (coord > 80 && coord < 96) { // если текущий ряд 7

                    add_move(depth, coord, coord - 1 - 16, FIGURE_TYPE_QUEEN, MOVE_TYPE_CONVERSION);
                    add_move(depth, coord, coord - 1 - 16, FIGURE_TYPE_ROOK, MOVE_TYPE_CONVERSION);
                    add_move(depth, coord, coord - 1 - 16, FIGURE_TYPE_BISHOP, MOVE_TYPE_CONVERSION);
                    add_move(depth, coord, coord - 1 - 16, FIGURE_TYPE_KNIGHT, MOVE_TYPE_CONVERSION);
                }
                else {

                    add_move(depth, coord, coord - 1 - 16, FIGURE_TYPE_PAWN, MOVE_TYPE_EAT);
                }
            }

            cell = position[coord + 1 - 16];
            cell_color = cell & MASK_COLOR;
            if (cell_color == BLACK) {
                if (coord > 80 && coord < 96) { // если текущий ряд 7

                    add_move(depth, coord, coord + 1 - 16, FIGURE_TYPE_QUEEN, MOVE_TYPE_CONVERSION);
                    add_move(depth, coord, coord + 1 - 16, FIGURE_TYPE_ROOK, MOVE_TYPE_CONVERSION);
                    add_move(depth, coord, coord + 1 - 16, FIGURE_TYPE_BISHOP, MOVE_TYPE_CONVERSION);
                    add_move(depth, coord, coord + 1 - 16, FIGURE_TYPE_KNIGHT, MOVE_TYPE_CONVERSION);
                }
                else {

                    add_move(depth, coord, coord + 1 - 16, FIGURE_TYPE_PAWN, MOVE_TYPE_EAT);
                }
            }

            // взятие проходной пешки
            if (coord > 112 && coord < 128) {

                // проходная пешка справа
                cell = position[coord + 1];
                cell_color = cell & MASK_COLOR;
                is_passed = cell & MASK_PASSED_PAWN_UCI;
                if (cell_color == BLACK && is_passed == IS_PASSED_PAWN_UCI) {

                    add_move(depth, coord, coord + 1, FIGURE_TYPE_PAWN, MOVE_TYPE_PASSED_PAWN_BLACK);
                }

                // проходная пешка слева
                cell = position[coord - 1];
                cell_color = cell & MASK_COLOR;
                is_passed = cell & MASK_PASSED_PAWN_UCI;
                if (cell_color == BLACK && is_passed == IS_PASSED_PAWN_UCI) {

                    add_move(depth, coord, coord - 1, FIGURE_TYPE_PAWN, MOVE_TYPE_PASSED_PAWN_BLACK);
                }
            }

        }
        else { //если это чёрная пешка

            if (position[coord + 16] == CELL_EMPTY) {

                if (coord > 160 && coord < 176) // если текущий ряд 2
                {
                    add_move(depth, coord, coord + 16, FIGURE_TYPE_QUEEN, MOVE_TYPE_CONVERSION);
                    add_move(depth, coord, coord + 16, FIGURE_TYPE_ROOK, MOVE_TYPE_CONVERSION);
                    add_move(depth, coord, coord + 16, FIGURE_TYPE_BISHOP, MOVE_TYPE_CONVERSION);
                    add_move(depth, coord, coord + 16, FIGURE_TYPE_KNIGHT, MOVE_TYPE_CONVERSION);
                }
                else {

                    add_move(depth, coord, coord + 16, FIGURE_TYPE_PAWN, MOVE_TYPE_SIMPLY);

                    if (is_moved != IS_MOVE) { // если пешка не ходила
                        if (position[coord + 32] == CELL_EMPTY) {
                            add_move(depth, coord, coord + 32, FIGURE_TYPE_PAWN, MOVE_TYPE_SIMPLY);
                        }
                    }
                }
            }

            //едим слева от пешки
            cell = position[coord - 1 + 16];
            cell_color = cell & MASK_COLOR;
            if (cell_color == WHITE) {

                if (coord > 160 && coord < 176) { // если текущий ряд 2

                    add_move(depth, coord, coord - 1 + 16, FIGURE_TYPE_QUEEN, MOVE_TYPE_CONVERSION);
                    add_move(depth, coord, coord - 1 + 16, FIGURE_TYPE_ROOK, MOVE_TYPE_CONVERSION);
                    add_move(depth, coord, coord - 1 + 16, FIGURE_TYPE_BISHOP, MOVE_TYPE_CONVERSION);
                    add_move(depth, coord, coord - 1 + 16, FIGURE_TYPE_KNIGHT, MOVE_TYPE_CONVERSION);
                }
                else {
                    add_move(depth, coord, coord - 1 + 16, FIGURE_TYPE_PAWN, MOVE_TYPE_EAT);
                }
            }
            //едим справа от пешки
            cell = position[coord + 1 + 16];
            cell_color = cell & MASK_COLOR;
            if (cell_color == WHITE) {

                if (coord > 160 && coord < 176) // если текущий ряд 2
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

            // взятие проходной пешки
            if (coord > 128 && coord < 144) {

                // проходная пешка справа
                cell = position[coord + 1];
                cell_color = cell & MASK_COLOR;
                is_passed = cell & MASK_PASSED_PAWN_UCI;
                if (cell_color == WHITE && is_passed == IS_PASSED_PAWN_UCI) {
                    add_move(depth, coord, coord + 1, FIGURE_TYPE_PAWN, MOVE_TYPE_PASSED_PAWN_WHITE);
                }
                // проходная пешка слева
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

// добавляем ход в массив
void add_move(int depth, int current_coord, int new_coord, int figure_type, MOVE_TYPE type) {

    int cell = position[current_coord];
    int color = cell & MASK_COLOR;
    int cell_type = cell & MASK_TYPE;


    // было бы неплохой оптимизацией делать это тошлько для короля и при шахах
    // подумай где хранить шах.
    // если ходит король, или не проверили позицию на шах
    // тут могут быть связки..

    if (!is_legal_move(current_coord, new_coord)) {

        return;
    }

    moves[depth][current_move[depth]].current_position = current_coord;
    moves[depth][current_move[depth]].next_position = new_coord;

    moves[depth][current_move[depth]].MoveType = type;

    if (type == MOVE_TYPE_CONVERSION) {

        moves[depth][current_move[depth]].NewFigureType = figure_type;
    }

    current_move[depth]++;
}

// Если ход верный, нет шаха королю. Наверное, можно как-то ставить бит шаха, подумай+
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

// проверяем зашахован ли в текущей пощиции король
int king_is_checked(int color) {

    int cell, cell_color;
    color = color ^ MASK_COLOR; // цвет оппонента


    // сканируем доску на поисках фигуры
    for (int i = 0; i < 8; i++) {
        for (int j = 68 + i * 16; j < 76 + i * 16; j++) {
            if (position[j] != 0) {
                cell = position[j];
                cell_color = cell & MASK_COLOR;
                if (cell_color == color) { // тащим все ходы противника
                    if (check_king(j))
                        return 1;
                }
            }
        }
    }
    return 0;

}

// сама реализцая проверки на шах
int check_king(int coord) {

    int n = 0;
    int cell = position[coord];

    int color = cell & MASK_COLOR;
    int opponent_color = color ^MASK_COLOR;
    int type = cell & MASK_TYPE;

    int check_type, cell_color;

    // Король
    if (type == FIGURE_TYPE_KING) {

        n = 0;
        while (KingMove[n] != 0) {

            int new_coord = coord + KingMove[n]; // Смотрим клетку
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
    // ферзь
    if (type == FIGURE_TYPE_QUEEN) {

        n = 0;
        while (QueenMove[n] != 0) {

            int new_coord = coord + QueenMove[n]; // Смотрим клетку

            while (position[new_coord] == CELL_EMPTY) { // пока не уперлись в фигуру

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


    // ладья
    if (type == FIGURE_TYPE_ROOK) {

        n = 0;
        while (RookMove[n] != 0) {

            int new_coord = coord + RookMove[n];

            while (position[new_coord] == CELL_EMPTY) { // пока не уперлись в фигуру

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

    // слон
    if (type == FIGURE_TYPE_BISHOP) {

        n = 0;
        while (BishopMove[n] != 0) {

            int new_coord = coord + BishopMove[n];
            while (position[new_coord] == CELL_EMPTY) { // пока не уперлись в фигуру

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

    //конь
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
    //пешка

    if (type == FIGURE_TYPE_PAWN) {

        // вроде бы взятие на проходе не нужно
        if (color == BLACK) {
            //проверим, можно ли есть

            cell = position[coord - 1 + 16]; // поле слева от пешки
            check_type = cell & (MASK_TYPE);
            cell_color = cell & MASK_COLOR;

            if (check_type == FIGURE_TYPE_KING && cell_color == opponent_color) {

                return 1;
            }

            cell = position[coord + 1 + 16]; // поле справа от пешки
            check_type = cell & (MASK_TYPE);
            cell_color = cell & MASK_COLOR;

            if (check_type == FIGURE_TYPE_KING && cell_color == opponent_color) {

                return 1;
            }
        }
        else {
            //проверим, можно ли есть
            cell = position[coord - 1 - 16]; // поле слева от пешки
            check_type = cell & (MASK_TYPE);
            cell_color = cell & MASK_COLOR;

            if (check_type == FIGURE_TYPE_KING && cell_color == opponent_color) {

                return 1;
            }

            cell = position[coord + 1 - 16]; // поле справа от пешки
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

// совершаем ход
void make_move(MOVE move, int depth) {

    memcpy(old_position[depth], position, 256 * sizeof(int)); // скопировали позицию до передвижений !!! переделай

    int cell = position[move.current_position];
    int new_cell = position[move.next_position];

    int cell_type = cell & MASK_TYPE;

    position[move.current_position] = cell | IS_MOVE; // говорим. что фигура ходила


    if (cell_type == FIGURE_TYPE_PAWN) {

        cell = cell ^ MASK_PASSED_PAWN_UCI; // если была проходная, то снимаем

        // если сходили пешкой на 2 хода, может можно сделать IS_MOVE
        if (move.current_position - move.next_position == 32 || move.current_position - move.next_position == -32) {

            cell = cell | MASK_PASSED_PAWN_UCI;
        }
    }


    // просто меняем позициб и обнулем
    if (move.MoveType == MOVE_TYPE_SIMPLY || move.MoveType == MOVE_TYPE_EAT) {

        position[move.current_position] = 0;
        position[move.next_position] = cell;
    }

    // берем черную проходную пешку
    if (move.MoveType == MOVE_TYPE_PASSED_PAWN_BLACK) {

        position[move.current_position] = 0;
        position[move.next_position] = 0;
        position[move.next_position - 16] = cell;
    }

    // берем черную проходную пешку
    if (move.MoveType == MOVE_TYPE_PASSED_PAWN_WHITE) {

        position[move.current_position] = 0;
        position[move.next_position] = 0;
        position[move.next_position + 16] = cell;
    }

    // если пека превртилась, нужна вакцина. Но Эйли жива
    if (move.MoveType == MOVE_TYPE_CONVERSION) {

        int color = cell & MASK_COLOR;
        position[move.current_position] = 0;
        position[move.next_position] = move.NewFigureType | color | IS_MOVE;
    }

    if (move.MoveType == MOVE_TYPE_CASTLING_O_O) {


        int king_cell = position[move.current_position];
        int rook_cell = position[move.current_position + 3];

        //position[move.current_position] = king_cell | IS_MOVE; // говорим, что король ходил
        //position[move.current_position - 4] = rook_cell ; // ладья ходила

        position[move.current_position] = 0;
        position[move.current_position + 3] = 0;
        position[move.current_position + 2] = king_cell;
        position[move.current_position + 1] = rook_cell | IS_MOVE;

    }

    if (move.MoveType == MOVE_TYPE_CASTLING_O_O_0) {

        int king_cell = position[move.current_position];
        int rook_cell = position[move.current_position - 4];

        //position[move.current_position] = king_cell | IS_MOVE; // говорим, что король ходил
        //position[move.current_position - 4] = rook_cell | IS_MOVE; // ладья ходила

        position[move.current_position] = 0;
        position[move.current_position - 4] = 0;
        position[move.current_position - 2] = king_cell;
        position[move.current_position - 1] = rook_cell | IS_MOVE;

    }
}

// убираем ход
void rollback_move(MOVE move, int depth) {

    memcpy(position, &old_position[depth], 256 * sizeof(int)); // копируем старую позицию !!! переделай
}

// дебаг, вывод всего дерева
void print_all_tree(int deep) {

    for (int i = 3; i > deep; i--) {

        board_print2(old_position[i]);
    }

    board_print2(position);
}

// получаем число ходов, которых можно сдлеть в этой позиции
int get_max_count_move(int current_player) {

    int count = 0;
    int cell, color, cell_color;

    if (current_player) {

        color = WHITE;
    }
    else {

        color = BLACK;
    }

    // сканируем доску на поисках фигуры
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

// получаем количество ходов для отдельной фигуры
int get_count_moves(int coord) {

    int count = 0;

    int n = 0;
    int cell = position[coord];

    int color = cell & MASK_COLOR;
    int opponent_color = color ^MASK_COLOR;
    int type = cell & MASK_TYPE;

    int cell_color, is_moved, is_passed;
    // король
    if (type == FIGURE_TYPE_KING) {

        n = 0;
        while (KingMove[n] != 0) {

            int new_coord = coord + KingMove[n]; // Смотрим клетку
            cell = position[new_coord];
            cell_color = cell & MASK_COLOR;

            if (cell == CELL_EMPTY) {

                count++;
            }
            else if (cell_color == opponent_color) {  // кушаем

                count++;
            }

            n++;
        }

        // рокировки
        cell = position[coord];
        is_moved = cell & MASK_MOVE;
        if (is_moved != IS_MOVE) { // если король не ходил

            // короткая рокировка
            if (position[coord + 1] == CELL_EMPTY && position[coord + 2] == CELL_EMPTY) {
                int is_rook = position[coord + 3];
                int type_is_rook = is_rook & MASK_TYPE;
                int rook_moved = is_rook & MASK_MOVE;

                // Если ладья не ходила
                if (type_is_rook == FIGURE_TYPE_ROOK && rook_moved != IS_MOVE) {
                    // Если поля для короля не бьют
                    if (is_legal_move(coord, coord + 1) && is_legal_move(coord, coord + 2)) {

                        count++;
                    }
                }
            }

            // длинная рокировка
            if (position[coord - 1] == CELL_EMPTY && position[coord - 2] == CELL_EMPTY &&
                position[coord - 3] == CELL_EMPTY) { // если клетки пустые

                int is_rook = position[coord - 4];
                int type_is_rook = is_rook & MASK_TYPE;
                int rook_moved = is_rook & MASK_MOVE;

                // Если ладья не ходила
                if (type_is_rook == FIGURE_TYPE_ROOK && rook_moved != IS_MOVE) {
                    // Если поля для короля не бьют
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

        if (color == WHITE) { //если это белая пешка

            //ход пешкой на одну клетку вперёд
            if (position[coord - 16] == CELL_EMPTY) {

                if (coord > 80 && coord < 96) { // если текущий ряд 7

                    count += 4;
                }
                else {

                    count++;
                    if (is_moved != IS_MOVE) { // если пешка не ходила

                        if (position[coord - 32] == CELL_EMPTY) {

                            count++;
                        }
                    }
                }
            }

            //проверим, можно ли есть
            cell = position[coord - 1 - 16];
            cell_color = cell & MASK_COLOR;

            if (cell_color == BLACK) {
                if (coord > 80 && coord < 96) { // если текущий ряд 7

                    count += 4;
                }
                else {

                    count++;
                }
            }

            cell = position[coord + 1 - 16];
            cell_color = cell & MASK_COLOR;
            if (cell_color == BLACK) {
                if (coord > 80 && coord < 96) { // если текущий ряд 7

                    count += 4;
                }
                else {

                    count++;
                }
            }

            // взятие проходной пешки
            if (coord > 112 && coord < 128) {

                // проходная пешка справа
                cell = position[coord + 1];
                cell_color = cell & MASK_COLOR;
                is_passed = cell & MASK_PASSED_PAWN_UCI;
                if (cell_color == BLACK && is_passed == IS_PASSED_PAWN_UCI) {

                    count++;
                }

                // проходная пешка слева
                cell = position[coord - 1];
                cell_color = cell & MASK_COLOR;
                is_passed = cell & MASK_PASSED_PAWN_UCI;
                if (cell_color == BLACK && is_passed == IS_PASSED_PAWN_UCI) {

                    count++;
                }
            }

        }
        else { //если это чёрная пешка

            if (position[coord + 16] == CELL_EMPTY) {

                if (coord > 160 && coord < 176) // если текущий ряд 2
                {
                    count += 4;
                }
                else {

                    count++;

                    if (is_moved != IS_MOVE) { // если пешка не ходила
                        if (position[coord + 32] == CELL_EMPTY) {
                            count++;
                        }
                    }
                }
            }

            //едим слева от пешки
            cell = position[coord - 1 + 16];
            cell_color = cell & MASK_COLOR;
            if (cell_color == WHITE) {

                if (coord > 160 && coord < 176) { // если текущий ряд 2

                    count += 4;
                }
                else {
                    count++;
                }
            }
            //едим справа от пешки
            cell = position[coord + 1 + 16];
            cell_color = cell & MASK_COLOR;
            if (cell_color == WHITE) {

                if (coord > 160 && coord < 176) // если текущий ряд 2
                {
                    count += 4;
                }
                else {

                    count++;
                }
            }

            // взятие проходной пешки
            if (coord > 128 && coord < 144) {

                // проходная пешка справа
                cell = position[coord + 1];
                cell_color = cell & MASK_COLOR;
                is_passed = cell & MASK_PASSED_PAWN_UCI;
                if (cell_color == WHITE && is_passed == IS_PASSED_PAWN_UCI) {
                    count++;
                }
                // проходная пешка слева
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