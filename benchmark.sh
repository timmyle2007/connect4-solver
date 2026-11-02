set -e

BUILD="g++ -std=c++17 -O2 -march=native -I. main.cpp bot.cpp -o"

$BUILD bot_0 -DNO_SYMMETRY -DNO_HASH_MOVE -DNO_PREFETCH -DNO_DRAW_CHECK -DBENCHMARK

$BUILD bot_1 -DNO_SYMMETRY -DNO_PREFETCH -DNO_DRAW_CHECK -DBENCHMARK

$BUILD bot_2 -DNO_SYMMETRY -DNO_DRAW_CHECK -DBENCHMARK

$BUILD bot_3 -DNO_DRAW_CHECK -DBENCHMARK

$BUILD bot_4 -DNO_SYMMETRY -DBENCHMARK

$BUILD bot_5 -DBENCHMARK

$BUILD bot_6 -DBUCKET_TABLE -DBENCHMARK

printf "%-6s %-14s %12s %14s %12s\n" bot "test set" mean_ms nodes ns_per_node

for test_file in tests/data/Test_L3_R1 tests/data/Test_L2_R1 tests/data/Test_L1_R1 tests/data/Test_L2_R2; do
        for bot in bot_0 bot_1 bot_2 bot_3 bot_4 bot_5 bot_6; do
                for iter in 1 2 3 4 5; do
                        ./$bot $test_file | tail -3
                done | awk -v bot=$bot -v test_file=$test_file '
                {ms=$1; nodes=$2; ns_nodes=$3}
                NR==1 || ms<best{ best=ms; best_nodes=nodes; best_ns=ns_nodes}
                END {printf "%-6s %-14s %12.5f %14.1f %10.2f\n", bot, test_file, best, best_nodes, best_ns}'
        done
    echo "---------------------------------------------------------------------"
done

#Final set

#To save time, only take the first 20 row
head -n 20 tests/data/Test_L1_R3 > tests/data/Test_L1_R3_20

for test_file in tests/data/Test_L1_R3_20; do
        for bot in bot_0 bot_1 bot_2 bot_3 bot_4 bot_5 bot_6; do
                for iter in 1 2 3; do
                        ./$bot $test_file | tail -3
                done | awk -v bot=$bot -v test_file=$test_file '
                {ms=$1; nodes=$2; ns_nodes=$3}
                NR==1 || ms<best{ best=ms; best_nodes=nodes; best_ns=ns_nodes}
                END {printf "%-6s %-14s %12.5f %14.1f %10.2f\n", bot, test_file, best, best_nodes, best_ns}'
        done
    echo "---------------------------------------------------------------------"
done
