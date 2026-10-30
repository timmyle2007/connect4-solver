#ifndef GAME_H
#define GAME_H
#include <string>
#include "board.h"
class Game{

public:
    Game(std::string game_code);

    void play_move(int col);

    int get_moves_count()const;


    BitBoard get_board()const{
        return board;
    }

    BitBoard get_current_player()const{
        return current_player;
    }

private:
    int moves_count = 0;
    BitBoard board = 0;
    BitBoard current_player = 0;
    int current_player_id;
};



#endif