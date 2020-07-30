//
// Created by Ali A. Kooshesh on 2/5/19.
//

#ifndef EXPRINTER_STATEMENTS_HPP
#define EXPRINTER_STATEMENTS_HPP

#include <iostream>
#include <vector>

#include "ArithExpr.hpp"
#include "SymTab.hpp"

// The Statement (abstract) class serves as a super class for all statements that
// are defined in the language. Ultimately, statements have to be evaluated.
// Therefore, this class defines evaluate, a pure-virtual function to make
// sure that all subclasses of Statement provide an implementation for this
// function.

class Statement {
public:
    Statement();

    virtual void print() = 0;
    virtual void evaluate(SymTab &symTab) = 0;

private:
};

// Statements is a collection of Statement. For example, all statements in a function
// can be represented by an instance of Statements.

class Statements {
public:
    Statements();

    void addStatement(Statement *statement);
    void evaluate(SymTab &symTab);

    void print();
  
private:
    std::vector<Statement *> _statements;
  //For for loops
  // std::vector<Statement *> _forStatements;
};

// AssignmentStatement represents the notion of an lValue having been assigned an rValue.
// The rValue is an expression.

class AssignmentStatement : public Statement {
public:
    AssignmentStatement();
    AssignmentStatement(std::string lhsVar, ExprNode *rhsExpr);

    std::string &lhsVariable();
    ExprNode *&rhsExpression();

    virtual void evaluate(SymTab &symTab);
    virtual void print();


private:
    std::string _lhsVariable;
    ExprNode *_rhsExpression;
};

//For print statements
class PrintStatement : public Statement {
public:
  PrintStatement();
  PrintStatement(std::string var);

  std::string &var();

  virtual void evaluate(SymTab &symTab);
  virtual void print();

private:
  std::string _variable;
};

//For for loops
class ForStatement : public Statement {
public:
  ForStatement();
  ForStatement(AssignmentStatement *leftAssignStmt, ExprNode *relationalForStmt, AssignmentStatement *rightAssignStmt, Statements *forLoopStmts);

  virtual void evaluate(SymTab &symTab);
  virtual void print();

private:
  AssignmentStatement *_leftAssignStmt;
  ExprNode *_relationalForStmt;
  AssignmentStatement *_rightAssignStmt;
  Statements *_forLoopStmts;
};

#endif //EXPRINTER_STATEMENTS_HPP
