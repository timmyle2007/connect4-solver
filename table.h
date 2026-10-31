#ifndef TABLE_H
#define TABLE_H

#include <cstdint>
#include <cstring>
#include <memory>
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
        void update(std::uint64_t key, ValueType value, int col){
            int index = key % TABLE_SIZE;
            keys[index] = (static_cast<KeyType>(key) & KEY_MASK) + (1ULL << 29) * col;
            values[index] = value;
        }
        std::pair<ValueType, int> get(std::uint64_t key)const{
            int index = key % TABLE_SIZE;
            if(((keys[index] & KEY_MASK) == (key & KEY_MASK)) && values[index] > 0)return std::make_pair(values[index], keys[index] >> 29);
            return std::make_pair(0, 0);
        }
        void prefetch(std::uint64_t key)const{
            int index = key % TABLE_SIZE;
            __builtin_prefetch(&keys[index]);
            __builtin_prefetch(&values[index]);
        }

    private:
        std::unique_ptr<KeyType[]> keys;
        std::unique_ptr<ValueType[]> values;
    };

}
#endif