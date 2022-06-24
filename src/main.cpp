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

int main() {
    using namespace lc;
    // auto board = Board::standard();
    auto board = Board({
        0x0c0a0b0e0d0b0a0c,
        0x0909090909090909,
        0x0000000000000000,
        0x0000000100000000,
        0x0000000000000000,
        0x0000000000000000,
        0x0101010001010101,
        0x0402030605030204
    });

    auto game = ChessGame(board);
    
    // Black pawn first double move
    game.move({5,1}, {5,3});
    // White pawn En passant
    game.move({4,3}, {5,2});

    board_render(game.board);


    // TODO: Change Move::Rook to Move::Castle!!!!
    
}