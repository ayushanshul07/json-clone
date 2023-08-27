#include <unordered_map>

template<typename Key, typename Value>
class aajson{

    private:
        std::unordered_map<Key, Value> _content;

    
    public:
        aajson();

        Value& operator[](const Key& key);
};

template<typename Key, typename Value>
aajson<Key, Value>::aajson(){

}

template<typename Key, typename Value>
Value& aajson<Key, Value>::operator[](const Key& key){
   return _content[key];
}