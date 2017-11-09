#include <stdio.h>
#include <time.h>
#include "board.h"
#include "move.h"
#include "evaluate.h"

Board position; // global
MOVE moves[DEPTH][200]; //ходы фигурой

MOVE out_move[2];
int current_deep;

int main() {


    test_board(position);
    //board_init(position);
    //test_board2(position);
    board_print2(position);

    for (int i = 0; i <= DEPTH; i++) {
        for (int j = 0; j < 200; j++)
            moves[i][j].MoveType = -1;
    }

    int count_end_pos = 0;
    int my_score(int depth, int alpha, int beta, int current_player) {

        int score_best;
        int score;

        if (current_player)
            score_best = -500;
        else
            score_best = 500;

        if (depth == 0) { // дошли до листка
            count_end_pos++;
            return evaluate(position, current_player);
        }

        generate_moves(depth, current_player); // поулчаем все ходы в moves[depth]

        if (moves[depth][0].MoveType == -1) {

            if (king_is_checked(WHITE)) {
                //printf("Мат белым\n");
                return -200;
            }
            else if (king_is_checked(BLACK)) {
                printf("Мат черным\n");
                board_print2(position);
                return 200;
            }
            else {
                //printf("Пат\n");
                //print_all_tree(depth);
                return 0;
            }
        }

        // проходимся по новым ходам
        for (int i = 0; moves[depth][i].MoveType != -1; i++) {

            make_move(moves[depth][i], depth); // тут меняется состояние доски

            score = my_score(depth - 1, -beta, -alpha, !current_player);

            rollback_move(moves[depth][i], depth); // возвращаем прежнюю доску

            if (current_player) { // если белые, максимимзируем

                if (score > score_best) {
                    printf("белые");
                    if(depth == current_deep) {

                        out_move[1] = moves[depth][i];
                    }

                    score_best = score; // максимальное в данном поддереве
                }

            }
            else { // минимизируем

                if (score < score_best) {

                    if(depth == current_deep) {
                        //printf("черные");
                        out_move[0] = moves[depth][i];
                    }

                    score_best = score;
                }
            }

            moves[depth][i].MoveType = -1;
        }


        return score_best;
    }

    time_t time1, time2;

   // int player = 1; // ходят белые
    //time1 = clock();
   // current_deep = 8;
   // int score = my_score(current_deep, -100, 100, player);

    int player = 1;



    current_deep = 7;
//    int deep = current_deep;
    while(current_deep > 1) {

        int score = my_score(current_deep, -100, 100, player);
        printf("Оценка: %d\n", score);

        make_move(out_move[player], 0);
        //board_print2(position);

        //printf(" %d %d %d %d %d\n", player, test, out_move[player].MoveType, out_move[player].current_position, out_move[player].next_position);

        player = !player;
        current_deep--;

    }

    //board_print2(position);
    //printf("Оценка: %d\n", score);
    //printf("Время: %f \n", ((float) (time2 - time1) / 1000000.0) * 1000);

    //printf("Количество листьев: %d", count_end_pos);

    return 0;
}