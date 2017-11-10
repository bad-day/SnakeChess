#include "board.h"
#include "move.h"
#include "uci.h"

Board position;

int main() {

    move_init();
    board_init(position);

    uci_listen();
    return 0;
}