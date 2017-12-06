#ifndef CHESS_UCI_H
#define CHESS_UCI_H

// listen GUI
void uci_listen();

// thread to infinite analyze
void *start();

// load board to engine
void fen_to_board(char *str);

// change coord of the form a1-h8 to form 0-255
int uci_to_coord(char a, char b);

// change coord of the form 0-255 to form a1-h8
void get_position(int position, char *str);

// change MOVE to form a1-h8
void move_to_uci(MOVE move, char *out);

#endif //CHESS_UCI_H
