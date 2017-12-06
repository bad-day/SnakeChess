#ifndef CHESS_CONFIG_H
#define CHESS_CONFIG_H

// max deep
#define DEPTH 100
// infinity value
#define INF 999999

#define CHECK_DEEPENING 1
#define FORCED_MATE_DEEP 15
#define NULL_MOVE_DEPTH 3
#define FUTILITY_DEPTH 3

#define PAWN_WEIGH 100
#define KNIGHT_WEIGH 300
#define BISHOP_WEIGH 300
#define ROOK_WEIGH 500
#define QUEEN_WEIGH 900

#endif //CHESS_CONFIG_H
