//
// Created by Ali A. Kooshesh on 1/30/19.
//

#include <iostream>
#include "Token.hpp"

Token::Token(): _name{""}, _eof{false}, _eol{false}, _symbol{'\0'}, _isWholeNumber{false}, _isDoubleValue{false}, _isStringValue{false}, _relationalOp{""} {}

void Token::print() const {
    if( eol() ) std::cout << "EOL\n" ;
    else if( eof() )                    std::cout << "EOF" ;
    else if( isOpenParen() )  std::cout << "(" ;
    else if( isCloseParen() )  std::cout << ")" ;
    else if( isOpenCurly() ) std::cout << "{" ;
    else if( isCloseCurly() ) std::cout << "}";
    else if( isAssignmentOperator() )  std::cout << " = " ;
    else if( isSemiColon() )  std::cout << ";" ;
    else if( isMultiplicationOperator() )  std::cout << " * " ;
    else if( isAdditionOperator() )  std::cout << " + " ;
    else if( isSubtractionOperator() )  std::cout << " - " ;
    else if( isModuloOperator() )  std::cout << " % " ;
    else if( isDivisionOperator() )  std::cout << " / " ;
    else if( isName() )  std::cout << getName();
    else if( isWholeNumber() ) std::cout << getWholeNumber();
    else if( isDoubleValue() ) std::cout << getDoubleValue();
    else if( isStringValue() ) std::cout << getStringValue();
    
    //Misc
    else if( isPoundSign() ) std::cout << " # " ;
    else if( isDecimalPoint() ) std::cout << " . " ;
    else if( isComma() ) std::cout << " , " ; 

    //Relational Operators
    else if ( isEquivalent() ) std::cout << " == " ;
    else if ( isNotEquivalent() ) std::cout << " != " ;
    else if ( isGreaterOrEqual() ) std::cout << " >= " ;
    else if ( isLessOrEqual() ) std::cout << " <= " ;
    else if ( isGreaterThan() ) std::cout << " > " ;
    else if ( isLessThan() ) std::cout << " < " ;
    
    
    else std::cout << "Uninitialized token.\n";
}
