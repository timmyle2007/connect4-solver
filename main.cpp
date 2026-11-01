#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <iomanip>
#include <iostream>
#include <chrono>
#include <cassert>
#include "bot.h"

using namespace C4;

int main(int argc, char* argv[]){
    if(argc != 2){
        std::cerr << "Usage: " << argv[0] << " <test_file>" << "\n";
        return 2;
    }

    std::ifstream in(argv[1]);
    if(!in){
        std::cerr << "Test file does not exist";
        return 2;
    }

    Bot bot;
    double total_ms = 0;
    int total_positions = 0;

    std::string line;
    while(getline(in, line)){
        std::stringstream ss(line);

        std::string game_code;
        int expected_answer;
        if(!(ss >> game_code >> expected_answer)){
            std::cerr << "Invalid format in test files";
            return 2;
        }

        bot.reset();

        auto t0 = std::chrono::steady_clock::now();
        int bot_guess = bot.play(game_code);
        auto t1 = std::chrono::steady_clock::now();

        total_ms += std::chrono::duration<double, std::milli>(t1 - t0).count();
        total_positions++;
        
        if(expected_answer != bot_guess){
            std::cerr << "WRONG ANSWER" << "\n";
            std::cerr << "Game Code: " << game_code << "\n";
            std::cerr << "Expected: " << expected_answer << "\n";
            std::cerr << "Got: " << bot_guess << "\n";
            return 1;
        }
    }

    std::cout << "SUCCESS" << "\n";
    std::cout << "Mean ms per position: " << static_cast<double>(total_ms) / total_positions << "\n";
    std::cout << "Mean nodes explored per position: " << static_cast<double>(bot.get_nodes_count()) / total_positions << "\n";
    std::cout << "Mean ns per node: " << static_cast<double>(total_ms * 1000000) / static_cast<double>(bot.get_nodes_count()) << "\n";

    return 0;
}