#include "json.h"
#include <string>

int main(){

    aajson<std::string, int> myjson;
    myjson["ayush"] = 24;

    return 0;
}