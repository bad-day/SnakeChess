#ifndef CHESS_UCI_H
#define CHESS_UCI_H

#define UCI_EXIT 1000

void fen_to_board(char *str);

void move_to_uci(MOVE move, char *out);

void get_position(int position, char *str);

void uci_listen();

int get_current_payer(char *str);

void *start();

int uci_to_coord(char a, char b);

#endif //CHESS_UCI_H
