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
    };
}