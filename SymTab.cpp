//
// Created by Ali A. Kooshesh on 2/5/19.
//

#include <iostream>
#include "SymTab.hpp"

void SymTab::setValueFor(std::string vName, TypeDescriptor* type) {

  symTab[vName] = type;

  // Define a variable by setting its initial value.
  if((symTab.find(vName)->second)->type() == TypeDescriptor::INTEGER){
    IntegerDescriptor *integerDescriptor = dynamic_cast<IntegerDescriptor *> (symTab.find(vName)->second);
    std::cout << vName << " <- " << integerDescriptor->getIntValue() << std::endl;
  }
  else if ((symTab.find(vName)->second)->type() == TypeDescriptor::STRING){
    StringDescriptor *stringDescriptor = dynamic_cast<StringDescriptor *> (symTab.find(vName)->second);
    std::cout << vName << " <- " << stringDescriptor->getStringValue() << std::endl;
  }
  else if ((symTab.find(vName)->second)->type() == TypeDescriptor::DOUBLE){
    DoubleDescriptor *doubleDescriptor = dynamic_cast<DoubleDescriptor *> (symTab.find(vName)->second);
    std::cout << vName << " <- " << doubleDescriptor->getDoubleValue() << std::endl;
  }
  else{
    std::cout << "SymTab::setValueFor: Unable to set value" << std::endl;
    exit(2);
  }
}

bool SymTab::isDefined(std::string vName) {
    return symTab.find(vName) != symTab.end();
}

TypeDescriptor* SymTab::getValueFor(std::string vName) {

  if(!isDefined(vName)) {
    std::cout << "SymTab::getValueFor: " << vName << " has not been defined.\n";
    exit(1);
  }
   
  else if((symTab.find(vName)->second)->type() == TypeDescriptor::INTEGER){
    IntegerDescriptor *integerDescriptor = dynamic_cast<IntegerDescriptor *> (symTab.find(vName)->second);
    std::cout << "SymTab::getValueFor: " << vName << " contains " << integerDescriptor->getIntValue() << std::endl;
    std::cout << integerDescriptor->getIntValue() << std::endl;
    return symTab.find(vName)->second;    
  }

  else if ((symTab.find(vName)->second)->type() == TypeDescriptor::STRING){
    StringDescriptor *stringDescriptor = dynamic_cast<StringDescriptor *> (symTab.find(vName)->second);
    std::cout << "SymTab::getValueFor: " << vName << " contains " << stringDescriptor->getStringValue() << std::endl;
    std::cout << stringDescriptor->getStringValue() << std::endl;
    return symTab.find(vName)->second;
  }

  else if ((symTab.find(vName)->second)->type() == TypeDescriptor::DOUBLE){
    DoubleDescriptor *doubleDescriptor = dynamic_cast<DoubleDescriptor *> (symTab.find(vName)->second);
    std::cout << "SymTab::getValueFor: " << vName << " contains " << doubleDescriptor->getDoubleValue() << std::endl;
    std::cout << doubleDescriptor->getDoubleValue() << std::endl;
    return symTab.find(vName)->second;
  }
  else{
    std::cout << "SymTab::getValueFor: Unable to get value for " << vName << std::endl;
    exit(2);
  }
}

void SymTab::print() {
    for(auto [var, value] : symTab ) {

        if (value->type() == TypeDescriptor::INTEGER) {
            IntegerDescriptor *integerDescriptor = dynamic_cast<IntegerDescriptor *> (value);
            std::cout << var << " = " << integerDescriptor->getIntValue() << std::endl;
        }
	else if (value->type() == TypeDescriptor::DOUBLE) {
            DoubleDescriptor *doubleDescriptor = dynamic_cast<DoubleDescriptor *> (value);
            std::cout << var << " = " << doubleDescriptor->getDoubleValue() << std::endl;
        }
	
	else if (value->type() == TypeDescriptor::STRING) {
            StringDescriptor *stringDescriptor = dynamic_cast<StringDescriptor *> (value);
            std::cout << var << " = " << stringDescriptor->getStringValue() << std::endl;
        }
	else{
	  std::cout << "error" << std::endl;
	}

    }
}

