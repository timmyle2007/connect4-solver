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
        void increment_nodes_count(){
            NODES_COUNT++;
        }
        long long get_nodes_count()const{
            return NODES_COUNT;
        }
    private:
        int negamax(const Game &game, int alpha, int beta);
        int iterative_deepening(const Game &game);
        long long NODES_COUNT = 0;
        TranspositionTable table;
    };
}



#endif