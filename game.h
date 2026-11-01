#ifndef GAME_H
#define GAME_H
#include <string>
#include "board.h"
#include <stdexcept>

namespace C4{
    class Game{

    public:
        Game(std::string game_code){
            current_player_id = 1;
            for(const auto &c: game_code){

                if(c < '1' || c > '7'){
                    throw std::invalid_argument("Invalid character in game code. Must be a digit from 1-7 only");
                }

                int col = static_cast<int>(c) - static_cast<int>('1');

                if(!is_valid_move(board, col)){
                    throw std::invalid_argument("Game code contains invalid move");
                }

                if(is_winning_move(board, current_player, col)){
                    throw std::invalid_argument("Game code already contains winning move");
                }

                play_move(col);
                current_player_id = 3 - current_player_id;

            }
        }   

        void play_move(int col){
            current_player = board ^ current_player;
            move(board, col);
            moves_count++;
            current_player_id = 3 - current_player_id;
        }

        int get_moves_count()const{
            return moves_count;
        }


        BitBoard get_board()const{
            return board;
        }

        BitBoard get_current_player()const{
            return current_player;
        }

        std::uint64_t get_game_key()const{
            return board + current_player;
        }

    private:
        int moves_count = 0;
        BitBoard board = 0;
        BitBoard current_player = 0;
        int current_player_id;
    };

}
#endif