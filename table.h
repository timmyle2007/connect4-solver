#ifndef TABLE_H
#define TABLE_H

#include <cstdint>
using KeyType = std::uint32_t;
using ValueType = std::int8_t;
class TranspositionTable{

public:
    TranspositionTable(){
        keys = new KeyType[TABLE_SIZE]{};
        values = new ValueType[TABLE_SIZE]{};
    }
    ~TranspositionTable(){
        delete[] keys;
        delete[] values;
    }
    void update(std::uint64_t key, ValueType value){
        int index = key % TABLE_SIZE;
        keys[index] = key;
        values[index] = value;
    }
    ValueType get(std::uint64_t key)const{
        int index = key % TABLE_SIZE;
        if(keys[index] == static_cast<KeyType>(key) && values[index] > 0)return values[index];
        return 0;
    }
private:
    static constexpr std::size_t TABLE_SIZE = (1 << 23) + 9;
    KeyType* keys;
    ValueType* values;
};

#endif