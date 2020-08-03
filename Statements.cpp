//
// Created by Ali A. Kooshesh on 2/5/19.
//

#include "Statements.hpp"
#include <vector>

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
    TypeDescriptor *rhs = rhsExpression()->evaluate(symTab);
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
PrintStatement::PrintStatement(std::vector<ExprNode *> testList){
  for (int i = 0; i < testList.size(); i++)
    _testList.push_back(testList[i]);
};

void PrintStatement::evaluate(SymTab &symTab){

  std::vector<TypeDescriptor *> printList;

  if (_testList.size() == 0){
    std::cout << "PrintStatement::evaluate: No Value" << std::endl;
  }
  else{
    for (int i = 0; i < _testList.size(); i++){
      TypeDescriptor * _testExpr = _testList[i]->evaluate(symTab);
      if (_testExpr->type() == TypeDescriptor::INTEGER){
	IntegerDescriptor *_testExprType = dynamic_cast<IntegerDescriptor *>(_testExpr);
	std::cout << "PrintStatement::evaluate: " << _testExprType->getIntValue() << std::endl;
	printList.push_back(_testExpr);
      }
      else if (_testExpr->type() == TypeDescriptor::DOUBLE){
	DoubleDescriptor *_testExprType = dynamic_cast<DoubleDescriptor *>(_testExpr);
	std::cout << "PrintStatement::evaluate: " << _testExprType->getDoubleValue() << std::endl;
	printList.push_back(_testExpr);
      }
      else if (_testExpr->type() == TypeDescriptor::STRING){
	StringDescriptor *_testExprType = dynamic_cast<StringDescriptor *>(_testExpr);
	std::cout << "PrintStatement::evaluate: " << _testExprType->getStringValue() << std::endl;
	printList.push_back(_testExpr);
      }
      else{
	std::cout << "Invalid type" << std::endl;
	exit(2);
      }
    }
    std::cout << "PrintStatement::evaluate:PrintOutput: ";
    for (int j = 0; j < printList.size(); j++){
      if (printList[j]->type() == TypeDescriptor::INTEGER){
	IntegerDescriptor *_testExprType = dynamic_cast<IntegerDescriptor *>(printList[j]);
	std::cout << _testExprType->getIntValue() << " ";
      }
      else if (printList[j]->type() == TypeDescriptor::DOUBLE){
	DoubleDescriptor *_testExprType = dynamic_cast<DoubleDescriptor *>(printList[j]);
	std::cout << _testExprType->getDoubleValue() << " ";
      }
      else {
	StringDescriptor *_testExprType = dynamic_cast<StringDescriptor *>(printList[j]);
	std::cout << _testExprType->getStringValue() << " ";
      }   
    }
    std::cout << std::endl;
  }
}

void PrintStatement::print(){

  if (_testList.size() == 0)
    std::cout << "print" << std::endl;
  else{
    std::cout << "print ";
    for (int i = 0; i < _testList.size()-1; i++){
      _testList[i]->print();
      std::cout << ", "; 
    }
    _testList[_testList.size()-1]->print();
    std::cout << std::endl;
  }
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

  TypeDescriptor *_typeDescRelational = _relationalForStmt->evaluate(symTab);
  IntegerDescriptor *_relationalTypeDesc = dynamic_cast<IntegerDescriptor *>(_typeDescRelational);
  
  while (_relationalTypeDesc->getIntValue()){
    _rightAssignStmt->evaluate(symTab);
    _forLoopStmts->evaluate(symTab);
    _typeDescRelational = _relationalForStmt->evaluate(symTab);
    _relationalTypeDesc = dynamic_cast<IntegerDescriptor *> (_typeDescRelational);
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


