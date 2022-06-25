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

void apply_move(lc::Board& board, lc::Move& move) {
    move.visit(
        [&](lc::Move::Normal arg) {
            board.set(move.to(), board.at(move.from()));
            board.set(move.from(), NONE);
            fmt::print("Normal\n");
        },
        [&](lc::Move::Promotion arg) {
            board.set(move.to(), arg.to);
            board.set(move.from(), NONE);
            fmt::print("Promotion\n");
        },
        [&](lc::Move::Castling arg) {
            board.set(move.to(), board.at(move.from()));
            board.set(move.from(), NONE);
            // White kingside castling
            if(move.to() == lc::Position{6,7}) {
                const lc::Position rook_pos = {7,7};
                board.set(lc::Position{5,7}, board.at(rook_pos));
                board.set(rook_pos, NONE);
            }
            // White queenside castling
            else if(move.to() == lc::Position{2,7}) {
                const lc::Position rook_pos = {0,7};
                board.set(lc::Position{3,7}, board.at(rook_pos));
                board.set(rook_pos, NONE);
            }
            // Black kingside castling
            else if(move.to() == lc::Position{6,0}) {
                const lc::Position rook_pos = {7,0};
                board.set(lc::Position{5,0}, board.at(rook_pos));
                board.set(rook_pos, NONE);
            }
            // Black queenside castling
            else if(move.to() == lc::Position{2,0}) {
                const lc::Position rook_pos = {0,0};
                board.set(lc::Position{3,0}, board.at(rook_pos));
                board.set(rook_pos, NONE);
            }
            fmt::print("Castling\n");
        },
        [&](lc::Move::EnPassant arg) {
            board.set(move.to(), board.at(move.from()));
            board.set(move.from(), NONE);
            board.set({move.to()[0],move.from()[1]}, NONE);
            fmt::print("En Passant\n");
        }
    );
}

namespace lc {
    ChessGame::ChessGame(const Board& _board)
        : board(_board)
    {
        // Average 2000-2800 elo games duration
        move_history.reserve(40);
    }

    bool ChessGame::move(const Position& from, const Position& to) {
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
                // Retrieve king possible moveset
                auto possible_moves = king_moves(board, piece, from, state);
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
        else {
            fmt::print("Invalid move\n");
        }
        return move_opt.has_value();
    }

    std::vector<Move> ChessGame::piece_moveset(const Position& pos) const {
        const auto piece = board.at(pos);
        switch (piece.kind())
        {
            case PAWN: {
                auto last_move = move_history.empty()
                    ? std::nullopt
                    : static_cast<std::optional<Move>>(move_history.back());
                return pawn_moves(board, piece, pos, last_move);
            }
            case KNIGHT: {
                // Retrieve knight possible moveset
                return knight_moves(board, piece, pos);
            }
            case BISHOP: {
                // Retrieve bishop possible moveset
                return bishop_moves(board, piece, pos);
            }
            case ROOK: {
                // Retrieve rook possible moveset
                return rook_moves(board, piece, pos);
            }
            case QUEEN: {
                // Retrieve queen possible moveset
                return queen_moves(board, piece, pos);
            }
            case KING: {
                // Retrieve king possible moveset
                return king_moves(board, piece, pos, state);
            }
        }
        return {};
    }
}