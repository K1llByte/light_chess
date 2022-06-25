#pragma once

#include <vector>
#include <algorithm>
#include <optional>

#include "move.hpp"

#define IN_BOUNDS(pos) (pos[0] <= 7 && pos[1] <= 7)

#define WHITE_KINGSIDE_ROOK_MOVED_BIT  0b000001
#define WHITE_KING_MOVED_BIT           0b000010
#define WHITE_QUEENSIDE_ROOK_MOVED_BIT 0b000100
#define BLACK_KINGSIDE_ROOK_MOVED_BIT  0b001000
#define BLACK_KING_MOVED_BIT           0b010000
#define BLACK_QUEENSIDE_ROOK_MOVED_BIT 0b100000

namespace lc {
    using PositionDiff = std::array<int8_t,2>;
    inline PositionDiff position_diff(
        const Position& from,
        const Position& to);

    inline std::vector<Move> pawn_moves(
        const Board& board,
        const Piece& piece,
        const Position& pos,
        const std::optional<Move>& previous_move);
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
        const Position& pos,
        uint8_t state);
}

/////////////// Implementation ///////////////

namespace lc {
    inline PositionDiff position_diff(
        const Position& from,
        const Position& to)
    {
        return {
            static_cast<int8_t>(from[0] - to[0]),
            static_cast<int8_t>(from[1] - to[1])
        };
    }


    std::vector<Move> pawn_moves(
        const Board& board,
        const Piece& piece,
        const Position& from,
        const std::optional<Move>& previous_move)
    {
        std::vector<Move> moves;
        
        // Only in pawn the direction of the moveset matters
        const int8_t direction = piece.is_white() ? -1 : 1;
        // Normal
        {
            const auto to = Position{from[0], uint8_t(from[1] + direction)};
            
            if(IN_BOUNDS(to) && board.at(to).kind() == NONE) {
                // Check if there's promotion
                if((piece.is_white() && to[1] == 0)
                    || (piece.is_black() && to[1] == 7)) {
                    moves.emplace_back(Move::promotion(from, to, QUEEN));
                }
                else {
                    moves.emplace_back(Move::normal(from, to));
                }
            }
        }
        
        // First double
        {
            const auto to1 = Position{from[0], uint8_t(from[1] + direction)};
            const auto to2 = Position{from[0], uint8_t(from[1] + 2*direction)};
            // Position to1 is valid and has no pieces
            if(IN_BOUNDS(to1) && board.at(to1).kind() == NONE
            // Position to2 is valid and has no pieces
                && IN_BOUNDS(to2) && board.at(to2).kind() == NONE
            // Is first move of piece
                && ((piece.is_white() && from[1] == 6)
                    || (piece.is_black() && from[1] == 1)))
            {
                moves.emplace_back(Move::normal(from, to2));
            }
        }
        
        // Capture
        {
            const auto to1 = Position{
                uint8_t(from[0] - 1),
                uint8_t(from[1] + direction)
            };
            const auto to2 = Position{
                uint8_t(from[0] + 1),
                uint8_t(from[1] + direction)
            };
            
            // Left capture
            if(IN_BOUNDS(to1)) {
                const auto to1_piece = board.at(to1);
                // Position on diagonal left has opposite color piece
                if(to1_piece.color() != piece.color()) {
                    // Check if promotion
                    if((piece.is_white() && to1[1] == 0)
                        || (piece.is_black() && to1[1] == 7)) {
                        moves.emplace_back(Move::promotion(from, to1, QUEEN, to1_piece));
                    }
                    else {
                        moves.emplace_back(Move::normal(from, to1, to1_piece));
                    }
                }
            }

            // Right capture
            if(IN_BOUNDS(to2)) {
                const auto to2_piece = board.at(to2);
                // Position on diagonal right has opposite color piece
                if(to2_piece.color() != piece.color()) {
                    // Check if promotion
                    if((piece.is_white() && to2[1] == 0)
                        || (piece.is_black() && to2[1] == 7)) {
                        moves.emplace_back(Move::promotion(from, to2, QUEEN, to2_piece));
                    }
                    else {
                        moves.emplace_back(Move::normal(from, to2, to2_piece));
                    }
                }
            }
        }
        
        // En Passant
        {
            // If there's no previous move, en passant can't be done
            if(previous_move.has_value()) {
                const auto to1 = Position{
                    uint8_t(from[0] - 1),
                    uint8_t(from[1] + direction)
                };
                const auto to2 = Position{
                    uint8_t(from[0] + 1),
                    uint8_t(from[1] + direction)
                };
                auto previous_move_diff = position_diff(previous_move->from(), previous_move->to());
                if(board.at(previous_move->to()).kind() == PAWN
                    && std::abs(previous_move_diff[1]) == 2)
                {
                    // NOTE: En passant can only be performed at one side in a turn
                    // Previous move is a pawn with diff == 2 with position.x == from.x-1 
                    if(previous_move->to()[0] == from[0]-1) {
                        moves.emplace_back(Move::en_passant(from, to1));
                    }
                    // Previous move is a pawn with diff == 2 with position.x == from.x+1
                    else if(previous_move->to()[0] == from[0]+1) {
                        moves.emplace_back(Move::en_passant(from, to2));
                    }
                }
            }
        }

        return moves;
    }

    std::vector<Move> knight_moves(
        const Board& board,
        const Piece& piece,
        const Position& pos)
    {
        std::vector<Move> moves;
        static const std::array<int8_t,2> diffs[] = {
            { 1, 2}, {-1,-2}, { 1,-2}, {-1, 2},
            { 2, 1}, {-2,-1}, { 2,-1}, {-2, 1}
        };

        for(const auto& diff : diffs) {
            auto to = Position{
                uint8_t(pos[0]+diff[0]),
                uint8_t(pos[1]+diff[1])
            };
            if(IN_BOUNDS(to)) {
                auto to_piece = board.at(to);
                if(to_piece.kind() == NONE
                    || to_piece.color() != piece.color())
                {
                    moves.emplace_back(Move::normal(pos,to,to_piece));
                }
            }
        }
        return moves;
    }

    std::vector<Move> bishop_moves(
        const Board& board,
        const Piece& piece,
        const Position& pos)
    {
        std::vector<Move> moves;
        auto diretional_check = [&](int8_t x_dir, int8_t y_dir) {
            Piece to_piece = NONE;
            for(int8_t i = 1; i < 8; ++i) {
                auto to = Position{
                    uint8_t(pos[0]+(i*x_dir)),
                    uint8_t(pos[1]+(i*y_dir))
                };
                if(!IN_BOUNDS(to))
                    break;
                to_piece = board.at(to);
                if(to_piece.kind() == NONE) {
                    moves.emplace_back(Move::normal(pos, to));
                }
                else if(to_piece.color() != piece.color()) {
                    moves.emplace_back(Move::normal(pos, to, to_piece));
                    break;
                }
                else {
                    break;
                }
            }
        };
        
        // North East
        diretional_check(1,-1);
        // North West
        diretional_check(-1,-1);
        // South East
        diretional_check(1,1);
        // South West
        diretional_check(-1,1);

        return moves;
    }

    std::vector<Move> rook_moves(
        const Board& board,
        const Piece& piece,
        const Position& pos)
    {
        std::vector<Move> moves;
        auto diretional_check = [&](int8_t x_dir, int8_t y_dir) {
            Piece to_piece = NONE;
            for(int8_t i = 1; i < 8; ++i) {
                auto to = Position{
                    uint8_t(pos[0]+(i*x_dir)),
                    uint8_t(pos[1]+(i*y_dir))
                };
                if(!IN_BOUNDS(to))
                    break;
                to_piece = board.at(to);
                if(to_piece.kind() == NONE) {
                    moves.emplace_back(Move::normal(pos, to));
                }
                else if(to_piece.color() != piece.color()) {
                    moves.emplace_back(Move::normal(pos, to, to_piece));
                    break;
                }
                else {
                    break;
                }
            }
        };
        
        // North
        diretional_check(0,-1);
        // South
        diretional_check(0,1);
        // East
        diretional_check(1,0);
        // West
        diretional_check(-1,0);

        return moves;
    }

    std::vector<Move> queen_moves(
        const Board& board,
        const Piece& piece,
        const Position& pos)
    {
        std::vector<Move> moves;
        auto diretional_check = [&](int8_t x_dir, int8_t y_dir) {
            Piece to_piece = NONE;
            for(int8_t i = 1; i < 8; ++i) {
                auto to = Position{
                    uint8_t(pos[0]+(i*x_dir)),
                    uint8_t(pos[1]+(i*y_dir))
                };
                if(!IN_BOUNDS(to))
                    break;
                to_piece = board.at(to);
                if(to_piece.kind() == NONE) {
                    moves.emplace_back(Move::normal(pos, to));
                }
                else if(to_piece.color() != piece.color()) {
                    moves.emplace_back(Move::normal(pos, to, to_piece));
                    break;
                }
                else {
                    break;
                }
            }
        };
        
        // North
        diretional_check(0,-1);
        // South
        diretional_check(0,1);
        // East
        diretional_check(1,0);
        // West
        diretional_check(-1,0);
        // North East
        diretional_check(1,-1);
        // North West
        diretional_check(-1,-1);
        // South East
        diretional_check(1,1);
        // South West
        diretional_check(-1,1);

        return moves;
    }

    std::vector<Move> king_moves(
        const Board& board,
        const Piece& piece,
        const Position& pos,
        uint8_t state)
    {
        std::vector<Move> moves;
        // Normal
        static const std::array<int8_t,2> diffs[] = {
            { 1, 0}, {-1, 0}, { 0,-1}, { 0, 1},
            { 1, 1}, {-1,-1}, { 1,-1}, {-1, 1}
        };
        for(const auto& diff : diffs) {
            auto to = Position{
                uint8_t(pos[0]+diff[0]),
                uint8_t(pos[1]+diff[1])
            };
            if(IN_BOUNDS(to)) {
                auto to_piece = board.at(to);
                if(to_piece.kind() == NONE
                    || to_piece.color() != piece.color())
                {
                    moves.emplace_back(Move::normal(pos,to,to_piece));
                }
            }
        }
        // Castling
        {
            static const int8_t info_bits[2][3] = {
                { 
                    WHITE_QUEENSIDE_ROOK_MOVED_BIT , 
                    WHITE_KING_MOVED_BIT      , 
                    WHITE_KINGSIDE_ROOK_MOVED_BIT
                },
                { 
                    BLACK_QUEENSIDE_ROOK_MOVED_BIT , 
                    BLACK_KING_MOVED_BIT      , 
                    BLACK_KINGSIDE_ROOK_MOVED_BIT 
                }
            };
            
            // TODO: Do this only if there's no check
            auto color = piece.color();
            if(!(state & info_bits[color][1])) {
                bool between_empty = true;
                if(!(state & info_bits[color][0])) {
                    // Check if positions in between have no pieces
                    for(int8_t i = 1; i < 4 && between_empty; ++i) {
                        auto to = Position{
                            uint8_t(pos[0] - i),
                            pos[1]
                        };
                        between_empty = board.at(to).kind() == NONE;
                    }
                    // If yes 
                    if(between_empty) {
                        moves.emplace_back(Move::castling(pos, {uint8_t(pos[0]-2), pos[1]}));
                    }
                }
                if(!(state & info_bits[color][2])) {
                    between_empty = true;
                    // Check if positions in between have no pieces
                    for(int8_t i = 1; i < 3 && between_empty; ++i) {
                        auto to = Position{
                            uint8_t(pos[0] + i),
                            pos[1]
                        };
                        between_empty = board.at(to).kind() == NONE;
                    }
                    // If yes 
                    if(between_empty) {
                        moves.emplace_back(Move::castling(pos, {uint8_t(pos[0]+2), pos[1]}));
                    }
                }
            }
        }
        return moves;
    }
}