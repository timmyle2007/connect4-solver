#ifndef BOT_H
#define BOT_H
#include <string>
#include "game.h"
#include "table.h"
#include <cstdint>
namespace C4{
    class Bot{
    public:
        int play(std::string game_code);
        void reset(){ 
            table.reset();
        }   
    private:
        int negamax(const Game &game, int alpha, int beta);
        int iterative_deepening(const Game &game);
        TranspositionTable table;
    };
}



#endif