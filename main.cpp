#include "json.h"
#include <string>

int main(){

    aajson<std::string, int> myjson;
    myjson["ayush"] = 22;
    myjson.printJson();
    return 0;
}