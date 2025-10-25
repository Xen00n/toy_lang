#pragma once
#include "../lexer/lexer.h"
#include <vector>
#include <stack>
#include <map>

enum class Symbol {
    Number, Plus, Minus, Mul, Div, Leftbrac, Rightbrac, EndOfFile,
    E, E_, T, T_, F, Epsilon
};

Symbol tokenToSymbol(const Token& tok);
bool parse(std::vector<Token>& tokens);
void initParseTable();
