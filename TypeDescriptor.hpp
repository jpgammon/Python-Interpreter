#ifndef EXPRINTER_TYPEDESCRIPTOR_HPP
#define EXPRINTER_TYPEDESCRIPTOR_HPP

#include <iostream>

//#include "ArithExpr.hpp"
//#include "SymTab.hpp"
//#include "Statements.hpp"

class TypeDescriptor {   
public:
  enum types {
      INTEGER, DOUBLE, STRING};

  TypeDescriptor(types type);
  types &type();
  virtual ~TypeDescriptor(){}

private:
  types _type;
};

//INTEGER
class IntegerDescriptor : public TypeDescriptor{
public:

  IntegerDescriptor(types descType, int intValue);
  int getIntValue();
  int setIntValue(int set);

private:
    int _intValue;
    
};

//DOUBLES
class DoubleDescriptor : public TypeDescriptor {
public:
    DoubleDescriptor(types descType, double doubleValue);
    double getDoubleValue();

private:
    double _doubleValue;
};

//STRINGS
class StringDescriptor : public TypeDescriptor{
public:
  StringDescriptor(types descType, std::string stringValue);
  std::string getStringValue();

private:
  std::string _stringValue;
};

#endif //EXPRINTER_TYPEDESCRIPTOR_HPP
