//
// Created by Ali A. Kooshesh on 2/5/19.
//

#include "Statements.hpp"

// Statement
Statement::Statement() {}

// Statements

Statements::Statements() {}
void Statements::addStatement(Statement *statement) { _statements.push_back(statement); }

void Statements::print() {
  for (auto s: _statements){
    s->print();
  }
}

void Statements::evaluate(SymTab &symTab) {
    for (auto s: _statements)
        s->evaluate(symTab);
}

// AssignmentStatement

AssignmentStatement::AssignmentStatement() : _lhsVariable{""}, _rhsExpression{nullptr} {}

AssignmentStatement::AssignmentStatement(std::string lhsVar, ExprNode *rhsExpr):
        _lhsVariable{lhsVar}, _rhsExpression{rhsExpr} {}

void AssignmentStatement::evaluate(SymTab &symTab) {
    int rhs = rhsExpression()->evaluate(symTab);
    symTab.setValueFor(lhsVariable(), rhs);
}

std::string &AssignmentStatement::lhsVariable() {
    return _lhsVariable;
}

ExprNode *&AssignmentStatement::rhsExpression() {
    return _rhsExpression;
}

void AssignmentStatement::print() {
    std::cout << _lhsVariable << " = ";
    _rhsExpression->print();
    std::cout << std::endl;
}


// Print Statement
PrintStatement::PrintStatement() : _variable(""){}
PrintStatement::PrintStatement(std::string var):_variable(var) {}

std::string &PrintStatement::var(){
  return _variable;
}

void PrintStatement::evaluate(SymTab &symTab){
  std::cout << symTab.getValueFor(_variable) << std::endl;
}

void PrintStatement::print(){
  std::cout << "print " << _variable << std::endl;
}

// For Loop Statement
ForStatement::ForStatement() : _leftAssignStmt{nullptr}, _relationalForStmt{nullptr},
			       _rightAssignStmt{nullptr}, _forLoopStmts{nullptr} {}
ForStatement::ForStatement(AssignmentStatement *leftAssignStmt, ExprNode *relationalForStmt,
			   AssignmentStatement *rightAssignStmt, Statements *forLoopStmts) :
  _leftAssignStmt{leftAssignStmt}, _relationalForStmt{relationalForStmt},
  _rightAssignStmt{rightAssignStmt}, _forLoopStmts{forLoopStmts} {}

void ForStatement::evaluate(SymTab &symTab){
  _leftAssignStmt->evaluate(symTab);
  while (_relationalForStmt->evaluate(symTab)){
    _forLoopStmts->evaluate(symTab);
    _rightAssignStmt->evaluate(symTab);
  }
}

void ForStatement::print(){
  std::cout << "for(" << _leftAssignStmt->lhsVariable() << " = ";
  _leftAssignStmt->rhsExpression()->print();
  std::cout << "; ";
  _relationalForStmt->print();
  std::cout << "; ";
  std::cout << _rightAssignStmt->lhsVariable() << " = ";
  _rightAssignStmt->rhsExpression()->print();
  std::cout << ") {" << std::endl;
  _forLoopStmts->print();
  std::cout << "}" << std::endl;


}


