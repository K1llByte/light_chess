#pragma once

#include <vector>
#include <algorithm>
#include <optional>

#include "move.hpp"

#define IN_BOUNDS(pos) (pos[0] <= 7 && pos[1] <= 7)

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
        const Position& pos);
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
        return moves;
    }

    std::vector<Move> bishop_moves(
        const Board& board,
        const Piece& piece,
        const Position& pos)
    {
        std::vector<Move> moves;
        auto diagonal_check = [&](int8_t x_dir, int8_t y_dir) {
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
                    moves.emplace_back(Move::normal(pos, to));
                    break;
                }
                else {
                    break;
                }
            }
        };
        
        // North East
        diagonal_check(1,-1);
        // North West
        diagonal_check(-1,-1);
        // South East
        diagonal_check(1,1);
        // South West
        diagonal_check(-1,1);

        return moves;
    }

    std::vector<Move> rook_moves(
        const Board& board,
        const Piece& piece,
        const Position& pos)
    {
        std::vector<Move> moves;
        auto diagonal_check = [&](int8_t x_dir, int8_t y_dir) {
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
                    moves.emplace_back(Move::normal(pos, to));
                    break;
                }
                else {
                    break;
                }
            }
        };
        
        // North
        diagonal_check(0,-1);
        // South
        diagonal_check(0,1);
        // East
        diagonal_check(1,0);
        // West
        diagonal_check(-1,0);

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