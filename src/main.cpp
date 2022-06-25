#include <fmt/core.h>

#include "board.hpp"
#include "game.hpp"

void board_render(const lc::Board& b) {
    static const char repr[7] = { ' ', 'p', 'k', 'b', 'R', 'Q', 'K' };
    for(uint8_t i = 0 ; i < 8 ; ++i)
    {
        fmt::print("   +---+---+---+---+---+---+---+---+\n");
        fmt::print(" {} ", 8 - i);
        for(uint8_t j = 0 ; j < 8 ; ++j)
        {
            const lc::Piece tmp = b.at({j,i});
            #ifdef NUMBERS_REPRESENTATION
                if(!tmp)
                    fmt::print("| {} ", int(tmp));
                else if(tmp != 9)
                    if(tmp.color())
                        fmt::print("|\033[34m{}\033[0m ", int(tmp));
                    else
                        fmt::print("| \033[31m{}\033[0m ", int(tmp));
                else
                    fmt::print("| \033[34m{}\033[0m ", int(tmp));
            #else
                if(tmp.color())
                    fmt::print("| \033[34m{}\033[0m ", repr[tmp.kind()]);
                else
                    fmt::print("| \033[31m{}\033[0m ", repr[tmp.kind()]);

            #endif
                //"\033[31m \033[0m"
        }
        fmt::print("|\n");
        
    }
    fmt::print("   +---+---+---+---+---+---+---+---+\n     a   b   c   d   e   f   g   h\n");
}

void board_render_with_moveset(const lc::ChessGame& game, const lc::Position& pos) {
    const auto& b = game.board;
    static const char repr[7] = { ' ', 'p', 'k', 'b', 'R', 'Q', 'K' };
    auto moveset = game.piece_moveset(pos);
    for(uint8_t i = 0 ; i < 8 ; ++i)
    {
        fmt::print("   +---+---+---+---+---+---+---+---+\n");
        fmt::print(" {} ", 8 - i);

        for(uint8_t j = 0 ; j < 8 ; ++j)
        {
            const lc::Position current_pos = {j,i};
            const lc::Piece tmp = b.at(current_pos);

            bool is_move_position = false;
            for(const auto& move : moveset) {
                if(move.to() == current_pos) {
                    fmt::print("| \033[33mo\033[0m ");
                    is_move_position = true;
                }
            }

            if(!is_move_position) {
                if(tmp.color())
                    fmt::print("| \033[34m{}\033[0m ", repr[tmp.kind()]);
                else
                    fmt::print("| \033[31m{}\033[0m ", repr[tmp.kind()]);
            }
    }
        fmt::print("|\n");
    }
    fmt::print("   +---+---+---+---+---+---+---+---+\n     a   b   c   d   e   f   g   h\n");
}

int main() {
    using namespace lc;
    // auto board = Board::standard();
    // auto board = Board({
    //     0x0c0a0b0e0d0b0a0c,
    //     0x0909090909090909,
    //     0x0000000000000000,
    //     0x0000000100000000,
    //     0x0000000000000000,
    //     0x0000000000000000,
    //     0x0101010001010101,
    //     0x0402030605030204
    // });

    auto board = Board({
        0x0009000900000000,
        0x0000010000000000,
        0x0000000000000000,
        0x0000000000000909,
        0x0000000000000000,
        0x0000000000000000,
        0x0000000005000000,
        0x0400000600000004 // 0x0002030600000004
    });

    auto game = ChessGame(board);
    
    // game.move({4,7}, {2,7});

    board_render_with_moveset(game, {4,7});
    board_render(game.board);

    // auto moveset = game.piece_moveset({5,7});
    // for(const auto& move : moveset) {
    //     fmt::print("({},{})\n", move.to()[0], move.to()[1]);
    // }
}