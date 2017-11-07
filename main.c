#include <stdio.h>
#include <time.h>
#include "board.h"
#include "move.h"


Board position; // global


MOVE moves[DEPTH][200];//ходы фигурой



int main() {


    //test_board(position);
    board_init(position);
    //test_board2(position);
    board_print2(position);


    for (int i = 0; i < DEPTH; i++)
        for (int j = 0; j < 200; j++)
            moves[i][j].MoveType = -1;

    int count_end_pos = 0;

    int my_score(int depth, int alpha, int beta, int current_player) {

        if (depth == 0) { // дошли до листка
            //board_print(position);
            count_end_pos++;
            return 0;
        }

        //printf("Зашли генерить новые ходы, глубина %d\n", depth);
        generate_moves(depth, current_player); // поулчаем все ходы в moves[depth]
        int test = 0;
        for ( test = 0; moves[depth][test].MoveType != -1; test++);

        if(test == 0) {

            //printf(" %d \n", test);
            //printf(" %d %d", moves[depth][0].current_position, moves[depth][0].next_position);


            board_print2(position);
        }
        //printf("Количество ходов, сгенеренных: %d\n", test);


        for (int i = 0; moves[depth][i].MoveType != -1; i++) {

            make_move(moves[depth][i], depth); // тут меняется состояние доски


                my_score(depth - 1, beta, alpha, !current_player);

            rollback_move(moves[depth][i], depth); // возвращаем прежнюю доску

            moves[depth][i].MoveType = -1; // чтобы цикл работал. Сделать очередь?
        }

        return 0;

    }

    time_t time1, time2;

    // Считываем текущее время
    time1 = clock();

    my_score(6, 0, 0, 1);

    time2 = clock();

    float diff = ((float)(time2 - time1) / 1000000.0 ) * 1000;
    printf("Время: %f \n", diff);
    printf("Количество листьев: %d", count_end_pos);

    return 0;
}