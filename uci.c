//
// Created by valera on 10.11.17.
//
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <pthread.h>

#include "move.h"
#include "board.h"
#include "evaluate.h"

extern Board position;
extern int current_deep;
extern int uci_status;

MOVE out_move;

pthread_t thread;
int current_player;
// загружаем доску в движок
// проходную пешку подумай
void fen_to_board(char *str) {

    int arr[64];
    for (int i = 0, k = 0, z = 0; str[i] != 0 && str[i] != ' '; i++, k++) {

        if (str[i] > '0' && str[i] < '9') {
            for (int j = 0; j < str[i] - '0'; j++, k++) {
                arr[k] = 0;
            }
            k--;
        }

        // black
        if (str[i] == 'r') {
            arr[k] = FIGURE_TYPE_ROOK | BLACK;
        }
        if (str[i] == 'n') {
            arr[k] = FIGURE_TYPE_KNIGHT | BLACK;
        }
        if (str[i] == 'b') {
            arr[k] = FIGURE_TYPE_BISHOP | BLACK;
        }
        if (str[i] == 'q') {
            arr[k] = FIGURE_TYPE_QUEEN | BLACK;
        }
        if (str[i] == 'k') {
            arr[k] = FIGURE_TYPE_KING | BLACK;
        }
        if (str[i] == 'p') {
            if (z == 1)
                arr[k] = FIGURE_TYPE_PAWN | BLACK;
            else
                arr[k] = FIGURE_TYPE_PAWN | BLACK | IS_MOVE;
        }

        // white
        if (str[i] == 'R') {
            arr[k] = FIGURE_TYPE_ROOK | WHITE;
        }
        if (str[i] == 'N') {
            arr[k] = FIGURE_TYPE_KNIGHT | WHITE;
        }
        if (str[i] == 'B') {
            arr[k] = FIGURE_TYPE_BISHOP | WHITE;
        }
        if (str[i] == 'Q') {
            arr[k] = FIGURE_TYPE_QUEEN | WHITE;
        }
        if (str[i] == 'K') {
            arr[k] = FIGURE_TYPE_KING | WHITE;
        }
        if (str[i] == 'P') {
            if (z == 6)
                arr[k] = FIGURE_TYPE_PAWN | WHITE;
            else
                arr[k] = FIGURE_TYPE_PAWN | WHITE | IS_MOVE;
        }

        if (str[i] == '/') {
            k--;
            z++;
        }
    }

    for (int i = 0, k = 0; i < 8; i++) {
        for (int j = 68 + i * 16; j < 76 + i * 16; j++, k++) {

            position[j] = arr[k];
        }
    }
}

void get_position(int position, char *str) {

    position = position - 64;

    int x = position % 16 - 4;
    int y = 8 - position / 16;

    str[0] = x + 'a';
    str[1] = y + '0';
}

// !!!
int get_current_payer(char *str) {

    int i = 0;
    for (int j = 0; j < 3; j++) {
        while (str[i] != ' ') {
            i++;
        }
    }


    if (str[i + 1] == 'w') {
        return 1;
    }
    return 0;
}
// довольно криво
void move_to_uci(MOVE move, char *out) {

    // получаем координату в нашеим виде e2
    int first = move.current_position;
    int second = move.next_position;

    char first_char[3];
    char second_char[3];

    get_position(first, first_char);
    get_position(second, second_char);

    if (move.MoveType == MOVE_TYPE_SIMPLY || move.MoveType == MOVE_TYPE_EAT ||
        move.MoveType == MOVE_TYPE_CASTLING_O_O || move.MoveType == MOVE_TYPE_CASTLING_O_O) {

        out[0] = first_char[0];
        out[1] = first_char[1];
        out[2] = second_char[0];
        out[3] = second_char[1];
        out[4] = 0;
    }

    if (move.MoveType == MOVE_TYPE_CONVERSION) {

        char new_fig;

        if (move.NewFigureType == FIGURE_TYPE_QUEEN)
            new_fig = 'q';

        if (move.NewFigureType == FIGURE_TYPE_ROOK)
            new_fig = 'r';

        if (move.NewFigureType == FIGURE_TYPE_BISHOP)
            new_fig = 'b';

        if (move.NewFigureType == FIGURE_TYPE_KNIGHT)
            new_fig = 'n';

        out[0] = first_char[0];
        out[1] = first_char[1];
        out[2] = second_char[0];
        out[3] = second_char[1];
        out[4] = new_fig;
        out[5] = 0;
    }
}

void* start() {

    time_t time1, time2;

    current_deep = 2;
    uci_status = 1; // проверяется в глобальной перемсенной в алгоритме
    while (current_deep < DEPTH) {

        time1 = clock();
        int score = 100*negamax(current_deep, -300, 300, current_player);
        time2 = clock();

        if(!current_player) {
            score = -score;
        }

        int time_def = (int)((time2 - time1)/1000);

        char best_move[100];
        move_to_uci(out_move, best_move);

        if(score != -1000 && score != 1000)
            printf("info depth %d score cp %d time %d pv %s\n", current_deep, score, time_def, best_move);

        fflush(stdout);
        current_deep++;
    }
}

void uci_listen() {

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
        }

        if(strstr(input, "isready")) {

            printf("readyok\n");
        }

        if(strstr(input, "go infinite")) {

            pthread_create(&thread, NULL, start, NULL);
        }

        if(strstr(input, "position fen ")) {

            uci_status = 0;

            char changed[100];
            strcpy(changed, &input[13]);

            fen_to_board(changed);


            current_player = get_current_payer(changed);

            //fen_to_board("rnb1kbnr/pp1ppppp/8/q1p5/4P3/2N3P1/PPPP1P1P/R1BQKBNR b KQkq - 0 3");
        }

        if(strstr(input, "stop")) {

            uci_status = 0;
        }

        fflush(stdout);
    }
}