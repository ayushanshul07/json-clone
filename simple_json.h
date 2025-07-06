#pragma once
#include <string>
#include <variant>
#include <vector>
#include <unordered_map>
#include <cctype>
#include <stdexcept>

namespace simplejson {

struct JsonValue;
using JsonObject = std::unordered_map<std::string, JsonValue>;
using JsonArray = std::vector<JsonValue>;

struct JsonValue {
    std::variant<std::nullptr_t, bool, double, std::string, JsonArray, JsonObject> value;
};

class Parser {
public:
    explicit Parser(const std::string& text) : text(text), pos(0) {}
    JsonValue parse_value();
    friend JsonValue parse(const std::string& text);
    size_t position() const { return pos; }

private:
    const std::string& text;
    size_t pos;

    void skip_ws();
    bool consume(char c);
    char peek() const;

    JsonValue parse_null();
    JsonValue parse_true();
    JsonValue parse_false();
    JsonValue parse_number();
    JsonValue parse_string();
    JsonValue parse_array();
    JsonValue parse_object();
};

inline void Parser::skip_ws() {
    while (pos < text.size() && std::isspace(static_cast<unsigned char>(text[pos])))
        ++pos;
}

inline char Parser::peek() const {
    if (pos >= text.size()) throw std::runtime_error("Unexpected end of input");
    return text[pos];
}

inline bool Parser::consume(char c) {
    if (pos < text.size() && text[pos] == c) {
        ++pos;
        return true;
    }
    return false;
}

inline JsonValue Parser::parse_value() {
    skip_ws();
    char c = peek();
    if (c == 'n') return parse_null();
    if (c == 't') return parse_true();
    if (c == 'f') return parse_false();
    if (c == '"') return parse_string();
    if (c == '[') return parse_array();
    if (c == '{') return parse_object();
    return parse_number();
}

inline JsonValue Parser::parse_null() {
    if (text.compare(pos, 4, "null") != 0) throw std::runtime_error("Invalid null");
    pos += 4;
    return JsonValue{nullptr};
}

inline JsonValue Parser::parse_true() {
    if (text.compare(pos, 4, "true") != 0) throw std::runtime_error("Invalid true");
    pos += 4;
    return JsonValue{true};
}

inline JsonValue Parser::parse_false() {
    if (text.compare(pos, 5, "false") != 0) throw std::runtime_error("Invalid false");
    pos += 5;
    return JsonValue{false};
}

inline JsonValue Parser::parse_number() {
    size_t start = pos;
    if (text[pos] == '-') ++pos;
    while (pos < text.size() && std::isdigit(static_cast<unsigned char>(text[pos]))) ++pos;
    if (pos < text.size() && text[pos] == '.') {
        ++pos;
        while (pos < text.size() && std::isdigit(static_cast<unsigned char>(text[pos]))) ++pos;
    }
    double num = std::stod(text.substr(start, pos - start));
    return JsonValue{num};
}

inline JsonValue Parser::parse_string() {
    if (!consume('"')) throw std::runtime_error("Expected \"");
    std::string result;
    while (pos < text.size()) {
        char c = text[pos++];
        if (c == '\\') {
            if (pos >= text.size()) throw std::runtime_error("Bad escape");
            char esc = text[pos++];
            switch (esc) {
                case '"': result.push_back('"'); break;
                case '\\': result.push_back('\\'); break;
                case '/': result.push_back('/'); break;
                case 'b': result.push_back('\b'); break;
                case 'f': result.push_back('\f'); break;
                case 'n': result.push_back('\n'); break;
                case 'r': result.push_back('\r'); break;
                case 't': result.push_back('\t'); break;
                default: throw std::runtime_error("Unsupported escape");
            }
        } else if (c == '"') {
            return JsonValue{result};
        } else {
            result.push_back(c);
        }
    }
    throw std::runtime_error("Unterminated string");
}

inline JsonValue Parser::parse_array() {
    consume('[');
    JsonArray arr;
    skip_ws();
    if (consume(']')) return JsonValue{arr};
    while (true) {
        arr.push_back(parse_value());
        skip_ws();
        if (consume(']')) break;
        if (!consume(',')) throw std::runtime_error("Expected , in array");
    }
    return JsonValue{arr};
}

inline JsonValue Parser::parse_object() {
    consume('{');
    JsonObject obj;
    skip_ws();
    if (consume('}')) return JsonValue{obj};
    while (true) {
        skip_ws();
        JsonValue key = parse_string();
        skip_ws();
        if (!consume(':')) throw std::runtime_error("Expected : in object");
        obj[std::get<std::string>(key.value)] = parse_value();
        skip_ws();
        if (consume('}')) break;
        if (!consume(',')) throw std::runtime_error("Expected , in object");
    }
    return JsonValue{obj};
}

inline JsonValue parse(const std::string& text) {
    Parser p(text);
    JsonValue val = p.parse_value();
    p.skip_ws();
    if (p.pos != text.size()) throw std::runtime_error("Extra data");
    return val;
}

} // namespace simplejson
