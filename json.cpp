#include <iostream>
#include "json.h"
#include <unordered_map>

template<typename Key, typename Value>
void aajson<Key, Value>::printJson() {
    auto iterator = _content.begin();
    for ( ; iterator != _content.end() ; iterator++ ) {
        std::cout<<iterator->first << " " << iterator->second << std::endl;
    }
}   

template<typename Key, typename Value>
aajson<Key, Value>::aajson() {
}

template<typename Key, typename Value>
Value& aajson<Key, Value>::operator[](const Key& key) {
   return _content[key];
}
template class aajson<std::string, int>;
