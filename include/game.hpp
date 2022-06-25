#pragma once

#include <vector>
#include <utility>

#include "move.hpp"

namespace lc {
    class ChessGame {   
        private:
        // Game state (turn color, ...)
        uint8_t           state;
        std::vector<Move> move_history;

        public:
        Board             board;

        public:
        explicit ChessGame(const Board& _board);
        explicit ChessGame(Board&& _board);
        
        // TODO:
        bool move(const Position&, const Position&);
        // TODO:
        bool undo();

        std::vector<Move> piece_moveset(const Position&) const;

    };
}