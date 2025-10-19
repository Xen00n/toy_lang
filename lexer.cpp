#include <bits/stdc++.h>

using namespace std;


enum class TokenType {
 Number,Plus,Minus,Mul,Div,Leftbrac,Rightbrac,EndOfFile,Invalid 
};

struct Token {
  TokenType type;
  string text;
};

class Lexer{
  string input;
  size_t pos = 0;

public:
    Lexer(string src): input(move(src)) {}
    Token getNextToken(){
      while(pos<input.size()&&isspace(input[pos]))
        pos++;
      if(pos>=input.size())
        return {TokenType::EndOfFile,"EOF"};
      char ch = input[pos];

      if(isdigit(ch) || ch == '.'){
        size_t start = pos;
        bool got_dot = false;
        if(ch=='.'){
          got_dot = true;
        }
        pos++;
        while(pos< input.size() && (isdigit(input[pos]) || input[pos] == '.')){
          if(input[pos] == '.'){
            if(got_dot){ 
              return {TokenType::Invalid,"Invalid number (mutltiple dots . )"};
            }
            else
              got_dot = true;
          }
          pos++;
        }
        return {TokenType::Number,input.substr(start,pos-start)};
      }
      pos++;
      switch (ch) {
        case '+': return {TokenType::Plus,"+"};

case '-': return {TokenType::Minus,"-"};
case '*': return {TokenType::Mul,"*"};
case '/': return {TokenType::Div,"/"};
case '(': return {TokenType::Leftbrac,"("};

case ')': return {TokenType::Rightbrac,")"};

        default:
          return {TokenType::Invalid,string(1,ch)};
          break;
      }
    }
};


int main() {
    Lexer lexer("(3 + 4) * 2.2.2- 5");
    Token tok;
    do {
        tok = lexer.getNextToken();
        std::cout << "Token: "<< " " << tok.text << "\n";
    } while (tok.type != TokenType::EndOfFile);
}
