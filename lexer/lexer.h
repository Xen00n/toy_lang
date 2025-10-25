#pragma once
#include <string>

enum class TokenType {
    Number, Plus, Minus, Mul, Div, Leftbrac, Rightbrac, EndOfFile, Invalid
};

struct Token {
    TokenType type;
    std::string text;
};

class Lexer {
    std::string input;
    size_t pos = 0;

public:
    Lexer(const std::string& src) : input(src) {}
    Token getNextToken();
};
