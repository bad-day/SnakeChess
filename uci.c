//
// Created by valera on 10.11.17.
//
#include <stdio.h>
#include <string.h>
#include "move.h"
#include "board.h"

extern Board position;

// загружаем доску в движок
// доделай рокировки и переделай
void fen_to_board(char* str) {

    int arr[64];

    for (int i = 0, k = 0, z = 0; str[i] != 0 && str[i] != ' '; i++, k++) {

        //if(str[i] == ' ')

        if(str[i] > '0' && str[i] < '9') {
            for (int j = 0; j < str[i] - '0'; j++, k++) {
                arr[k] = 0;
            }
            k--;
        }
        // black
        if(str[i] == 'r') {
            arr[k] = FIGURE_TYPE_ROOK | BLACK;
        }
        if(str[i] == 'n') {
            arr[k] = FIGURE_TYPE_KNIGHT | BLACK;
        }
        if(str[i] == 'b') {
            arr[k] = FIGURE_TYPE_BISHOP | BLACK;
        }
        if(str[i] == 'q') {
            arr[k] = FIGURE_TYPE_QUEEN | BLACK;
        }
        if(str[i] == 'k') {
            arr[k] = FIGURE_TYPE_KING | BLACK;
        }
        if(str[i] == 'p') {
            if(z == 1)
                arr[k] = FIGURE_TYPE_PAWN | BLACK;
            else
                arr[k] = FIGURE_TYPE_PAWN | BLACK | IS_MOVE;
        }

        // white
        if(str[i] == 'R') {
            arr[k] = FIGURE_TYPE_ROOK | WHITE;
        }
        if(str[i] == 'N') {
            arr[k] = FIGURE_TYPE_KNIGHT | WHITE;
        }
        if(str[i] == 'B') {
            arr[k] = FIGURE_TYPE_BISHOP | WHITE;
        }
        if(str[i] == 'Q') {
            arr[k] = FIGURE_TYPE_QUEEN | WHITE;
        }
        if(str[i] == 'K') {
            arr[k] = FIGURE_TYPE_KING | WHITE;
        }
        if(str[i] == 'P') {
            if(z == 6)
                arr[k] = FIGURE_TYPE_PAWN | WHITE;
            else
                arr[k] = FIGURE_TYPE_PAWN | WHITE | IS_MOVE;
        }

        if(str[i] == '/') {
            k--;
            z++;
        }
    }

//    for(int i = 0; i < 64; i++) {
//
//        if ( i % 8 == 0) {
//            printf("\n");
//        }
//        printf("%d\t", arr[i]);
//    }



        for (int i = 0, k = 0; i < 8; i++) {
            for (int j = 68 + i * 16; j < 76 + i * 16; j++, k++) {

                position[j] = arr[k];
            }

        }

}

void get_position(int position, char* str) {

    position = position - 64;

    int x = position % 16 - 4;
    int y = 8 - position / 16;

    str[0] = x + 'a';
    str[1] = y + '0';
}

void move_to_uci(MOVE move, char* out) {

    // получаем координату в нашеим виде e2
    int first = move.current_position;
    int second = move.next_position;

    char first_char[3];
    char second_char[3];

    get_position(first, first_char);
    get_position(second, second_char);

    if(move.MoveType == MOVE_TYPE_SIMPLY || move.MoveType == MOVE_TYPE_EAT){

        out[0] = first_char[0];
        out[1] = first_char[1];
        out[2] = second_char[0];
        out[3] = second_char[1];
        out[4] = 0;
    }

    if(move.MoveType == MOVE_TYPE_CONVERSION){

        char new_fig;

        if(move.NewFigureType == FIGURE_TYPE_QUEEN)
            new_fig = 'q';

        if(move.NewFigureType == FIGURE_TYPE_ROOK)
            new_fig = 'r';

        if(move.NewFigureType == FIGURE_TYPE_BISHOP)
            new_fig = 'b';

        if(move.NewFigureType == FIGURE_TYPE_KNIGHT)
            new_fig = 'n';

        out[0] = first_char[0];
        out[1] = first_char[1];
        out[2] = second_char[0];
        out[3] = second_char[1];
        out[4] = new_fig;
        out[5] = 0;
    }
    FILE *fp;
    fp = fopen("log2", "wb");
    fwrite(out, strlen(out), 1, fp);

}
