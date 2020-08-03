//
// Created by Ali A. Kooshesh on 2/5/19.
//

#include<iostream>
#include <math.h>
#include "ArithExpr.hpp"

// ExprNode
ExprNode::ExprNode(Token token): _token{token} {}

Token ExprNode::token() { return _token; }

// InfixExprNode functions
InfixExprNode::InfixExprNode(Token tk) : ExprNode{tk}, _left(nullptr), _right(nullptr) {}

ExprNode *&InfixExprNode::left() { return _left; }   //returns left pointer by reference with the *& syntax

ExprNode *&InfixExprNode::right() { return _right; }

//int InfixExprNode::evaluate(SymTab &symTab) {
TypeDescriptor *InfixExprNode::evaluate(SymTab &symTab){

    // Evaluates an infix expression using a post-order traversal of the expression tree.
    TypeDescriptor *l = left()->evaluate(symTab);
    TypeDescriptor *r = right()->evaluate(symTab);
    
    //HANDLES leftSTRING TO rightSTRING EVALUATIONS   
    if (l->type() == TypeDescriptor::STRING && r->type() == TypeDescriptor::STRING) {
        StringDescriptor *leftString = dynamic_cast<StringDescriptor *> (l);
        StringDescriptor *rightString = dynamic_cast<StringDescriptor *> (r);
	std::string lValue = leftString->getStringValue();
	std::string rValue = rightString->getStringValue();

        std::cout << "InfixExprNode::evaluate: " << lValue << " " << token().symbol() << " " << rValue << std::endl;
        if (token().isAdditionOperator())
            return new StringDescriptor(TypeDescriptor::STRING,lValue + rValue);
        //Relationals
        else if (token().isGreaterThan())
            return new IntegerDescriptor(TypeDescriptor::INTEGER,lValue > rValue);
        else if (token().isLessThan())
            return new IntegerDescriptor(TypeDescriptor::INTEGER,lValue < rValue);
        else if (token().isGreaterOrEqual())
            return new IntegerDescriptor(TypeDescriptor::INTEGER,lValue >= rValue);
        else if (token().isLessOrEqual())
            return new IntegerDescriptor(TypeDescriptor::INTEGER,lValue <= rValue);
        else if (token().isEquivalent())
            return new IntegerDescriptor(TypeDescriptor::INTEGER,lValue == rValue);
        else if (token().isNotEquivalent())
            return new IntegerDescriptor(TypeDescriptor::INTEGER,lValue != rValue);

        else {
            std::cout << "InfixExprNode::evaluate: don't know how to evaluate this operator\n";
            token().print();
            std::cout << std::endl;
            exit(2);
        }
    }
    
    //HANDLES leftSTRING TO rightDOUBLE EVALUATIONS
    if (l->type() == TypeDescriptor::STRING && r->type() == TypeDescriptor::DOUBLE){
      StringDescriptor *leftString = dynamic_cast<StringDescriptor *> (l);
      DoubleDescriptor *rightDouble = dynamic_cast<DoubleDescriptor *> (r);
      std::string lValue = leftString->getStringValue();
      double rValue = rightDouble->getDoubleValue();

      std::cout << "InfixExprNode::evaluate: Mismatch Operators\n" << std::endl;
      exit(2);
    }

    
    //HANDLES leftDOUBLE TO rightSTRING EVALUATIONS
    else if (l->type() == TypeDescriptor::DOUBLE && r->type() == TypeDescriptor::STRING){
      DoubleDescriptor *leftDouble = dynamic_cast<DoubleDescriptor *> (l);
      StringDescriptor *rightString = dynamic_cast<StringDescriptor *> (r);
      double lValue = leftDouble->getDoubleValue();
      std::string rValue = rightString->getStringValue();

      std::cout << "InfixExprNode::evaluate: Mismatch Operators\n" << std::endl;
      exit(2);
    }

    //HANDLES leftSTRING TO rightINTEGER EVALUATIONS
    else if (l->type() == TypeDescriptor::STRING && r->type() == TypeDescriptor::INTEGER){
      StringDescriptor *leftString = dynamic_cast<StringDescriptor *> (l);
      IntegerDescriptor *rightInteger = dynamic_cast<IntegerDescriptor *> (r);
      std::string lValue = leftString->getStringValue();
      int rValue = rightInteger->getIntValue();

      std::cout << "InfixExprNode::evaluate: Mismatch Operators\n" << std::endl;
      exit(2);
    }

    //HANDLES leftINTEGER TO rightSTRING EVALUATIONS
    else if (l->type() == TypeDescriptor::INTEGER && r->type() == TypeDescriptor::STRING){
      IntegerDescriptor *leftInteger = dynamic_cast<IntegerDescriptor *> (l);
      StringDescriptor *rightString = dynamic_cast<StringDescriptor *> (r);
      int lValue = leftInteger->getIntValue();
      std::string rValue = rightString->getStringValue();

      std::cout << "InfixExprNode::evaluate: Mismatch Operators\n" << std::endl;
      exit(2);
    }
    
    //HANDLES leftINTEGER TO rightDOUBLE EVALUATIONS
    else if (l->type() == TypeDescriptor::INTEGER && r->type() == TypeDescriptor::DOUBLE){
      IntegerDescriptor *leftInteger = dynamic_cast<IntegerDescriptor *> (l);
      DoubleDescriptor *rightDouble = dynamic_cast<DoubleDescriptor *> (r);
      int lValue = leftInteger->getIntValue();
      double rValue = rightDouble->getDoubleValue();
    
     std::cout << "InfixExprNode::evaluate: " << lValue << " " << token().symbol() << " " << rValue << std::endl;
        if (token().isAdditionOperator())
            return new DoubleDescriptor(TypeDescriptor::DOUBLE,lValue + rValue);
        else if (token().isSubtractionOperator())
            return new DoubleDescriptor(TypeDescriptor::DOUBLE, lValue - rValue);
        else if (token().isMultiplicationOperator())
            return new DoubleDescriptor(TypeDescriptor::DOUBLE,lValue * rValue);
        else if (token().isDivisionOperator())
            return new DoubleDescriptor(TypeDescriptor::DOUBLE,lValue / rValue); // division by zero?
	else if (token().isModuloOperator())
	  return new DoubleDescriptor(TypeDescriptor::DOUBLE, fmod(lValue, rValue));
	//Relationals
        else if (token().isGreaterThan())
            return new IntegerDescriptor(TypeDescriptor::INTEGER,lValue > rValue);
        else if (token().isLessThan())
            return new IntegerDescriptor(TypeDescriptor::INTEGER,lValue < rValue);
        else if (token().isGreaterOrEqual())
            return new IntegerDescriptor(TypeDescriptor::INTEGER,lValue >= rValue);
        else if (token().isLessOrEqual())
            return new IntegerDescriptor(TypeDescriptor::INTEGER,lValue <= rValue);
        else if (token().isEquivalent())
            return new IntegerDescriptor(TypeDescriptor::INTEGER,lValue == rValue);
        else if (token().isNotEquivalent())
            return new IntegerDescriptor(TypeDescriptor::INTEGER,lValue != rValue);

        else {
            std::cout << "InfixExprNode::evaluate: don't know how to evaluate this operator\n";
            token().print();
            std::cout << std::endl;
            exit(2);
        }
    }
    
    //HANDLES leftDOUBLE TO rightINTEGER EVALUATIONS
    else if (l->type() == TypeDescriptor::DOUBLE && r->type() == TypeDescriptor::INTEGER){
      DoubleDescriptor *leftDouble = dynamic_cast<DoubleDescriptor *> (l);
      IntegerDescriptor *rightInteger = dynamic_cast<IntegerDescriptor *> (r);
      double lValue = leftDouble->getDoubleValue();
      int rValue = rightInteger->getIntValue();

       std::cout << "InfixExprNode::evaluate: " << lValue << " " << token().symbol() << " " << rValue << std::endl;
        if (token().isAdditionOperator())
            return new DoubleDescriptor(TypeDescriptor::DOUBLE,lValue + rValue);
        else if (token().isSubtractionOperator())
            return new DoubleDescriptor(TypeDescriptor::DOUBLE, lValue - rValue);
        else if (token().isMultiplicationOperator())
            return new DoubleDescriptor(TypeDescriptor::DOUBLE,lValue * rValue);
        else if (token().isDivisionOperator())
            return new DoubleDescriptor(TypeDescriptor::DOUBLE,lValue / rValue); // division by zero?
	else if (token().isModuloOperator())
	  return new DoubleDescriptor(TypeDescriptor::DOUBLE, fmod(lValue, rValue));
	//Relationals
        else if (token().isGreaterThan())
            return new IntegerDescriptor(TypeDescriptor::INTEGER,lValue > rValue);
        else if (token().isLessThan())
            return new IntegerDescriptor(TypeDescriptor::INTEGER,lValue < rValue);
        else if (token().isGreaterOrEqual())
            return new IntegerDescriptor(TypeDescriptor::INTEGER,lValue >= rValue);
        else if (token().isLessOrEqual())
            return new IntegerDescriptor(TypeDescriptor::INTEGER,lValue <= rValue);
        else if (token().isEquivalent())
            return new IntegerDescriptor(TypeDescriptor::INTEGER,lValue == rValue);
        else if (token().isNotEquivalent())
            return new IntegerDescriptor(TypeDescriptor::INTEGER,lValue != rValue);

        else {
            std::cout << "InfixExprNode::evaluate: don't know how to evaluate this operator\n";
            token().print();
            std::cout << std::endl;
            exit(2);
        }
      }

      
    //HANDLES leftDOUBLE TO rightDOUBLE EVALUATIONS
    else if (l->type() == TypeDescriptor::DOUBLE && r->type() == TypeDescriptor::DOUBLE) {
        DoubleDescriptor *leftDouble = dynamic_cast<DoubleDescriptor *> (l);
        DoubleDescriptor *rightDouble = dynamic_cast<DoubleDescriptor *> (r);
        double lValue = leftDouble->getDoubleValue();
        double rValue = rightDouble->getDoubleValue();

        std::cout << "InfixExprNode::evaluate: " << lValue << " " << token().symbol() << " " << rValue << std::endl;
        if (token().isAdditionOperator())
            return new DoubleDescriptor(TypeDescriptor::DOUBLE,lValue + rValue);
        else if (token().isSubtractionOperator())
            return new DoubleDescriptor(TypeDescriptor::DOUBLE, lValue - rValue);
        else if (token().isMultiplicationOperator())
            return new DoubleDescriptor(TypeDescriptor::DOUBLE,lValue * rValue);
        else if (token().isDivisionOperator())
            return new DoubleDescriptor(TypeDescriptor::DOUBLE,lValue / rValue); // division by zero?
	else if (token().isModuloOperator())
	  return new DoubleDescriptor(TypeDescriptor::DOUBLE, fmod(lValue, rValue));
	//Relationals
        else if (token().isGreaterThan())
            return new IntegerDescriptor(TypeDescriptor::INTEGER,lValue > rValue);
        else if (token().isLessThan())
            return new IntegerDescriptor(TypeDescriptor::INTEGER,lValue < rValue);
        else if (token().isGreaterOrEqual())
            return new IntegerDescriptor(TypeDescriptor::INTEGER,lValue >= rValue);
        else if (token().isLessOrEqual())
            return new IntegerDescriptor(TypeDescriptor::INTEGER,lValue <= rValue);
        else if (token().isEquivalent())
            return new IntegerDescriptor(TypeDescriptor::INTEGER,lValue == rValue);
        else if (token().isNotEquivalent())
            return new IntegerDescriptor(TypeDescriptor::INTEGER,lValue != rValue);

        else {
            std::cout << "InfixExprNode::evaluate: don't know how to evaluate this operator\n";
            token().print();
            std::cout << std::endl;
            exit(2);
        }
    }


    //HANDLES leftINT TO rightINT EVALUATIONS
    else if (l->type() == TypeDescriptor::INTEGER && r->type() == TypeDescriptor::INTEGER) {
        IntegerDescriptor *leftInt = dynamic_cast<IntegerDescriptor *> (l);
        IntegerDescriptor *rightInt = dynamic_cast<IntegerDescriptor *> (r);
        int lValue = leftInt->getIntValue();
        int rValue = rightInt->getIntValue();

        std::cout << "InfixExprNode::evaluate: " << lValue << " " << token().symbol() << " " << rValue << std::endl;
        if (token().isAdditionOperator())
            return new IntegerDescriptor(TypeDescriptor::INTEGER,lValue + rValue);
        else if (token().isSubtractionOperator())
            return new IntegerDescriptor(TypeDescriptor::INTEGER, lValue - rValue);
        else if (token().isMultiplicationOperator())
            return new IntegerDescriptor(TypeDescriptor::INTEGER,lValue * rValue);
        else if (token().isDivisionOperator())
            return new IntegerDescriptor(TypeDescriptor::INTEGER,lValue / rValue); // division by zero?
        else if (token().isModuloOperator())
            return new IntegerDescriptor(TypeDescriptor::INTEGER,lValue % rValue);
            //Relationals
        else if (token().isGreaterThan())
            return new IntegerDescriptor(TypeDescriptor::INTEGER,lValue > rValue);
        else if (token().isLessThan())
            return new IntegerDescriptor(TypeDescriptor::INTEGER,lValue < rValue);
        else if (token().isGreaterOrEqual())
            return new IntegerDescriptor(TypeDescriptor::INTEGER,lValue >= rValue);
        else if (token().isLessOrEqual())
            return new IntegerDescriptor(TypeDescriptor::INTEGER,lValue <= rValue);
        else if (token().isEquivalent())
            return new IntegerDescriptor(TypeDescriptor::INTEGER,lValue == rValue);
        else if (token().isNotEquivalent())
            return new IntegerDescriptor(TypeDescriptor::INTEGER,lValue != rValue);

        else {
            std::cout << "InfixExprNode::evaluate: don't know how to evaluate this operator\n";
            token().print();
            std::cout << std::endl;
            exit(2);
        }
    }

    else {
      std::cout << "InfixExprNode::evaluate: don't know how to evaluate this operator\n";
      token().print();
      std::cout << std::endl;
      exit(2);
    }
}

void InfixExprNode::print() {
    _left->print();
    token().print();
    _right->print();
}

// WholeNumber -- token has the number in it. Returns the number
Integer::Integer(Token token): ExprNode{token} {}

void Integer::print() {
    token().print();
}

//int WholeNumber::evaluate(SymTab &symTab) {
TypeDescriptor *Integer::evaluate(SymTab &symTab) {

  std::cout << "Integer::evaluate: returning " << token().getWholeNumber() << std::endl;
  return new IntegerDescriptor(TypeDescriptor::INTEGER,token().getWholeNumber());
}

// Variable
Variable::Variable(Token token): ExprNode{token} {}

void Variable::print() {
    token().print();
}

TypeDescriptor *Variable::evaluate(SymTab &symTab) {
    if( ! symTab.isDefined(token().getName())) {
        std::cout << "Use of undefined variable, " << token().getName() << std::endl;
        exit(1);
    }
    std::cout << "Variable::evaluate: returning ";
    symTab.getValueFor(token().getName());
    
    return symTab.getValueFor(token().getName());
}

//STRING
String::String(Token token) : ExprNode{token} {}

void String::print(){
  token().print();
}

TypeDescriptor *String::evaluate(SymTab &symTab){
    std::cout << "String::evaluate: returning " << token().getStringValue() << std::endl;
    return new StringDescriptor(TypeDescriptor::STRING, token().getStringValue());
}


//DOUBLE
Double::Double(Token token) : ExprNode{token} {}

void Double::print(){
  token().print();
}

TypeDescriptor *Double::evaluate(SymTab &symTab){
  std::cout << "Double::evaluate: returning " << token().getDoubleValue() << std::endl;
  return new DoubleDescriptor(TypeDescriptor::DOUBLE, token().getDoubleValue());
}
