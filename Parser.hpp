
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

    Statements *statements();

    AssignmentStatement *assignStatement();
  PrintStatement *printStatement();
  ForStatement *forStatement();

    ExprNode *expr();
    ExprNode *term();
    ExprNode *primary();

  //Relational
  ExprNode *relationalExpr();
  ExprNode *relationalTerm();
  ExprNode *relationalPrimary();

    std::string id();

private:
    Tokenizer &tokenizer;

    void die(std::string where, std::string message, Token &token);

};

#endif

