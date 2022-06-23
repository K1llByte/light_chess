#pragma once

#include <vector>

#include "move.hpp"

namespace lc {
    // using PositionDiff = std::array<int8_t,2>;
    // inline PositionDiff position_diff(
    //     const Position& from,
    //     const Position& to);

    inline std::vector<Move> pawn_moves(
        const Board& board,
        const Piece& piece,
        const Position& pos);
    inline std::vector<Move> knight_moves(
        const Board& board,
        const Piece& piece,
        const Position& pos);
    inline std::vector<Move> bishop_moves(
        const Board& board,
        const Piece& piece,
        const Position& pos);
    inline std::vector<Move> rook_moves(
        const Board& board,
        const Piece& piece,
        const Position& pos);
    inline std::vector<Move> queen_moves(
        const Board& board,
        const Piece& piece,
        const Position& pos);
    inline std::vector<Move> king_moves(
        const Board& board,
        const Piece& piece,
        const Position& pos);
}

/////////////// Implementation ///////////////

namespace lc {
    // inline PositionDiff position_diff(
    //     const Position& from,
    //     const Position& to)
    // {
    //     return {
    //         static_cast<int8_t>(from[0] - to[0]),
    //         static_cast<int8_t>(from[1] - to[1])
    //     };
    // }


    std::vector<Move> pawn_moves(
        const Board& board,
        const Piece& piece,
        const Position& from)
    {
        std::vector<Move> moves;
        
        // Only in pawn the direction of the moveset matters
        if(piece.color() == WHITE) {
            // Normal
            // First double
            // Capture
            // En Passant
        }
        else {
            // Normal
            {
                auto to = Position{from[0], from[1]};
                if(board.at(to).kind() == NONE) {
                    moves.emplace_back(Move::normal(from, to));
                }
            }
            // First double
            // Capture
            // En Passant
        }

        return moves;
    }

    std::vector<Move> knight_moves(
        const Board& board,
        const Piece& piece,
        const Position& pos)
    {
        std::vector<Move> moves;
        return moves;
    }

    std::vector<Move> bishop_moves(
        const Board& board,
        const Piece& piece,
        const Position& pos)
    {
        std::vector<Move> moves;
        return moves;
    }

    std::vector<Move> rook_moves(
        const Board& board,
        const Piece& piece,
        const Position& pos)
    {
        std::vector<Move> moves;
        return moves;
    }

    std::vector<Move> queen_moves(
        const Board& board,
        const Piece& piece,
        const Position& pos)
    {
        std::vector<Move> moves;
        return moves;
    }

    std::vector<Move> king_moves(
        const Board& board,
        const Piece& piece,
        const Position& pos)
    {
        std::vector<Move> moves;
        return moves;
    }
}