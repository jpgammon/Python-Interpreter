//
// Created by Ali A. Kooshesh on 1/30/19.
//

#ifndef EXPRINTER_TOKENIZER_HPP
#define EXPRINTER_TOKENIZER_HPP


#include <fstream>
#include <vector>
#include <stack>
#include "Token.hpp"

class Tokenizer {

public:
  Tokenizer(std::ifstream &inStream);
  Token getToken();
  void ungetToken();
  void printProcessedTokens();
  
private:
  Token lastToken;
  bool ungottenToken;
  std::ifstream &inStream;
  std::vector<Token> _tokens;
  std::stack<int> indentS;
  bool parsingANewLine;

private:
  std::string readName();
  std::string readInteger();
  bool isDouble(std::string s);
  double readDouble(int readInt);
  std::string readString();
};

#endif //EXPRINTER_TOKENIZER_HPP
