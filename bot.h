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
        int null_window_search(const Game &game);
        long long NODES_COUNT = 0;

        #ifndef BUCKET_TABLE
        TranspositionTable table;
        #else
        BucketTranspositionTable table;
        #endif
    };
}



#endif