#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <iomanip>
#include <iostream>
#include <chrono>
#include <cassert>
#include "bot.h"

int main(int argc, char* argv[]){
    std::string filename;
    if(argc == 1){
        filename = "TEST_L2_R1";
    }else{
        filename = argv[1];
    }
    std::ifstream in(filename);
    std::string line;

    Bot negamax_bot = Bot();

    double total = 0.0;

    int test_count = 0;

    while(std::getline(in, line)){
        std::stringstream ss(line);

        std::string token;

        std::string game_code;
        int answer;

        std::getline(ss, token, ' ');
        game_code = token;

        std::getline(ss, token);
        answer = std::stoi(token);

        auto t0 = std::chrono::steady_clock::now();
        


        int guess = negamax_bot.play(game_code);


        auto t1 = std::chrono::steady_clock::now();

        double ms = std::chrono::duration<double, std::milli>(t1 - t0).count();

        total += ms; test_count++;
        
        if(guess != answer){
            std::cout << "WRONG ANSWER" << "\n";
            std::cout << "Game Code: " << game_code << "\n";
            std::cout << "Expected: " << answer << "\n";
            std::cout << "Got: " << guess << "\n";
            return 1;
        }
    }

    std::cout << "SUCCESS" << "\n";
    std::cout << "Average time: " << total / test_count << "\n";
    return 0;
}