#include "game.hpp"

#include "piece_moves.hpp"

std::optional<lc::Move> get_move(
    const std::vector<lc::Move>& moves,
    const lc::Position& to)
{
    for(const auto& move : moves) {
        if(to == move.to()) {
            return move;
        }
    }
    return std::nullopt;
}

void apply_move(lc::Board& board, const std::optional<lc::Move>& move_opt) {
    // if(move_opt.has_value()) {
    //     move_opt->
    // }

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
                
                auto last_move = move_history.empty()
                    	? std::nullopt
                        : static_cast<std::optional<Move>>(move_history.back());
                auto possible_moves = pawn_moves(board, piece, from, last_move);
                auto move_opt = get_move(possible_moves, to);
                apply_move(board, move_opt);

                return move_opt.has_value();
            }
            case KNIGHT: {
                auto possible_moves = knight_moves(board, piece, from);
                auto move_opt = get_move(possible_moves, to);
                apply_move(board, move_opt);

                return move_opt.has_value();
            }
            case BISHOP: {
                auto possible_moves = bishop_moves(board, piece, from);
                auto move_opt = get_move(possible_moves, to);
                apply_move(board, move_opt);

                return move_opt.has_value();
            }
            case ROOK: {
                auto possible_moves = rook_moves(board, piece, from);
                auto move_opt = get_move(possible_moves, to);
                apply_move(board, move_opt);

                return move_opt.has_value();
            }
            case QUEEN: {
                auto possible_moves = queen_moves(board, piece, from);
                auto move_opt = get_move(possible_moves, to);
                apply_move(board, move_opt);

                return move_opt.has_value();
            }
            case KING: {
                auto possible_moves = king_moves(board, piece, from);
                auto move_opt = get_move(possible_moves, to);
                apply_move(board, move_opt);

                return move_opt.has_value();
            }
        }
        // In case piece is None or move is invalid
        fmt::print("ENd\n");
        return false;
    }
}