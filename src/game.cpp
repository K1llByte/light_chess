#include "game.hpp"

#include "piece_moves.hpp"

bool contains_move(
    const std::vector<lc::Move>& moves,
    const lc::Position& to)
{
    for(const auto& move : moves) {
        if(to == move.to()) {
            return true;
        }
    }
    return false;
}

namespace lc {
    ChessGame::ChessGame(const Board& _board)
        : board(_board)
    {
        // Average 2000-2800 elo games duration
        move_history.reserve(40);
    }

    bool ChessGame::move(const Position& from, const Position& to) {
        // TODO: Should be static
        static const int8_t info_bits[2][3] = {
            { 
                WHITE_LONG_ROOK_MOVED_BIT , 
                WHITE_KING_MOVED_BIT      , 
                WHITE_SHORT_ROOK_MOVED_BIT
            },
            { 
                BLACK_LONG_ROOK_MOVED_BIT , 
                BLACK_KING_MOVED_BIT      , 
                BLACK_SHORT_ROOK_MOVED_BIT 
            }
        };

        const Piece piece = board.at(from);
        // TODO: check turn color
        switch (piece.kind()) {
            case PAWN: {
                auto possible_moves = pawn_moves(board, piece, from);
                return contains_move(possible_moves, to);
            }
            case KNIGHT: {
                auto possible_moves = knight_moves(board, piece, from);
                return contains_move(possible_moves, to);
            }
            case BISHOP: {
                auto possible_moves = bishop_moves(board, piece, from);
                return contains_move(possible_moves, to);
            }
            case ROOK: {
                auto possible_moves = rook_moves(board, piece, from);
                return contains_move(possible_moves, to);
            }
            case QUEEN: {
                auto possible_moves = queen_moves(board, piece, from);
                return contains_move(possible_moves, to);
            }
            case KING: {
                auto possible_moves = king_moves(board, piece, from);
                return contains_move(possible_moves, to);
            }
        }
        // In case piece is None or move is invalid
        return false;
    }
}