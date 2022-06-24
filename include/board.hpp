#pragma once

#include "piece.hpp"

#include <array>
#include <cassert>

namespace lc {
    using Position = std::array<uint8_t,2>;

    class Board {
        public:
        std::array<uint64_t,8> board_data;
        
        public:
        constexpr Board(const std::array<uint64_t,8>& data)
            : board_data{data} {}
        static constexpr Board standard();
        static constexpr Board empty();

        constexpr Piece at(const Position&) const;
        constexpr void set(const Position&, const Piece&);

        constexpr bool operator==(const Board& other) {
            for(size_t i = 0; i < 8; ++i)
                if(board_data[i] != other.board_data[i])
                    return false;
            return true;
        }
    };
}

/////////////// Implementation ///////////////

#include <fmt/core.h>

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

    constexpr Piece Board::at(const Position& pos) const {
        // TODO: Bounds check error handling
        assert(pos[0] < 8 && pos[1] < 8);
        return Piece((board_data[pos[1]] >> (pos[0]*8)) & 0xff);
    }
    
    constexpr void Board::set(const Position& pos, const Piece& piece) {
        // TODO: Bounds check error handling
        assert(pos[0] < 8 && pos[1] < 8);
        // Impl 1: Faster
        uint64_t mask = ~(uint64_t(0xff) << (pos[0]*8));
        board_data[pos[1]] &= mask;
        board_data[pos[1]] |= (uint64_t(piece.raw()) << (pos[0]*8));
        
        // Impl 2:
        // auto sub = board_data[pos[1]] >> (pos[0]*8);
        // board_data[pos[1]] -= sub;
        // board_data[pos[1]] += (piece.raw() << (pos[0]*8));
    }
}