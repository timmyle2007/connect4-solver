#include "game.h"
#include <string>
#include <iostream>


Game::Game(std::string game_code){
    
    current_player_id = 1;
    for(const auto &c: game_code){

        int col = static_cast<int>(c) - static_cast<int>('1');

        play_move(col);

        current_player_id = 3 - current_player_id;

    }
}   

void Game::play_move(int col){
    current_player = board ^ current_player;
    move(board, col);
    moves_count++;
    current_player_id = 3 - current_player_id;
}

int Game::get_moves_count() const{
    return moves_count;
}

