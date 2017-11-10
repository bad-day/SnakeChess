#include <stdio.h>
#include <time.h>
#include <string.h>

#include "board.h"
#include "move.h"
#include "evaluate.h"
#include "uci.h"

Board position; // global
MOVE moves[DEPTH][200]; //ходы фигурой

MOVE out_move;
int current_deep;

int main() {


    //test_board(position);
    board_init(position);
    //test_board2(position);
    //board_print2(position);

    for (int i = 0; i <= DEPTH; i++) {
        for (int j = 0; j < 200; j++)
            moves[i][j].MoveType = -1;
    }

    int count_end_pos = 0;
    int my_score(int depth, int alpha, int beta, int current_player) {

        if (depth == 0) { // дошли до листка
            count_end_pos++;
            return evaluate(position, current_player);
        }

        int score_best;
        int score;

        score_best = -300;

        generate_moves(depth, current_player); // поулчаем все ходы в moves[depth]

        if (moves[depth][0].MoveType == -1) {

            if (king_is_checked(WHITE)) {
                //printf("Мат белым\n");
                return 200;
            }
            else if (king_is_checked(BLACK)) {
                if (depth < 4) {
                    //printf("Мат черным\n");
                    //board_print2(position);
                }

                //
                return -200;
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

            score = -my_score(depth - 1, -beta, -alpha, !current_player);

            rollback_move(moves[depth][i], depth); // возвращаем прежнюю доску

            if (score > score_best) {
                score_best = score;              // обновляем  лучшую оценку
                if (depth == current_deep) {
                    //printf(" %d %d %d\n", moves[depth][i].MoveType, moves[depth][i].current_position, moves[depth][i].next_position);
                    out_move = moves[depth][i];
                }

            }

            moves[depth][i].MoveType = -1;
        }

        return score_best;
    }

    time_t time1, time2;

    // int player = 1; // ходят белые

    // current_deep = 8;
    // int score = my_score(current_deep, -100, 100, player);

//    current_deep = 7;
//    int score = my_score(current_deep, -100, 100, !player);
//
//    printf("Оценка: %d\n", score);

    //board_print2(position);
    //printf("Оценка: %d\n", score);
    //printf("Время: %f \n", ((float) (time2 - time1) / 1000000.0) * 1000);

    //printf("Количество листьев: %d", count_end_pos);

    char input[100] ;






    while(fgets(input, 90, stdin)) {

        if(strstr(input, "uci")) {

            printf("id name Demo_engine\n");
            printf("id author XXX\n");
            printf("uciok\n");
            printf("position startpos\n");

        }

        if(strstr(input, "quit")) {

            printf("quit\n");
            return 0;

        }

        if(strstr(input, "isready")) {

            printf("readyok\n");

        }

        if(strstr(input, "go infinite")) {

            int player = 1; // ходят белые
            int deep = current_deep = 1;
            while (current_deep < 8) {

                time1 = clock();
                int score = 100*my_score(current_deep, -100, 100, player);
                time2 = clock();

                if(player == 0) {
                    score = -score;
                }

                int time_def = (int)((time2 - time1)/1000);

                char best_move[100];
                move_to_uci(out_move, best_move);


                printf("info depth %d score cp %d time %d pv %s\n", current_deep, score, time_def, best_move);


                fflush(stdout);
                current_deep++;
            }


            //printf("info depth 4 score cp -1 time 10 nodes 26 nps 633 pv e2e4\n");

            //printf("info depth 4 score cp -300 time 55 nodes 1292 nps 25606 pv d7d5 e2e3 e7e6 g1f3\n");
            //printf("bestmove e2e4\n");

        }

        if(strstr(input, "position fen ")) {
            // выставляем позицию, текущего игрока и пр


            char changed[100];
            strcpy(changed, &input[13]);

            fen_to_board(changed);
            //fen_to_board("rnb1kbnr/pp1ppppp/8/q1p5/4P3/2N3P1/PPPP1P1P/R1BQKBNR b KQkq - 0 3");
        }


        fflush(stdout);


    }


    //strcmp()
//
//    fgets(Line,10,stdin);
//
//    if (Line == "uci") {
//        printf("id name Demo_engine\n");
//        printf("id author XXX\n");
//        printf("uciok\n");
//    }


    return 0;
}