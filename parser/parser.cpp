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


static bool isTerminal(Symbol s)
{
    switch(s){
        case Symbol::Leftbrac:
        case Symbol::Rightbrac:
        case Symbol::Plus:
        case Symbol::Minus:
        case Symbol::Mul:
        case Symbol::Div:
        case Symbol::Number:
        case Symbol::EndOfFile:
            return true;
        default:
            return false;
    }
}

static const char* symName(Symbol s)
{
    switch(s){
        case Symbol::E:         return "E";
        case Symbol::E_:        return "E'";
        case Symbol::T:         return "T";
        case Symbol::T_:        return "T'";
        case Symbol::F:         return "F";
        case Symbol::Plus:      return "+";
        case Symbol::Minus:     return "-";
        case Symbol::Mul:       return "*";
        case Symbol::Div:       return "/";
        case Symbol::Leftbrac:  return "(";
        case Symbol::Rightbrac: return ")";
        case Symbol::Number:    return "number";
        case Symbol::EndOfFile: return "$";
        case Symbol::Epsilon:   return "ε";
        default:                return "?";
    }
}

void printTree(Node* n, int depth )
{
    if(!n) return;

    // indent
    for(int i = 0; i < depth; i++)
        cout << "  ";

    cout << symName(n->sym) << "\n";

    for(Node* c : n->kids)
        printTree(c, depth + 1);
}

bool parse(vector<Token>& tokens , Node*& outRoot)
{
    stack<Symbol> st;
    stack<Node*> nodeSt;

    // create root + eof nodes
    Node* root    = new Node{Symbol::E};
    Node* eofNode = new Node{Symbol::EndOfFile};

    // push symbols
    st.push(Symbol::EndOfFile);
    st.push(Symbol::E);

    // push corresponding nodes
    nodeSt.push(eofNode);
    nodeSt.push(root);

    size_t pos = 0;
    Token current = tokens[pos];
    Symbol lookahead = tokenToSymbol(current);

    while(!st.empty())
    {
        Symbol top = st.top();
        st.pop();

        // terminal match
        if(isTerminal(top) && top == lookahead)
        {
            nodeSt.pop(); // remove terminal node from stack

            // consume
            pos++;
            if(pos < tokens.size()){
                current = tokens[pos];
                lookahead = tokenToSymbol(current);
            } else {
                lookahead = Symbol::EndOfFile;
            }
            continue;
        }

        // terminal mismatch
        if(isTerminal(top))
        {
            cerr << "syntax error: expected terminal but got '" << current.text << "'\n";
            return false;
        }

        if(top == Symbol::Epsilon)
            continue;

        // nonterminal expand
        auto key = make_pair(top, lookahead);
        if(table.find(key) == table.end())
        {
            cerr << "Syntax error at '" << current.text << "'\n";
            return false;
        }

        Rule& rhs = table[key];

        Node* parent = nodeSt.top();
        nodeSt.pop();

        vector<Node*> children;
        for(Symbol s : rhs)
        {
            if(s == Symbol::Epsilon){
                Node* eps = new Node{Symbol::Epsilon};
                parent->kids.push_back(eps);
            } else {
                Node* c = new Node{s};
                parent->kids.push_back(c);
                children.push_back(c);
            }
        }

        for(auto it = children.rbegin(); it != children.rend(); ++it){
            st.push((*it)->sym);
            nodeSt.push(*it);
        }
    }

    outRoot = root;     
    return (lookahead == Symbol::EndOfFile);

}

