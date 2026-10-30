// #include <utility>
// #include "board.h"
// #include <iostream>





// Board::Board(): active(0){}

// Board::Board(BitBoard b): active(b){}

// bool Board::is_valid_move(int col)const{
//     return !(active & top_of_column(col)); 
// } 

// void Board::play_move(int col){
//     active |= (active + bottom_of_column(col));
// }

// bool Board::is_winning_move(int col, const Board &all)const{

//     BitBoard new_column = (all.active + bottom_of_column(col)) & column_mask(col);
//     BitBoard new_active = active | new_column;
//     //VERTICAL
//     if(new_active & (new_active >> 1) & (new_active >> 2) & (new_active >> 3))return true;  
//     //HORIZONTAL
//     if(new_active & (new_active >> (HEIGHT + 1)) & (new_active >> (2 * (HEIGHT + 1))) & (new_active >> (3 * (HEIGHT + 1))))return true;
//     //TOP LEFT -> BOTTOM RIGHT
//     if(new_active & (new_active >> HEIGHT) & (new_active >> (2 * HEIGHT)) & (new_active >> (3 * HEIGHT)))return true;
//     //TOP RIGHT -> BOTTOM LEFT
//     if(new_active & (new_active >> (HEIGHT + 2)) & (new_active >> (2 * (HEIGHT + 2)) & (new_active >> (3 * (HEIGHT + 2)))))return true;

//     return false;

// }

// bool Board::is_on(int row, int col)const{
//     return (active >> (col * (HEIGHT + 1) + row)) & 1;
// }

// Board operator ^(const Board &a, const Board &b){
//     return Board(a.active ^ b.active);
// }

// void Board::print()const{
//     for(int i = 0; i < WIDTH; i++){
//         for(int j = 0; j < HEIGHT; j++){
//             std::cout << is_on(i, j);
//         }
//         std::cout << "\n";
//     }
// }

