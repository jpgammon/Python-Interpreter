//
// Created by student on 2020-03-11.
//

#include "TypeDescriptor.hpp"

TypeDescriptor::TypeDescriptor(TypeDescriptor::types type): _type{type} {}

TypeDescriptor::types &TypeDescriptor::type() {
    return _type;
}

//INTEGERS
IntegerDescriptor::IntegerDescriptor(TypeDescriptor::types descType, int intValue): TypeDescriptor{descType} , _intValue{intValue} {}

int IntegerDescriptor::getIntValue() {
    return  _intValue;
}

int IntegerDescriptor::setIntValue(int set) {
  _intValue = set;
  return _intValue;
}

//DOUBLES
DoubleDescriptor::DoubleDescriptor(TypeDescriptor::types descType, double doubleValue): TypeDescriptor{descType} , _doubleValue{doubleValue} {}

double DoubleDescriptor::getDoubleValue() {
    return  _doubleValue;
}

//STRINGS
StringDescriptor::StringDescriptor(TypeDescriptor::types descType, std::string stringValue): TypeDescriptor{descType} , _stringValue{stringValue} {}

std::string StringDescriptor::getStringValue() {
    return  _stringValue;
}

