//
// Created by valera on 08.11.17.
//

#include "board.h"

//extern Board position;

int evaluate(Board position, int player) {

    int cell, type, color;
    int black = 0, white = 0;
    // сканируем доску на поисках фигуры
    for (int i = 0; i < 8; i++) {
        for (int j = 68 + i * 16; j < 76 + i * 16; j++) {
            if (position[j] != 0) {

                cell = position[j];
                color = cell & MASK_COLOR;
                type = cell & MASK_TYPE;

                if (color == WHITE) {
                    switch (type) {
                        case FIGURE_TYPE_QUEEN:
                            white += 9;
                            break;

                        case FIGURE_TYPE_ROOK:
                            white += 5;
                            break;

                        case FIGURE_TYPE_BISHOP:
                            white += 3;
                            break;

                        case FIGURE_TYPE_KNIGHT:
                            white += 3;
                            break;

                        case FIGURE_TYPE_PAWN:
                            white += 1;
                            break;
                    }
                }
                else {
                    switch (type) {
                        case FIGURE_TYPE_QUEEN:
                            black += 9;
                            break;

                        case FIGURE_TYPE_ROOK:
                            black += 5;
                            break;

                        case FIGURE_TYPE_BISHOP:
                            black += 3;
                            break;

                        case FIGURE_TYPE_KNIGHT:
                            black += 3;
                            break;

                        case FIGURE_TYPE_PAWN:
                            black += 1;
                            break;
                    }
                }
            }
        }
    }

    if(player)
        return white - black;
    else
        return -1*(white - black);
}
