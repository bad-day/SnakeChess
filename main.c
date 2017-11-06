#include <stdio.h>
#include "board.h"
#include "move.h"


Board position; // global

int *KingWhitePointer; //указатель на короля в массиве позиций белых
int *KingBlackPointer; //указатель на короля в массиве позиций чёрных

MOVE moves[DEPTH][200];//ходы фигурой



int main() {


    test_board(position);
    board_print(position);


    for (int i = 0; i < DEPTH; i++)
        for (int j = 0; j < 200; j++)
            moves[i][j].MoveType = -1;

int count_end_pos = 0;
    int my_score(int depth, int alpha, int beta, int current_player) {

        if (depth == 0) { // дошли до листка
            board_print(position);
            count_end_pos++;
            return 0;
        }


        //printf("Зашли генерить новые ходы, глубина %d\n", depth);
        generate_moves(depth, current_player); // поулчаем все ходы в moves[depth]
        int test = 0;
        for ( test = 0; moves[depth][test].MoveType != -1; test++);
        printf("Количество ходов, сгенеренных: %d\n", test);


        for (int i = 0; moves[depth][i].MoveType != -1; i++) {


            make_move(moves[depth][i], depth); // тут меняется состояние доски

                my_score(depth - 1, beta, alpha, !current_player);

            rollback_move(moves[depth][i], depth); // возвращаем прежнюю доску

            moves[depth][i].MoveType = -1; // чтобы цикл работал. Сделать очередь?
        }

        return 0;

    }

    my_score(2, 0, 0, 1);
    printf("количество листьев: %d", count_end_pos);

    return 0;
}