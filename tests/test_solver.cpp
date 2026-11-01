// Oracle tests: the solver against Pascal Pons' reference scores.
// Each line of a test file is "<moves> <score>", score from the side to move.
#include "doctest.h"
#include "bot.h"

#include <fstream>
#include <sstream>
#include <string>
#include <vector>

using namespace C4;

struct TestCase { std::string code; int score; };

static std::vector<TestCase> load(const std::string& path) {
    std::ifstream in(path);
    REQUIRE_MESSAGE(in.good(), "cannot open " << path);

    std::vector<TestCase> test_cases;
    std::string line;
    while (std::getline(in, line)) {
        std::istringstream ss(line);
        TestCase current_tc;
        if (ss >> current_tc.code >> current_tc.score) test_cases.push_back(current_tc);
    }

    REQUIRE(test_cases.size() == 1000);
    return test_cases;
}

static void check_file(const std::string& path) {
    Bot bot;
    for (const auto& c : load(path)) {
        bot.reset();   
        int guess = bot.play(c.code);
        CHECK_MESSAGE(guess == c.score, "WRONG ANSWER: position: " << c.code << " | Expected: " << c.score << " | Guess: " << guess);
    }
}

//Fast test cases
TEST_CASE("Test_L3_R1 (end-game, easy)")    { check_file("Test_L3_R1"); }
TEST_CASE("Test_L2_R1 (middle-game, easy)") { check_file("Test_L2_R1"); }

// Slow test cases
TEST_CASE("Test_L1_R1 (begin-game, easy)" * doctest::skip()) { check_file("Test_L1_R1"); }
TEST_CASE("Test_L2_R2 (middle-game, medium)" * doctest::skip()) { check_file("Test_L2_R2"); }

TEST_CASE("Invalid game codes are rejected") {
    Bot bot;
    CHECK_THROWS_AS(bot.play("8"), std::invalid_argument);          // invalud column
    CHECK_THROWS_AS(bot.play("4444444"), std::invalid_argument);    // column overflow
    CHECK_THROWS_AS(bot.play("4455667"), std::invalid_argument);    // move after the game has finished
}
