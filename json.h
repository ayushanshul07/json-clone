#ifndef jsonclass
#define jsonclass
#include <unordered_map>
#include <string>
#include<iostream>

template<typename Key, typename Value>
class aajson{
    private:
        std::unordered_map<Key, Value> _content;
    public:
        aajson();
        void printJson();
        Value& operator[](const Key& key);
};
#endif