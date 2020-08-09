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
  
  //Relational Operators
  void relationalOp(std::string s) {_relationalOp = s; }
  //Logical Operators
  void logicalOp(std::string s) {_logicalOp = s; }
  
  char symbol() { return _symbol; }

  bool isSemiColon() const { return _symbol == ';'; }
  bool isColon() const { return _symbol == ':'; }
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
  
  //Misc
  bool isPoundSign() const { return _symbol == '#'; }
  bool isDecimalPoint() const { return _symbol == '.'; }
  bool isComma() const { return _symbol == ','; }

  //SPECIAL OPERATORS
  //Relational Operators
  bool isEquivalent()     const { return _relationalOp == "=="; }
  bool isNotEquivalent()  const { return _relationalOp == "!="; }
  bool isGreaterOrEqual() const { return _relationalOp == ">="; }
  bool isLessOrEqual()    const { return _relationalOp == "<="; }
  bool isGreaterThan()    const { return _relationalOp == ">"; }
  bool isLessThan()       const { return _relationalOp == "<"; }
  bool isNotEquivalent2() const { return _relationalOp == "<>"; }
  bool isRelationalOperator() const {
    return isGreaterThan() ||
      isLessThan() ||
      isEquivalent() ||
      isNotEquivalent() ||
      isNotEquivalent2() ||
      isGreaterOrEqual() ||
      isLessOrEqual();
  }

  //Logical Operators
  bool isAndOperator()    const { return _logicalOp == "and"; }
  bool isOrOperator()     const { return _logicalOp == "or"; }
  bool isNotOperator()    const { return _logicalOp == "not"; }
  bool isLogicalOperator() const {
    return isAndOperator() ||
      isOrOperator() ||
      isNotOperator(); 	 
  }
  
  bool isName() const { return _name.length() > 0; }
  std::string getName() const { return _name; }
  void setName(std::string n) { _name = n; }

  bool &isWholeNumber() { return _isWholeNumber; }
  bool isWholeNumber() const { return _isWholeNumber; }
  int getWholeNumber() const { return _wholeNumber; }
  void setWholeNumber(int n) {
    _wholeNumber = n;
    isWholeNumber() = true;
  }

  bool &isDoubleValue() { return _isDoubleValue; }
  bool isDoubleValue() const { return _isDoubleValue; }
  double getDoubleValue() const { return _doubleValue; }
  void setDoubleValue(double d){
    _doubleValue = d;
    isDoubleValue() = true;    
  }

  bool &isStringValue() { return _isStringValue; }
  bool isStringValue() const { return _isStringValue; }
  std::string getStringValue() const { return _stringValue; }
  void setStringValue(std::string s){
    _stringValue = s;
    isStringValue() = true;
  }
  
  //Relational Operators getter/setter
  std::string getRelationalOp() const { return _relationalOp; }
  void setRelationalOp(std::string s){ _relationalOp = s; }

  //Logical Operators getter/setter
  std::string getLogicalOp() const { return _logicalOp; }
  void setLogicalOp(std::string s){ _logicalOp = s; }
  bool isLogicalOp(){
    return (_logicalOp == "and" || _logicalOp == "or" || _logicalOp == "not");
  }
  
  //Get KeyWord function
  bool isKeyWord(){
    return (_name == "print" || _name == "for" || _name == "if");     
  }
  //Get SecondaryKeyWord function (elif, else)
  bool isSecondaryKeyWord(){
    return (_name == "elif" || _name == "else");
  }
  
  void print() const;

  //Indentation
  bool isIndent() const{ return _indent; };
  bool isDedent() const{ return _dedent; };

  bool &isIndent() { return _indent; };
  bool &isDedent() { return _dedent; };
  
private:
    std::string _name;
    bool _eof, _eol;
    bool _isWholeNumber;
    bool _isDoubleValue;
    bool _isStringValue;
    char _symbol;
    int _wholeNumber;
    double _doubleValue;
    std::string _stringValue;
    std::string _relationalOp;
    std::string _logicalOp;
    bool _indent;
    bool _dedent;
};

#endif //EXPRINTER_TOKEN_HPP
