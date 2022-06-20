#pragma once

#include "piece.hpp"

#include <array>
#include <cassert>

namespace lc {
    class Board {
        private:
        std::array<uint64_t,8> board_data;
        
        public:
        constexpr Board(const std::array<uint64_t,8>& data)
            : board_data{data} {}
        static constexpr Board standard();
        static constexpr Board empty();

        constexpr Piece at(size_t x, size_t y) const;

        constexpr bool operator==(const Board& other) {
            for(size_t i = 0; i < 8; ++i)
                if(board_data[i] != other.board_data[i])
                    return false;
            return true;
        }
    };
}

/////////////// Implementation ///////////////

namespace lc {
    constexpr Board Board::standard() {
        constexpr std::array<uint64_t,8> data = {
            0x0c0a0b0e0d0b0a0c,
            0x0909090909090909,
            0x0000000000000000,
            0x0000000000000000,
            0x0000000000000000,
            0x0000000000000000,
            0x0101010101010101,
            0x0402030605030204
        };
        return Board(data);
    }

    constexpr Board Board::empty() {
        constexpr std::array<uint64_t,8> data = {
            0x0000000000000000,
            0x0000000000000000,
            0x0000000000000000,
            0x0000000000000000,
            0x0000000000000000,
            0x0000000000000000,
            0x0000000000000000,
            0x0000000000000000
        };
        return Board(data);
    }

    constexpr Piece Board::at(size_t x, size_t y) const {
        // TODO: Bound check error handling
        assert(x < 8 && y < 8);
        return Piece((board_data[y] >> (x*8)) & 0xff);
    }
}