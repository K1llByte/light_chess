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
    auto board1 = Board::standard();
    // auto board2 = Board({
    //     0x0c0a0b0e0d0b0a0c,
    //     0x0909090909090909,
    //     0x0000000000000000,
    //     0x0000000000000000,
    //     0x0000000000000000,
    //     0x0000000000000000,
    //     0x0101010101010101,
    //     0x0402030605030204
    // });
    // auto board3 = Board::empty();

    // board1.set({3,3}, pawn(BLACK));

    auto game = ChessGame(board1);
    // e2 to e4
    fmt::print("{}\n", game.move({4,6}, {4,4}));

    board_render(board1);

    auto move1 = Move::en_passant({0,0},{0,0});
    move1.visit(
        [](Move::Normal arg){
            fmt::print("Normal\n");
        },
        [](auto arg){
            fmt::print("Otherwise\n");
        }
    );

    // TODO: Change Move::Rook to Move::Castle!!!!
    
}