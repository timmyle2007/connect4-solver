#include "bot.h"
#include <iostream>
#include <algorithm>

using namespace C4;

constexpr int ORIG_MOVE_ORDER[7] = {3, 2, 4, 1, 5, 0, 6};
constexpr int MIN_SCORE = -18;
constexpr int MAX_SCORE = 18;

struct Move_Data{
    int col, score;
};


int generate_move_order(const Game &game, BitBoard playable_mask, Move_Data current_move_order[7], int first_move){
    int index = 0;
    BitBoard player_board = game.get_current_player(), game_board = game.get_board();

    auto get_score = [player_board, game_board, playable_mask](int col) -> int{
        BitBoard new_cell = playable_mask & column_mask(col);
        return __builtin_popcountll(winning_moves(player_board | new_cell, game_board | new_cell));
    };

    for(const auto &col: ORIG_MOVE_ORDER){
        if((playable_mask & column_mask(col))){
            current_move_order[index] = {col, get_score(col) + 1000 * (col == first_move)};
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
    increment_nodes_count();
    if(game.get_moves_count() == WIDTH * HEIGHT)return 0;

    if(can_win_next_move(game.get_current_player(), game.get_board())){
        return (WIDTH * HEIGHT - game.get_moves_count() + 1) / 2;
    }

    beta = std::min(beta, (WIDTH * HEIGHT - game.get_moves_count() - 1) / 2);
    if(alpha >= beta)return beta;

    std::uint64_t key = game.get_game_key();
    std::uint64_t mirror_key = mirror(key);
    bool flipped = (mirror_key < key);

    auto [score, first_move] = table.get(std::min(mirror_key, key));
    if(flipped)first_move = 6 - first_move;
    
    if(score){
        if(score < MAX_SCORE - MIN_SCORE + 1){
            beta = std::min(beta, MIN_SCORE + score);
            if(alpha >= beta)return beta;
        }else{
            alpha = std::max(alpha, MIN_SCORE + score - (MAX_SCORE - MIN_SCORE + 1));
            if(alpha >= beta)return alpha;
        }
    }else{
        first_move = -1;
    }


    BitBoard playable_mask = moves_to_play(game.get_current_player(), game.get_board());
    if(playable_mask == 0){
        return -(WIDTH * HEIGHT - game.get_moves_count()) / 2;
    }

    Move_Data current_move_order[7];
    int no_moves = generate_move_order(game, playable_mask, current_move_order, first_move);
    int best_col = current_move_order[0].col;

    for(int i = 0; i < no_moves; i++){

        int col = current_move_order[i].col;

        Game new_game(game);
        new_game.play_move(col);

        std::uint64_t new_game_key = new_game.get_game_key();
        table.prefetch(std::min(new_game_key, mirror_key));
        
        int new_score = -negamax(new_game, -beta, -alpha);

        if(new_score >= beta){
            if(!flipped){
                table.update(key, MAX_SCORE - MIN_SCORE + 1 + new_score - MIN_SCORE, col);
            }else{
                table.update(mirror_key, MAX_SCORE - MIN_SCORE + 1 + new_score - MIN_SCORE, 6 - col);
            }
            return new_score;
        }

        if(new_score > alpha){
            best_col = col;
            alpha = new_score;
        }

    }
    
    if(!flipped){
        table.update(key, alpha - MIN_SCORE, best_col);
    }else{
        table.update(mirror_key, alpha - MIN_SCORE, 6 - best_col);
    }
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



