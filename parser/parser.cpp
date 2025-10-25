#include "parser.h"
#include <iostream>
using namespace std;

using Rule = vector<Symbol>;
map<pair<Symbol,Symbol>,Rule> table;

Symbol tokenToSymbol(const Token& tok){
    switch (tok.type) {
        case TokenType::Plus: return Symbol::Plus;
        case TokenType::Minus: return Symbol::Minus;
        case TokenType::Mul: return Symbol::Mul;
        case TokenType::Div: return Symbol::Div;
        case TokenType::Leftbrac: return Symbol::Leftbrac;
        case TokenType::Rightbrac: return Symbol::Rightbrac;
        case TokenType::Number: return Symbol::Number;
        case TokenType::EndOfFile: return Symbol::EndOfFile;
        default: return Symbol::EndOfFile;
    }
}

void initParseTable() {
    // E → T E'
    table[{Symbol::E,Symbol::Leftbrac}] = {Symbol::T,Symbol::E_};
    table[{Symbol::E, Symbol::Number}]   = {Symbol::T, Symbol::E_};

    // E' → + T E' | - T E' | ε
    table[{Symbol::E_, Symbol::Plus}]  = {Symbol::Plus, Symbol::T, Symbol::E_};
    table[{Symbol::E_, Symbol::Minus}] = {Symbol::Minus, Symbol::T, Symbol::E_};
    table[{Symbol::E_, Symbol::Rightbrac}] = {Symbol::Epsilon};
    table[{Symbol::E_, Symbol::EndOfFile}] = {Symbol::Epsilon};

    // T → F T'
    table[{Symbol::T, Symbol::Leftbrac}] = {Symbol::F, Symbol::T_};
    table[{Symbol::T, Symbol::Number}]   = {Symbol::F, Symbol::T_};

    // T' → * F T' | / F T' | ε
    table[{Symbol::T_, Symbol::Mul}]   = {Symbol::Mul, Symbol::F, Symbol::T_};
    table[{Symbol::T_, Symbol::Div}]   = {Symbol::Div, Symbol::F, Symbol::T_};
    table[{Symbol::T_, Symbol::Plus}]  = {Symbol::Epsilon};
    table[{Symbol::T_, Symbol::Minus}] = {Symbol::Epsilon};
    table[{Symbol::T_, Symbol::Rightbrac}] = {Symbol::Epsilon};
    table[{Symbol::T_, Symbol::EndOfFile}] = {Symbol::Epsilon};

    // F → ( E ) | Number
    table[{Symbol::F, Symbol::Leftbrac}] = {Symbol::Leftbrac, Symbol::E, Symbol::Rightbrac};
    table[{Symbol::F, Symbol::Number}]   = {Symbol::Number};
}

bool parse(vector<Token>& tokens) {
    stack<Symbol> st;
    st.push(Symbol::EndOfFile);
    st.push(Symbol::E);

    size_t pos = 0;
    Token current = tokens[pos];
    Symbol lookahead = tokenToSymbol(current);

    while (!st.empty()) {
        Symbol top = st.top();
        st.pop();

        if(top == lookahead) {
            pos++;
            if(pos < tokens.size()) {
                current = tokens[pos];
                lookahead = tokenToSymbol(current);
            }
        }
        else if(top == Symbol::Epsilon) {
            continue;
        }
        else if(static_cast<int>(top) <= static_cast<int>(Symbol::EndOfFile)) {
            cerr << "Syntax error: expected ";
            switch(top) {
                case Symbol::Rightbrac: cerr << "')'"; break;
                case Symbol::Leftbrac:  cerr << "'('"; break;
                case Symbol::Plus:      cerr << "'+'"; break;
                case Symbol::Minus:     cerr << "'-'"; break;
                case Symbol::Mul:       cerr << "'*'"; break;
                case Symbol::Div:       cerr << "'/'"; break;
                case Symbol::Number:    cerr << "a number"; break;
                case Symbol::EndOfFile: cerr << "end of input"; break;
                default: cerr << "something else"; break;
            }
            cerr << " but found '" << current.text << "'\n";
            return false;
        }
        else {
            auto key = make_pair(top, lookahead);
            if(table.find(key) == table.end()) {
                cerr << "Syntax error at '" << current.text << "'\n";
                return false;
            }
            auto rhs = table[key];
            for(auto it = rhs.rbegin(); it != rhs.rend(); ++it)
                if(*it != Symbol::Epsilon) st.push(*it);
        }
    }

    return lookahead == Symbol::EndOfFile;
}
