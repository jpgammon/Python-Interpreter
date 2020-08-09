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

int Statements::size(){
  return _statements.size();
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

// IF STATEMENTS
IfStatement::IfStatement(ExprNode *ifExpr, Statements *ifStmts, Statements *elseStmts,
			 std::vector<ExprNode *> elifExprs, std::vector<Statements *> elifStmts, bool elseStmtsBool){
  _ifExpr = ifExpr;
  _ifStmts = ifStmts;
  _elseStmts = elseStmts;
  _elseStmtsBool = elseStmtsBool;

  for (int i = 0; i < elifExprs.size(); i++)
    _elifExprs.push_back(elifExprs[i]);

  for (int j = 0; j < elifStmts.size(); j++)
    _elifStmts.push_back(elifStmts[j]);
}
	
ExprNode *&IfStatement::ifExpr(){
  return _ifExpr;
}

void IfStatement::evaluate(SymTab &symTab){

  bool elifTest = 0;
  //If Statements evaluation
  TypeDescriptor *_ifTypeDesc = _ifExpr->evaluate(symTab);
  IntegerDescriptor *_ifExprTypeDesc = dynamic_cast<IntegerDescriptor *>(_ifTypeDesc);

  if (_ifExprTypeDesc->getIntValue()){
    elifTest = 1;
    _ifStmts->evaluate(symTab);
  }

  //Elif Statement evaluation
  else if (_elifExprs.size() > 0){
    for (int i = 0; i < _elifExprs.size(); i++){
      TypeDescriptor *_elifTypeDesc = _elifExprs[i]->evaluate(symTab);
      IntegerDescriptor *_elifExprTypeDesc = dynamic_cast<IntegerDescriptor *>(_elifTypeDesc);
      if (_elifExprTypeDesc->getIntValue()){
	_elifStmts[i]->evaluate(symTab);
	elifTest = 1;
	break;
      }
    }
  }
    
  //Else Statement evaluation
  if (!elifTest && _elseStmts->size() > 0)
      _elseStmts->evaluate(symTab);
}

void IfStatement::print(){
  std::cout << "if ";
  _ifExpr->print();
  std::cout << ":" << std::endl;
  _ifStmts->print();

  if (_elifExprs.size() > 0){
    for (int i = 0; i < _elifExprs.size(); i++){
      std::cout << "elif ";
      _elifExprs[i]->print();
      std::cout << ":" << std::endl;
      _elifStmts[i]->print();
    }
  }
  
  if (_elseStmtsBool){
    std::cout << "else: " << std::endl;
    _elseStmts->print();
  }
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

ForStatement::ForStatement(std::string varName, std::vector<ExprNode *>rangeList, Statements *forStmts){
  _varName = varName;
  _forStmts = forStmts;
  for (int i = 0; i < rangeList.size(); i++)
    _rangeList.push_back(rangeList[i]);
}

void ForStatement::evaluate(SymTab &symTab){

  TypeDescriptor *rListInit; TypeDescriptor *rListVar; TypeDescriptor *rListRange; TypeDescriptor *rListStep;
  IntegerDescriptor *intInit; IntegerDescriptor *intVar; IntegerDescriptor *intRange; IntegerDescriptor *intStep;

  if (_rangeList.size() == 1){

    rListRange = _rangeList[0]->evaluate(symTab);
    intRange = dynamic_cast<IntegerDescriptor *> (rListRange);
    
    intVar = new IntegerDescriptor(TypeDescriptor::INTEGER, 0);
    symTab.setValueFor(_varName, intVar);
    
    for (int i = 0; i < intRange->getIntValue(); i++){
      _forStmts->evaluate(symTab);
      if (i+1 != intRange->getIntValue())
	symTab.setValueFor(_varName, intVar = new IntegerDescriptor(TypeDescriptor::INTEGER, i+1));
    }
  }

  //Undefined varName for range(x, y)
  else if (_rangeList.size() == 2){
    //Init value passed in
    rListInit = _rangeList[0]->evaluate(symTab);
    intInit = dynamic_cast<IntegerDescriptor *> (rListInit);
    //Range value passed in
    rListRange = _rangeList[1]->evaluate(symTab);
    intRange = dynamic_cast<IntegerDescriptor *> (rListRange);
      
    symTab.setValueFor(_varName, intInit);
    for (int i = intInit->getIntValue(); i < intRange->getIntValue(); i++){
      _forStmts->evaluate(symTab);
      if (i+1 != intRange->getIntValue())
	symTab.setValueFor(_varName, intVar = new IntegerDescriptor(TypeDescriptor::INTEGER, i+1));
    }
  } 
    
  else if (_rangeList.size() == 3){

    //Init value passed in
    rListInit = _rangeList[0]->evaluate(symTab);
    intInit = dynamic_cast<IntegerDescriptor *> (rListInit);
    //Range value passed in
    rListRange = _rangeList[1]->evaluate(symTab);
    intRange = dynamic_cast<IntegerDescriptor *> (rListRange);
    //Step value passed in
    rListStep = _rangeList[2]->evaluate(symTab);
    intStep = dynamic_cast<IntegerDescriptor *> (rListStep);

    symTab.setValueFor(_varName, intInit);
    for (int i = intInit->getIntValue(); i < intRange->getIntValue(); i += intStep->getIntValue()){
      _forStmts->evaluate(symTab);
      if(i+1 != intRange->getIntValue())
	symTab.setValueFor(_varName, intVar = new IntegerDescriptor(TypeDescriptor::INTEGER, i + intStep->getIntValue()));
    }
  }

  else{
    std::cout << "Invalid use of For Statement" << std::endl;
    exit(2);
  }
}

void ForStatement::print(){
  std::cout << "for " << _varName << " in range(";
  for (int i = 0; i < _rangeList.size()-1; i++){
    _rangeList[i]->print();
    std::cout << ", ";
  }
  _rangeList[_rangeList.size()-1]->print();
  std::cout << "):" << std::endl;
  _forStmts -> print(); 
}
