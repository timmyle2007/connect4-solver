#include "bot.h"
#include <iostream>
#include <algorithm>

constexpr int ORIG_MOVE_ORDER[7] = {3, 2, 4, 1, 5, 0, 6};
constexpr int MIN_SCORE = -22;


struct Move_Data{
    int col, score;
};


int generate_move_order(const Game &game, BitBoard playable_mask, Move_Data current_move_order[7]){
    int index = 0;
    BitBoard player_board = game.get_current_player(), game_board = game.get_board();

    auto get_score = [player_board, game_board, playable_mask](int col) -> int{
        BitBoard new_cell = playable_mask & column_mask(col);
        return __builtin_popcountll(winning_moves(player_board | new_cell, game_board | new_cell));
    };


    for(const auto &col: ORIG_MOVE_ORDER){
        if((playable_mask & column_mask(col))){
            current_move_order[index] = {col, get_score(col)};
            int ptr = index;
            while(ptr > 0 && current_move_order[ptr].score > current_move_order[ptr - 1].score){
                std::swap(current_move_order[ptr], current_move_order[ptr - 1]);
                ptr--;
            }
            index++;
        }
    }
    return index;
}

int Bot::negamax(const Game &game, int alpha, int beta){

    std::uint64_t key = game.get_board() + game.get_current_player();

    int value = table.get(key);

    if(value){
        beta = std::min(beta, MIN_SCORE + value);
    }

    if(alpha >= beta)return beta;

    if(game.get_moves_count() == WIDTH * HEIGHT)return 0;

    if(can_win_next_move(game.get_current_player(), game.get_board())){
        return (WIDTH * HEIGHT - game.get_moves_count() + 1) / 2;
    }

    beta = std::min(beta, (WIDTH * HEIGHT - game.get_moves_count() - 1) / 2);

    if(alpha >= beta)return beta;

    BitBoard playable_mask = moves_to_play(game.get_current_player(), game.get_board());
    
    if(playable_mask == 0){
        return -(WIDTH * HEIGHT - game.get_moves_count()) / 2;
    }
    
    Move_Data current_move_order[7];

    int no_moves = generate_move_order(game, playable_mask, current_move_order);

    for(int i = 0; i < no_moves; i++){

        int col = current_move_order[i].col;
        Game new_game(game);
        new_game.play_move(col);
        int new_score = -negamax(new_game, -beta, -alpha);

        if(new_score >= beta)return(new_score);

        if(new_score > alpha){
            alpha = new_score;
        }


    }

    table.update(key, alpha - MIN_SCORE);

    return alpha;

}


int Bot::iterative_deepening(const Game &game){
    int min =  -(WIDTH * HEIGHT - game.get_moves_count() + 1) / 2, max = (WIDTH * HEIGHT - game.get_moves_count() + 1) / 2;
    while(min < max){
        int mid = (min + max) >> 1;

        if(mid >= 0 && max / 2 > mid)mid = max / 2;
        else if(mid <= 0 && min / 2 < mid)mid = min / 2;

        int res = negamax(game, mid, mid + 1);
        if(res <= mid)max = mid;
        else min = mid + 1;
    }
    return min;
}


int Bot::play(std::string game_code){

    Game game(game_code);
    if(static_cast<int>(game_code.size()) % 2 == 0){
        return Bot::iterative_deepening(game);
    }else{
        return Bot::iterative_deepening(game);
    }
}



