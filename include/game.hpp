#pragma once

#include <vector>
#include <utility>

#include "move.hpp"

#define WHITE_SHORT_ROOK_MOVED_BIT 0b000001
#define WHITE_KING_MOVED_BIT       0b000010
#define WHITE_LONG_ROOK_MOVED_BIT  0b000100
#define BLACK_SHORT_ROOK_MOVED_BIT 0b001000
#define BLACK_KING_MOVED_BIT       0b010000
#define BLACK_LONG_ROOK_MOVED_BIT  0b100000

#define IN_BOUNDS(x) (0 <= x) && (x < 8)

namespace lc {
    class ChessGame {   
        private:
        // Game state (turn color, ...)
        uint8_t           state;
        std::vector<Move> move_history;
        Board             board;

        public:
        ChessGame(const Board& _board);
        
        // TODO:
        bool move(const Position&, const Position&);
        // TODO:
        constexpr void undo();
    };
}