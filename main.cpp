#include "simple_json.h"
#include <iostream>
#include <string>

int main() {
    std::string text = "{\"name\": \"Codex\", \"active\": true, \"count\": 3}";
    try {
        auto value = simplejson::parse(text);
        std::cout << "Parsed JSON successfully\n";
        (void)value; // suppress unused warning
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}
