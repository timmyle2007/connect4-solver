//Unit tests for bitboard
#include "doctest.h"
#include "board.h"
#include <random>
using namespace C4;

static constexpr BitBoard cell(int row, int col) {
    return 1ULL << (col * (HEIGHT + 1) + row);
}

TEST_CASE("possible moves: empty board returns bottom row"){
    BitBoard board = 0;
    CHECK(possible_moves(board) == BOTTOM_MASK);
}

TEST_CASE("possible moves: full board returns 0"){
    BitBoard board = 0;
    for(int i = 0; i < WIDTH; i++)board |= column_mask(i);
    CHECK(possible_moves(board) == 0);
}

TEST_CASE("possible moves: full column is not included in possible moves"){
    BitBoard board = 0;
    board |= column_mask(3);
    CHECK(possible_moves(board) == (BOTTOM_MASK ^ cell(0, 3)));
}

TEST_CASE("possible moves: correct answer for non-zero and non-full columns"){
    BitBoard board =  cell(0, 0)
                    | cell(0, 1) | cell(1, 1)
                    | cell(0, 2) | cell(1, 2) | cell(2, 2)
                    | cell(0, 3) | cell(1, 3) | cell(2, 3) | cell(3, 3);
                    
    CHECK(possible_moves(board) == (cell(1, 0) | cell(2, 1) | cell(3, 2) | cell(4, 3) | cell(0, 4) | cell(0, 5) | cell(0, 6)));
}

TEST_CASE("winning_moves: horizontal three") {
    //  . X X X . . . row 0
    BitBoard player  = cell(0, 1) | cell(0, 2) | cell(0, 3);
    BitBoard all = player; 
    BitBoard answer = cell(0, 0) | cell(0, 4); 
    CHECK(winning_moves(player, all) == answer);
}

TEST_CASE("winning_moves: vertical three"){
    //. . . X . . . row 2
    //. . . X . . . row 1
    //. . . X . . . row 0
    BitBoard player = cell(0, 3) 
                    | cell(1, 3) 
                    | cell(2, 3);
    BitBoard all = player;
    BitBoard answer = cell(3, 3);   // directly above the three
    CHECK(winning_moves(player, all) == answer);

}

TEST_CASE("winning_moves: bottom-left -> top-right diagonal three"){
    //. . . . X X .    row 3
    //. . . X O O .    row 2
    //. . X O O X .    row 1
    //. . O X O O .    row 0
    BitBoard player = cell(3, 4) | cell(3, 5)
                    | cell(2, 3)
                    | cell(1, 2) | cell(1, 5)
                    | cell(0, 3);
    BitBoard opp    = cell(2, 4) | cell(2, 5)
                    | cell(1, 3) | cell(1, 4)
                    | cell(0, 2) | cell(0, 4) | cell(0, 5);

    BitBoard all = player | opp;
    BitBoard answer = cell(0, 1) | cell(4, 5);
    CHECK(winning_moves(player, all) == answer);

}

TEST_CASE("winning_moves: bottom_right -> top_left diagonal three"){
    //. X . . . . .    row 3
    //. O X . . . .    row 2
    //. O O X . . .    row 1
    //. X X O . . .    row 0

    BitBoard player = cell(3, 1)
                    | cell(2, 2)
                    | cell(1, 3)
                    | cell(0, 1) | cell(0, 2);

    BitBoard opp    = cell(2, 1)
                    | cell(1, 1) | cell(1, 2)
                    | cell(0, 3);

    BitBoard all = player | opp;
    BitBoard answer = cell(4, 0) | cell(0, 4);
    CHECK(winning_moves(player, all) == answer);
}

TEST_CASE("winning_moves: Does not report cells outside the board (horizontal, diagonal)"){

    //. . . X . . .    row 2
    //. . . . X . .    row 1
    //X X X . . X .    row 0

    BitBoard player = cell(2, 3)
                    | cell(1, 4)
                    | cell(0, 0) | cell(0, 1) | cell(0, 2) | cell(0, 5);

    BitBoard all = player;
    BitBoard answer = cell(0, 3) | cell(3, 2);
    CHECK(winning_moves(player, all) == answer);

}

TEST_CASE("winning_moves: does not report cells outside the board (vertical)"){
    //. . . X . . .    row 5
    //. . . X . . .    row 4
    //. . . X . . .    row 3
    //. . . O . . .    row 2
    //. . . X . . .    row 1
    //. . . O . . .    row 0
    BitBoard player = cell(5, 3) | cell(4, 3) | cell(3, 3) | cell(1, 3);
    BitBoard opp    = cell(2, 3) | cell(0, 3);
    BitBoard all = player | opp;
    CHECK(winning_moves(player, all) == 0);
}


static constexpr std::uint64_t get_col(BitBoard board, int col){
    BitBoard column = board & column_mask(col);
    return (column >> (col * (HEIGHT + 1)));
}
TEST_CASE("mirror: Correctness"){
    std::mt19937_64 rng(261207);
    for(int tc = 0; tc < 100; tc++){

        BitBoard board = rng() & BOARD_MASK;
        BitBoard mirror_board = mirror(board);

        for(int col = 0; col < 7; col++){
            CHECK(get_col(board, col) == get_col(mirror_board, 6 - col));
        }

    }
}

TEST_CASE("move: Updates board correctly"){
    BitBoard board = 0;

    move(board, 0);
    CHECK(board == cell(0, 0));

    move(board, 3);
    CHECK(board == (cell(0, 0) | cell(0, 3)));

    move(board, 0);
    CHECK(board == (cell(0, 0) | cell(1, 0) | cell(0, 3)));
}

TEST_CASE("can_win_next_move: Correctly identifies winning next moves"){
    {
        //. . . . . . . row 2
        //. . . . . . . row 1
        //. X . X X . . row 0
        BitBoard player = cell(0, 1) | cell(0, 3) | cell(0, 4);
        BitBoard all = player;
        CHECK(can_win_next_move(player, all));
    }
    {
        //. . . . . . . row 3
        //. . . . . . . row 2
        //. X X X . . . row 1
        //. X O O X . . row 0
        BitBoard player = cell(1, 1) | cell(1, 2) | cell(1, 3)
                        | cell(0, 1) | cell(0, 4);
        BitBoard opp    = cell(0, 2) | cell(0, 3);
        BitBoard all = player | opp;
        CHECK(can_win_next_move(player, all));
    }
    {
        //. . . . . . . row 3
        //. . X . . . . row 2
        //. . X . . . . row 1
        //. O X O . . . row 0
        BitBoard player = cell(2, 2)
                        | cell(1, 2)
                        | cell(0, 2);
        BitBoard opp    = cell(0, 1) | cell(0, 3);
        BitBoard all = player | opp;
        CHECK(can_win_next_move(player, all));
    }
    {
        //. . X . . . . row 3
        //. . O X . . . row 2
        //. . O O . . . row 1
        //. . O O O X . row 0
        BitBoard player = cell(3, 2)
                        | cell(2, 3)
                        | cell(0, 5);
        BitBoard opp    = cell(2, 2)
                        | cell(1, 2) | cell(1, 3)
                        | cell(0, 2) | cell(0, 3) | cell(0, 4);
        BitBoard all = player | opp;
        CHECK(can_win_next_move(player, all));
    }
    {
        //. . . . X . . row 3
        //. . . . O . . row 2
        //. . X O O . . row 1
        //. X O O O . . row 0
        BitBoard player = cell(3, 4)
                        | cell(1, 2)
                        | cell(0, 1);
        BitBoard opp    = cell(2, 4)
                        | cell(1, 3) | cell(1, 4)
                        | cell(0, 2) | cell(0, 3) | cell(0, 4);
        BitBoard all = player | opp;
        CHECK(can_win_next_move(player, all));
    }
}

TEST_CASE("can_win_next_move: Correctly rejects non-winning positions"){
    {
        //. . . . . . . row 3
        //. . . . . . . row 2
        //X O X X . . . row 1
        //X X O O X X . row 0
        BitBoard player = cell(1, 0) | cell(1, 2) | cell(1, 3)
                        | cell(0, 0) | cell(0, 1) | cell(0, 4) | cell(0, 5);
        BitBoard opp    = cell(1, 1)
                        | cell(0, 2) | cell(0, 3);
        BitBoard all = player | opp;
        CHECK_FALSE(can_win_next_move(player, all));
    }
    {
        //. . . . X . . row 3
        //. . . . O . . row 2
        //. . X . O . . row 1
        //. X O . O . . row 0
        BitBoard player = cell(3, 4)
                        | cell(1, 2)
                        | cell(0, 1);
        BitBoard opp    = cell(2, 4)
                        | cell(1, 4)
                        | cell(0, 2) | cell(0, 4);
        BitBoard all = player | opp;
        CHECK_FALSE(can_win_next_move(player, all));
    }
    {
        //. . . . . . . row 3
        //. X X X . . . row 2
        //. X X O . . . row 1
        //. O O X O . . row 0
        BitBoard player = cell(2, 1) | cell(2, 2) | cell(2, 3)
                        | cell(1, 1) | cell(1, 2)
                        | cell(0, 3);
        BitBoard opp    = cell(1, 3)
                        | cell(0, 1) | cell(0, 2) | cell(0, 4);
        BitBoard all = player | opp;
        CHECK_FALSE(can_win_next_move(player, all));
    }
}

TEST_CASE("moves_to_play: No threats"){
    //. . . . . . . row 2
    //. . X O . . . row 1
    //. X O X O . . row 0
    BitBoard player = cell(1, 2) 
                    | cell(0, 1) | cell(0, 3);
    BitBoard opp    = cell(1, 3) 
                    | cell(0, 2) | cell(0, 4);
    BitBoard all = player | opp;
    BitBoard answer = cell(0, 0) | cell(1, 1) | cell(2, 2) | cell(2, 3) | cell(1, 4) | cell(0, 5) | cell(0, 6);
    CHECK(moves_to_play(player, all) == answer);
}

TEST_CASE("moves_to_play: One threat"){
    //. . . . . . . row 2
    //. . X X . . . row 1
    //. O O O X . . row 0
    BitBoard player = cell(1, 2) | cell(1, 3) 
                    | cell(0, 4);
    BitBoard opp    = cell(0, 1) | cell(0, 2) | cell(0, 3);
    BitBoard all = player | opp;
    BitBoard answer = cell(0, 0);
    CHECK(moves_to_play(player, all) == answer);
}

TEST_CASE("moves_to_play: Two threats"){
    //. . . . . . . row 2
    //. . X X . . . row 1
    //. O O O . X . row 0
    BitBoard player = cell(1, 2) | cell(1, 3) | cell(0, 5);
    BitBoard opp    = cell(0, 1) | cell(0, 2) | cell(0, 3);
    BitBoard all = player | opp;
    BitBoard answer = 0;
    CHECK(moves_to_play(player, all) == answer);
}

TEST_CASE("moves_to_play: Does not include moves that make the opponent win afterward"){
    //. . . . . . . row 2
    //. . O O O . . row 1
    //. . X O X . . row 0
    BitBoard player = cell(0, 2) | cell(0, 4);
    BitBoard opp    = cell(1, 2) | cell(1, 3) | cell(1, 4)
                    | cell(0, 3);
    BitBoard all = player | opp;
    BitBoard answer = cell(0, 0) | cell(2, 2) | cell(2, 3) | cell(2, 4) | cell(0, 6);
    CHECK(moves_to_play(player, all) == answer);  
}

TEST_CASE("moves_to_play: Returns zero when the only block is itself under another opponent winning cell"){
    //. . . . . . . row 2
    //. X O O O . . row 1
    //X X O O O . . row 0
    BitBoard player = cell(1, 1)
                    | cell(0, 0) | cell(0, 1);
    BitBoard opp    = cell(1, 2) | cell(1, 3) | cell(1, 4)
                    | cell(0, 2) | cell(0, 3) | cell(0, 4);
    BitBoard all = player | opp;
    BitBoard answer = 0;
    CHECK(moves_to_play(player, all) == answer);
}
