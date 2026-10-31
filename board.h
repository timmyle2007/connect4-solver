#ifndef BOARD_H
#define BOARD_H
#include <cstdint>

namespace C4{
    constexpr int WIDTH = 7;
    constexpr int HEIGHT = 6;

    using BitBoard = std::uint64_t;

    constexpr BitBoard top_of_column(int col){
        return 1ULL << ((col + 1) * (HEIGHT + 1) - 2);
    }

    constexpr BitBoard bottom_of_column(int col){
        return 1ULL << (col * (HEIGHT + 1));
    }

    constexpr BitBoard column_mask(int col){
        return ((1ULL << (HEIGHT)) - 1) << (col * (HEIGHT + 1));    
    }

    constexpr BitBoard calculate_bottom_mask(){
        BitBoard board = 0;
        for(int i = 0; i < WIDTH; i++){
            board |= bottom_of_column(i);
        }
        return board;
    }

    constexpr BitBoard calculate_board_mask(){
        BitBoard board = 0;
        for(int i = 0; i < WIDTH; i++){
            board |= column_mask(i);
        }
        return board;
    }

    inline constexpr BitBoard BOTTOM_MASK = calculate_bottom_mask();

    inline constexpr BitBoard BOARD_MASK = calculate_board_mask();

    constexpr bool is_valid_move(BitBoard board, int col){
        return !(board & top_of_column(col));
    }

    constexpr bool is_winning_move(BitBoard board, BitBoard player, int col){
        BitBoard new_column = (board + bottom_of_column(col)) & column_mask(col);
        BitBoard new_active = player | new_column;
        //VERTICAL
        if(new_active & (new_active >> 1) & (new_active >> 2) & (new_active >> 3))return true;  
        //HORIZONTAL
        if(new_active & (new_active >> (HEIGHT + 1)) & (new_active >> (2 * (HEIGHT + 1))) & (new_active >> (3 * (HEIGHT + 1))))return true;
        //TOP LEFT -> BOTTOM RIGHT
        if(new_active & (new_active >> HEIGHT) & (new_active >> (2 * HEIGHT)) & (new_active >> (3 * HEIGHT)))return true;
        //TOP RIGHT -> BOTTOM LEFT
        if(new_active & (new_active >> (HEIGHT + 2)) & (new_active >> (2 * (HEIGHT + 2)) & (new_active >> (3 * (HEIGHT + 2)))))return true;

        return false;
    }

    constexpr BitBoard possible_moves(const BitBoard &board){
        return (board + BOTTOM_MASK) & BOARD_MASK;
    }

    constexpr BitBoard winning_moves(const BitBoard &player_board, const BitBoard &whole_board){
        BitBoard positions = 0;

        //VERTICAL
        positions |= (player_board << 1) & (player_board << 2) & (player_board << 3);

        //HORIZONTAL
        //position 0
        positions |= (player_board >> (HEIGHT + 1)) & (player_board >> (2 * (HEIGHT + 1))) & (player_board >> (3 * (HEIGHT + 1)));
        //position 1
        positions |= (player_board << (HEIGHT + 1)) & (player_board >> (HEIGHT + 1)) & (player_board >> (2 * (HEIGHT + 1)));
        //position 2
        positions |= (player_board << (2 * (HEIGHT + 1))) & (player_board << (HEIGHT + 1)) & (player_board >> (HEIGHT + 1));
        //position 3
        positions |= (player_board << (3 * (HEIGHT + 1))) & (player_board << (2 * (HEIGHT + 1))) & (player_board << (HEIGHT + 1));
        
        //BOTTOM LEFT TO TOP RIGHT DIAGONAL
        //position 0
        positions |= (player_board >> (HEIGHT + 2)) & (player_board >> (2 * (HEIGHT + 2))) & (player_board >> (3 * (HEIGHT + 2)));
        //position 1
        positions |= (player_board << (HEIGHT + 2)) & (player_board >> (HEIGHT + 2)) & (player_board >> (2 * (HEIGHT + 2)));
        //position 2
        positions |= (player_board << (2 * (HEIGHT + 2))) & (player_board << (HEIGHT + 2)) & (player_board >> (HEIGHT + 2));
        //position 3
        positions |= (player_board << (3 * (HEIGHT + 2))) & (player_board << (2 * (HEIGHT + 2))) & (player_board << (HEIGHT + 2));

        //BOTTOM RIGHT TO TOP LEFT DIAGONAL
        //position 0
        positions |= (player_board >> (HEIGHT)) & (player_board >> (2 * (HEIGHT))) & (player_board >> (3 * (HEIGHT)));
        //position 1
        positions |= (player_board << (HEIGHT)) & (player_board >> (HEIGHT)) & (player_board >> (2 * (HEIGHT)));
        //position 2
        positions |= (player_board << (2 * (HEIGHT))) & (player_board << (HEIGHT)) & (player_board >> (HEIGHT));
        //position 3
        positions |= (player_board << (3 * (HEIGHT))) & (player_board << (2 * (HEIGHT))) & (player_board << (HEIGHT));

        return positions & (whole_board ^ BOARD_MASK);
    }

    constexpr bool can_win_next_move(const BitBoard &player_board, const BitBoard &whole_board){
        return (winning_moves(player_board, whole_board) & possible_moves(whole_board)) > 0;
    }

    constexpr BitBoard moves_to_play(const BitBoard &player_board, const BitBoard &whole_board){
        BitBoard possible = possible_moves(whole_board);
        BitBoard opponent_winning_moves = winning_moves(whole_board ^ player_board, whole_board);
        BitBoard next_winning_moves = (opponent_winning_moves & possible);
        if(next_winning_moves){
            if(__builtin_popcountll(next_winning_moves) == 1)return next_winning_moves & ~(opponent_winning_moves >> 1);
            else{
                return 0;
            }
        }   
        return possible & ~(opponent_winning_moves >> 1);
    }

    constexpr void move(BitBoard &board, int col){
        board |= board + bottom_of_column(col);
    }

    constexpr BitBoard mirror(BitBoard board){
        BitBoard result = 0;
        for(int i = 0; i < WIDTH; i++){
            result = (result << (HEIGHT + 1)) + (board & ((1 << (HEIGHT + 1)) - 1)); board >>= (HEIGHT + 1);
        }
        return result;
    }

}
#endif