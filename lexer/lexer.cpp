#include "lexer.h"
#include <cctype>

Token Lexer::getNextToken() {
    while(pos < input.size() && isspace(input[pos])) pos++;
    if(pos >= input.size()) return {TokenType::EndOfFile,"EOF"};

    char ch = input[pos];

    // Number (integer or float)
    if(isdigit(ch) || ch == '.') {
        size_t start = pos;
        bool got_dot = (ch == '.');
        pos++;
        while(pos < input.size() && (isdigit(input[pos]) || input[pos] == '.')) {
            if(input[pos] == '.') {
                if(got_dot) return {TokenType::Invalid, "Invalid number (multiple dots)"};
                got_dot = true;
            }
            pos++;
        }
        return {TokenType::Number, input.substr(start, pos - start)};
    }

    // Single-character tokens
    pos++;
    switch(ch) {
        case '+': return {TokenType::Plus,"+"};
        case '-': return {TokenType::Minus,"-"};
        case '*': return {TokenType::Mul,"*"};
        case '/': return {TokenType::Div,"/"};
        case '(': return {TokenType::Leftbrac,"("};
        case ')': return {TokenType::Rightbrac,")"};
        default:  return {TokenType::Invalid, std::string(1,ch)};
    }
}
