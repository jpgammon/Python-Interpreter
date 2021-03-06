//
// Created by Ali A. Kooshesh on 1/30/19.
//

#ifndef EXPRINTER_TOKEN_HPP
#define EXPRINTER_TOKEN_HPP
#include<string>

class Token {

public:
  Token();

  bool &eof()  { return _eof; }  //eof = end of file
  bool &eol()  { return _eol; }  //eol = end of line

    bool eof() const { return _eof; }
    bool eol() const  { return _eol; }

    bool isOpenParen() const  { return _symbol == '('; }
    bool isCloseParen() const { return _symbol == ')'; }

  bool isOpenCurly() const { return _symbol == '{'; }
  bool isCloseCurly() const { return _symbol == '}'; }

    void symbol(char c) { _symbol = c; }
  //Relational
  void relationalOp(std::string s) {_relationalOp = s; }
    char symbol() { return _symbol; }

    bool isSemiColon() const { return _symbol == ';'; }
    bool isAssignmentOperator() const              { return _symbol == '='; }
    bool isMultiplicationOperator() const { return _symbol == '*'; }
    bool isAdditionOperator() const       { return _symbol == '+'; }
    bool isSubtractionOperator() const    { return _symbol == '-'; }
    bool isModuloOperator() const         { return _symbol == '%'; }
    bool isDivisionOperator() const       { return _symbol == '/'; }
    bool isArithmeticOperator() const {
        return isMultiplicationOperator() ||
               isAdditionOperator() ||
               isSubtractionOperator() ||
               isModuloOperator() ||
               isDivisionOperator();
    }
    //Relational Operators
  
    bool isEquivalent()     const  { return _relationalOp == "=="; }
    bool isNotEquivalent()  const { return _relationalOp == "!="; }
    bool isGreaterOrEqual() const { return _relationalOp == ">="; }
    bool isLessOrEqual()    const { return _relationalOp == "<="; }
    bool isGreaterThan()    const { return _relationalOp == ">"; }
    bool isLessThan()       const { return _relationalOp == "<"; }
    
    bool isRelationalOperator() const {
        return isGreaterThan() ||
      	       isLessThan();
	       isEquivalent() ||
               isNotEquivalent() ||
               isGreaterOrEqual() ||
               isLessOrEqual();
    }
  
  
    bool isName() const { return _name.length() > 0; }
    std::string getName() const                  { return _name; }
    void setName(std::string n) { _name = n; }

    bool &isWholeNumber() { return _isWholeNumber; }
    bool isWholeNumber() const { return _isWholeNumber; }
    int getWholeNumber() const { return _wholeNumber; }
    void setWholeNumber(int n) {
        _wholeNumber = n;
        isWholeNumber() = true;
    }

  //Relational Operators getter/setter
  std::string getRelationalOp() const { return _relationalOp; }
  void setRelationalOp(std::string s){ _relationalOp = s; }

  //Get KeyWord function
  bool isKeyWord(){
    return (_name == "print" || _name == "for");     
  }
  
  void print() const;

private:
    std::string _name;
    bool _eof, _eol;
    bool _isWholeNumber;
    char _symbol;
    int _wholeNumber;
    std::string _relationalOp;
};

#endif //EXPRINTER_TOKEN_HPP
