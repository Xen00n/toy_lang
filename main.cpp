#include "lexer/lexer.h"
#include "parser/parser.h"
#include <iostream>
#include <vector>

using namespace std;

int main() {
    string input;
    cout << "Enter expression: ";
    getline(cin, input);

    Lexer lexer(input);
    vector<Token> tokens;

    Token tok;
    do {
        tok = lexer.getNextToken();
        if(tok.type == TokenType::Invalid) {
            cerr << "Lexer error: " << tok.text << "\n";
            return 1;
        }
        tokens.push_back(tok);
    } while(tok.type != TokenType::EndOfFile);

    initParseTable();
    Node* root=nullptr;
    if(parse(tokens,root)){
        cout << "Parsing successful!\n";
        printTree(root,0);
 
    }
    else
        cout << "Parsing failed.\n";

    return 0;
}
