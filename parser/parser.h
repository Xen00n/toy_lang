#pragma once
#include "../lexer/lexer.h"
#include <vector>
#include <stack>
#include <map>

enum class Symbol {
    Number, Plus, Minus, Mul, Div, Leftbrac, Rightbrac, EndOfFile,
    E, E_, T, T_, F, Epsilon
};
struct Node
{
    Symbol sym;
    std::vector<Node*> kids;
};
Symbol tokenToSymbol(const Token& tok);

bool parse(std::vector<Token>& tokens, Node*& outRoot);

void initParseTable();

static bool isTerminal(Symbol s);
static const char* symName(Symbol s);
void printTree(Node* n, int depth = 0);
