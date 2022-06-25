#pragma once

#include <vector>
#include <utility>

#include "move.hpp"

// Extends bits from piece_moves.hpp
#define TURN_COLOR_BIT 0b1000000

namespace lc {
    class ChessGame {   
        private:
        // Game state (turn color, ...)
        bool              free_game;
        uint8_t           state;
        std::vector<Move> move_history;

        public:
        Board             board;

        public:
        explicit ChessGame(const Board& _board, bool _free_game = false);
        explicit ChessGame(Board&& _board, bool _free_game = false);
        
        bool move(const Position&, const Position&);
        // TODO:
        bool undo();

        std::vector<Move> piece_moveset(const Position&) const;

    };
}