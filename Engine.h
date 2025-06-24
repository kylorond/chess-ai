#ifndef ENGINE_H
#define ENGINE_H

#include "Board.h"
#include "Defs.h"

class Engine {
public:
    Move find_best_move(Board& board, int depth);

private:
    int evaluate(Board& board);
    int minimax(Board& board, int depth, bool maximizing_player);
};

#endif // ENGINE_H