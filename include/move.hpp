#pragma once

#include <variant>

#include "board.hpp"

// Helper type for visitor
template<class... Ts> struct overloaded : Ts... { using Ts::operator()...; };
// Explicit deduction guide (not needed as of C++20)
template<class... Ts> overloaded(Ts...) -> overloaded<Ts...>;

namespace lc {
    class Move {
        public:
        // Can capture
        struct Normal {
            Piece capture;
        };
        // Can capture
        struct Promotion {
            Piece to;
            Piece capture;
        };
        struct Castling {};
        // Is capture (opponent pawn)
        struct EnPassant {};

        using MoveKind =
            std::variant<
                Normal,
                Promotion,
                Castling,
                EnPassant>;

        private:
        Position from_pos;
        Position to_pos;
        MoveKind kind;

        public:
        static constexpr Move normal(Position&& from, Position&& to, Piece capture = NONE);
        static constexpr Move promotion(Position&& from, Position&& to, const Piece& promotion, Piece capture = NONE);
        static constexpr Move castling(Position&& from, Position&& to);
        static constexpr Move en_passant(Position&& from, Position&& to);

        static constexpr Move normal(const Position& from, const Position& to, Piece capture = NONE);
        static constexpr Move promotion(const Position& from, const Position& to, const Piece& promotion, Piece capture = NONE);
        static constexpr Move castling(const Position& from, const Position& to);
        static constexpr Move en_passant(const Position& from, const Position& to);

        constexpr Position from() const { return from_pos; }
        constexpr Position to() const { return to_pos; }
        template<typename ...F>
        constexpr void visit(F&&...);

        private:
        constexpr Move(Position&& _from, Position&& _to, MoveKind&& _kind)
            : from_pos(std::move(_from))
            , to_pos(std::move(_to))
            , kind(std::move(_kind)) {}
        constexpr Move(const Position& _from, const Position& _to, MoveKind&& _kind)
            : from_pos(_from)
            , to_pos(_to)
            , kind(_kind) {}
    };
}

/////////////// Implementation ///////////////

namespace lc {
    constexpr Move Move::normal(Position&& from, Position&& to, Piece capture) {
        return Move(
            std::forward<Position>(from),
            std::forward<Position>(to),
            Move::Normal{capture}
        );
    }

    constexpr Move Move::promotion(Position&& from, Position&& to, const Piece& promotion, Piece capture) {
        return Move(
            std::forward<Position>(from),
            std::forward<Position>(to),
            Move::Promotion{promotion, capture}
        );
    }

    constexpr Move Move::castling(Position&& from, Position&& to) {
        return Move(
            std::forward<Position>(from),
            std::forward<Position>(to),
            Move::Castling{}
        );
    }

    constexpr Move Move::en_passant(Position&& from, Position&& to) {
        return Move(
            std::forward<Position>(from),
            std::forward<Position>(to),
            Move::EnPassant{}
        );
    }


    constexpr Move Move::normal(const Position& from, const Position& to, Piece capture) {
        return Move(
            from,
            to,
            Move::Normal{capture}
        );
    }

    constexpr Move Move::promotion(const Position& from, const Position& to, const Piece& promotion, Piece capture) {
        return Move(
            from,
            to,
            Move::Promotion{promotion, capture}
        );
    }

    constexpr Move Move::castling(const Position& from, const Position& to) {
        return Move(
            from,
            to,
            Move::Castling{}
        );
    }

    constexpr Move Move::en_passant(const Position& from, const Position& to) {
        return Move(
            from,
            to,
            Move::EnPassant{}
        );
    }

    template<typename ...F>
    constexpr void Move::visit(F&&... visitors) {
        std::visit(overloaded{ std::forward<F>(visitors)... }, kind);
    }
}