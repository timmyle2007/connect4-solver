set -e
BUILD="g++ -std=c++17 -O2 -march=native -Wall -Wextra -Wconversion -I. -Itests"
$BUILD main.cpp bot.cpp -o program
$BUILD tests/main.cpp tests/test_board.cpp tests/test_solver.cpp tests/test_table.cpp bot.cpp -o tests_program
./tests_program 

#For harder test cases
#./tests_program -ns 