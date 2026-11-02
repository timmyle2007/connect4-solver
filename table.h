#ifndef TABLE_H
#define TABLE_H

#include <cstdint>
#include <cstring>
#include <memory>
#include <vector>
#include <random>
namespace C4{

    class TranspositionTable{
    using KeyType = std::uint32_t;
    using ValueType = std::int8_t;
    static constexpr std::size_t TABLE_SIZE = 8388593;
    static constexpr std::uint32_t KEY_MASK = (1ULL << 29) - 1;
    public:
        TranspositionTable(): keys(std::make_unique<KeyType[]>(TABLE_SIZE)), values(std::make_unique<ValueType[]>(TABLE_SIZE)){
            reset();
        }
        void reset(){
            std::memset(keys.get(), 0, TABLE_SIZE * sizeof(KeyType));
            std::memset(values.get(), 0, TABLE_SIZE * sizeof(ValueType));
        }
        void update(std::uint64_t key, int value, int col){
            std::uint64_t index = key % TABLE_SIZE;
            keys[index] = (static_cast<KeyType>(key) & KEY_MASK) + static_cast<KeyType>((1ULL << 29) * col);
            values[index] = static_cast<ValueType>(value);
        }
        std::pair<ValueType, int> get(std::uint64_t key)const{
            std::uint64_t index = key % TABLE_SIZE;
            if(((keys[index] & KEY_MASK) == (key & KEY_MASK)) && values[index] > 0)return std::make_pair(values[index], keys[index] >> 29);
            return std::make_pair(0, 0);
        }
        void prefetch(std::uint64_t key)const{
            std::uint64_t index = key % TABLE_SIZE;
            __builtin_prefetch(&keys[index]);
            __builtin_prefetch(&values[index]);
        }

    private:
        std::unique_ptr<KeyType[]> keys;
        std::unique_ptr<ValueType[]> values;
    };


    class BucketTranspositionTable{
    public:
        using KeyType = std::uint32_t;
        using ValueType = std::int8_t;
        static constexpr std::size_t TABLE_SIZE = 8388593;
        static constexpr std::uint32_t KEY_MASK = (1ULL << 29) - 1;
        static constexpr int SLOTS_PER_BUCKET = 6;
        struct alignas(64) Bucket{
            std::uint32_t keys[SLOTS_PER_BUCKET];
            std::int8_t values[SLOTS_PER_BUCKET];
        };

        BucketTranspositionTable(): table(TABLE_SIZE){

        }

        void reset(){
            std::fill(table.begin(), table.end(), Bucket{});
        }

        void update_bucket(Bucket& bucket, std::size_t index, std::uint64_t key, int value, int col){
            bucket.keys[index] = (static_cast<KeyType>(key) & KEY_MASK) + static_cast<KeyType>((1ULL << 29) * col);
            bucket.values[index] = static_cast<ValueType>(value);
        }

        void update(std::uint64_t key, int value, int col){
            std::uint64_t index = key % TABLE_SIZE;
            Bucket& bucket = table[index];

            std::size_t free_index = SLOTS_PER_BUCKET;

            for(std::size_t i = 0; i < SLOTS_PER_BUCKET; i++){
                if(bucket.values[i] > 0 && bucket.keys[i] == static_cast<KeyType>(key)){
                    update_bucket(bucket, i, key, value, col);
                    return;
                }
                if(bucket.values[i] == 0){
                    free_index = i;
                }
            }

            if(free_index != SLOTS_PER_BUCKET){
                update_bucket(bucket, free_index, key, value, col);
                return;
            }

            update_bucket(bucket, rng() % SLOTS_PER_BUCKET , key, value, col);

        }


        std::pair<ValueType, int>get(std::uint64_t key){
            std::uint64_t index = key % TABLE_SIZE;
            Bucket &bucket = table[index];
            for(std::size_t i = 0; i < 12; i++){
                if(((bucket.keys[i] & KEY_MASK) == (key & KEY_MASK)) && bucket.values[i] > 0)return std::make_pair(bucket.values[i], bucket.keys[i] >> 29);
            }
            return std::make_pair(0, 0);
        }

        void prefetch(std::uint64_t key){
            std::uint64_t index = key % TABLE_SIZE;
            __builtin_prefetch(&table[index]);
        }
    private:
        std::mt19937 rng{42};
        std::vector<Bucket>table;
    };

};
#endif