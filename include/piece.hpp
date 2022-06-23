#pragma once

#define NONE   0
#define PAWN   1
#define KNIGHT 2
#define BISHOP 3
#define ROOK   4
#define QUEEN  5
#define KING   6

#define WHITE 0b0000
#define BLACK 0b1000

#define COLOR_MASK 0b1000
#define VALUE_MASK 0b0111


namespace lc {
    class Piece {
        private:
        uint8_t data;

        public:
        constexpr Piece(uint8_t _data)
            : data(_data) {}

        constexpr uint8_t color() const { return data & COLOR_MASK; }
        constexpr uint8_t kind() const { return data & VALUE_MASK; }
        constexpr uint8_t raw() const { return data; }

        constexpr bool is_white() const { return color() == WHITE; }
        constexpr bool is_black() const { return color() == BLACK; }
    };

    using Color = uint8_t;
    constexpr Piece pawn(Color c)   { return PAWN   | c; }
    constexpr Piece knight(Color c) { return KNIGHT | c; }
    constexpr Piece bishop(Color c) { return BISHOP | c; }
    constexpr Piece rook(Color c)   { return ROOK   | c; }
    constexpr Piece queen(Color c)  { return QUEEN  | c; }
    constexpr Piece king(Color c)   { return KING   | c; }
}