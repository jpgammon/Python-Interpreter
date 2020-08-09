
#ifndef __PARSER_HPP
#define __PARSER_HPP

#include "Token.hpp"
#include "Tokenizer.hpp"
#include "SymTab.hpp"
#include "ArithExpr.hpp"
#include "Statements.hpp"

#include<vector>
#include<iostream>
#include<map>


class Parser {
public:
  Parser(Tokenizer &tokenizer) : tokenizer{tokenizer} {}

  Statements *suite();
  Statements *statements();
  AssignmentStatement *assignStatement();
  AssignmentStatement *forLoopAssignStatement();
  PrintStatement *printStatement();
  ForStatement *forStatement();
  IfStatement *ifStatement();

  std::vector<ExprNode *> parseRangeList();

  ExprNode *expr();
  ExprNode *term();
  ExprNode *primary();

  //Relational
  ExprNode *relationalExpr();
  ExprNode *relationalPrimary();

  //Logical
  ExprNode *or_Test();
  ExprNode *and_Test();
  ExprNode *not_Test();
  
  std::string id();

private:
  Tokenizer &tokenizer;
  
  void die(std::string where, std::string message, Token &token);
};

#endif

