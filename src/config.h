#ifndef CHESS_CONFIG_H
#define CHESS_CONFIG_H

// max deep
#define DEPTH 100
// infinity value
#define INF 999999
// checkmate value
#define CHECKMATE 5000

#define CHECK_DEEPENING 1
#define FORCED_MATE_DEEP 15
#define NULL_MOVE_DEPTH 4
#define FUTILITY_DEPTH 4

#define PAWN_WEIGH 100
#define KNIGHT_WEIGH 300
#define BISHOP_WEIGH 310
#define ROOK_WEIGH 500
#define QUEEN_WEIGH 900

#endif //CHESS_CONFIG_H
