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
    if(move_opt.has_value()) {
        auto move = *move_opt;
        move.visit(
            [&](lc::Move::Normal arg) {
                fmt::print("{},{}\n", move.to()[0], move.to()[1]);
                board.set(move.to(), board.at(move.from()));
                board.set(move.from(), arg.capture);
                fmt::print("Normal\n");
            },
            [&](lc::Move::Promotion arg) {
                board.set(move.to(), arg.to);
                board.set(move.from(), arg.capture);
                fmt::print("Promotion\n");
            },
            [&](lc::Move::Rook arg) {
                board.set(move.to(), board.at(move.from()));
                board.set(move.from(), NONE);
                // White kingside castling
                // White queenside castling
                // Black kingside castling
                // Black queenside castling
                if(move.to() == lc::Position{6,7}) {
                    const lc::Position rook_pos = {7,7};
                    board.set(lc::Position{5,7}, board.at(rook_pos));
                    board.set(rook_pos, NONE);
                }
                else if(move.to() == lc::Position{6,7}) {
                    const lc::Position rook_pos = {0,7};
                    board.set(lc::Position{3,7}, board.at(rook_pos));
                    board.set(rook_pos, NONE);
                }
                else if(move.to() == lc::Position{6,7}) {
                    const lc::Position rook_pos = {7,0};
                    board.set(lc::Position{5,0}, board.at(rook_pos));
                    board.set(rook_pos, NONE);
                }
                else if(move.to() == lc::Position{6,7}) {
                    const lc::Position rook_pos = {0,0};
                    board.set(lc::Position{3,0}, board.at(rook_pos));
                    board.set(rook_pos, NONE);
                }
                fmt::print("Castling\n");
            },
            [&](lc::Move::EnPassant arg) {
                // Right En Passant
                board.set(move.to(), board.at(move.from()));
                board.set(move.from(), NONE);
                board.set({move.to()[0],move.from()[1]}, NONE);
                fmt::print("En Passant\n");
            }
        );
    }
    else {
        fmt::print("There's no move\n");
    }
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

        std::optional<Move> move_opt = std::nullopt;
        const Piece piece = board.at(from);
        // TODO: check turn color
        // In case piece is None or move is invalid
        switch (piece.kind()) {
            case PAWN: {
                // Retrieve last move, needed to check for~
                // en passant move possibility
                auto last_move = move_history.empty()
                    	? std::nullopt
                        : static_cast<std::optional<Move>>(move_history.back());
                // Retrieve pawn possible moveset
                auto possible_moves = pawn_moves(board, piece, from, last_move);
                move_opt = get_move(possible_moves, to);
                break;
            }
            case KNIGHT: {
                // Retrieve knight possible moveset
                auto possible_moves = knight_moves(board, piece, from);
                move_opt = get_move(possible_moves, to);
                break;
            }
            case BISHOP: {
                // Retrieve bishop possible moveset
                auto possible_moves = bishop_moves(board, piece, from);
                move_opt = get_move(possible_moves, to);
                break;
            }
            case ROOK: {
                // Retrieve rook possible moveset
                auto possible_moves = rook_moves(board, piece, from);
                move_opt = get_move(possible_moves, to);
                break;
            }
            case QUEEN: {
                // Retrieve queen possible moveset
                auto possible_moves = queen_moves(board, piece, from);
                move_opt = get_move(possible_moves, to);
                break;
            }
            case KING: {
                auto possible_moves = king_moves(board, piece, from);
                move_opt = get_move(possible_moves, to);
                break;
            }
        }

        if(move_opt.has_value()) {
            auto move = *move_opt;
            // Apply move if exists, to board
            apply_move(board, move);
            // Add move to move history
            move_history.push_back(move);
        }
        return move_opt.has_value();
    }
}