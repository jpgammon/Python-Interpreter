//
// Created by Ali A. Kooshesh on 1/30/19.
//

#include <iostream>
#include <string>
#include "Tokenizer.hpp"

//int lineCount = 0;

std::string Tokenizer::readName() {
    // This function is called when it is known that
    // the first character in input is an alphabetic character.
    // The function reads and returns all characters of the name.
    std::string name;
    char c;
    while( inStream.get(c) && isalnum(c) ) {
        name += c;
    }
    if(inStream.good())  // In the loop, we have read one char too many.
        inStream.putback(c);
    return name;
}

std::string Tokenizer::readString() {

  std::string s;
  char c;

  while (inStream.get(c)){
    if (c == '"')
      return s;
    s+=c;
  }
  std::cout << "Tokenizer::readString(): Invalid string: " << s << std::endl;
  exit(1); 
 
}

std::string Tokenizer::readInteger() {
  // This function is called when it is known that
  // the first character in input is a digit.
  // The function reads and returns all remaining digits.
 
  char c;
  std::string all = "";
  
  while( inStream.get(c) && (isdigit(c) || c == '.' || c == '-')) {
    all += c;
  }

  if (inStream.good())
    inStream.putback(c);
  return all;
}

bool Tokenizer::isDouble(std::string s){
  for (int i = 0; i < s.length(); i++){
    if (s[i] == '.')
      return true;
  }
  return false;
}

Tokenizer::Tokenizer(std::ifstream &stream): ungottenToken{false}, inStream{stream}, lastToken{} {}

Token Tokenizer::getToken() {

    if(ungottenToken) {
        ungottenToken = false;
        return lastToken;
    }

    char c;
    
    while( inStream.get(c) && isspace(c) && c != '\n' ){  // Skip spaces but not new-line chars.
    }
   
    if(inStream.bad()) {
        std::cout << "Error while reading the input stream in Tokenizer.\n";
        exit(1);
    }
    std::string test;
    Token token;
    char p;
    std::string isDoubleReturn = "";
    std::string rOp;
    if( inStream.eof()) {
        token.eof() = true;
    } else if( c == '\n' ) {  // will not ever be the case unless new-line characters are not supressed.
        token.eol() = true;
    }

    //Reads in Strings
    else if( c == '"' ) {
      token.setStringValue( readString() );     
    }

    //Reads in Integers and Doubles
    else if( isdigit(c) ) {// a integer?
      // put the digit back into the input stream so
      // we read the entire number in a function
      inStream.putback(c);

      test = readInteger();
      if (isDouble(test)){
	double testD = std::stod(test);
	token.setDoubleValue(testD);
      }
      else{
	int testI = std::stoi(test);
	token.setWholeNumber (testI);
      }
    }	
    //Relational Operators
    //Tests for ">" or ">="
    else if( c == '>' ){
      p = inStream.peek();
      if (p == '='){
	rOp = ">=";
	token.relationalOp(rOp);
	inStream.get(c);
      }
      else
	token.relationalOp(">");
    }
    //Tests for "<" or "<="
    else if (c == '<' ){       
      p = inStream.peek();
      if (p == '='){
	rOp = "<=";
	token.relationalOp(rOp);
	inStream.get(c);
      }
      else
	token.relationalOp("<");
    }
    //Tests for '=' or "=="
    else if( c == '=' ){
      p = inStream.peek();
      if (p == '='){
        rOp = "==";
        token.relationalOp(rOp);
	inStream.get(c);
      }
      else
        token.symbol(c);
    }
    //Tests for "!="
    else if ( c == '!'){
      p = inStream.peek();
      if (p == '='){
	rOp = "!=";
	token.relationalOp(rOp);
	inStream.get(c);
      }
      else
	token.symbol(c);
    }
    //Tests for "#" comment symbol
    else if( c == '#')
      token.symbol(c);
    //Testts for "," in print statement
    else if( c == ',')
      token.symbol(c);
    
    else if( c == '+' || c == '*' || c == '/' || c == '%')
        token.symbol(c);

    //Handles negatives in addition to normal subtraction
    else if( c == '-' ){
      std::string intString; std::string test; std::string negationTest;
      int subCounter = 2; 
      p = inStream.peek();
      //Negative Numbers
      if (isdigit(p)){
	inStream.putback(c);
	
	test = readInteger();
	if (isDouble(test)){
	  double testD2 = std::stod(test);
	  token.setDoubleValue(testD2);
	}
	else{
	  int testI2 = std::stoi(test);
	  token.setWholeNumber (testI2);
	}	
      }
      //Multiple negation signs
      else if (p == '-'){
	inStream.get(c);
	p = inStream.peek();
	while (p == '-'){
	   inStream.get(c);
	   p = inStream.peek();
	   subCounter++;
	}
	negationTest = readInteger();

	if (isDouble(negationTest)){
          double testD3 = std::stod(negationTest);
	  if (subCounter % 2 == 1)
	    testD3 = -testD3;
          token.setDoubleValue(testD3);
        }
        else{
          int testI3 = std::stoi(negationTest);
	  if (subCounter % 2 == 1)
	    testI3 = -testI3;
          token.setWholeNumber (testI3);
        }	  
      }
      //Normal Subtraction
      else
	token.symbol(c);  
    }

    
    else if( c == ';' )
        token.symbol(c);
    else if( c == '(' || c == ')')
        token.symbol(c);
    else if (c == '{' || c == '}')
      token.symbol(c); 
    else if(isalpha(c)) {  // an identifier?
        // put c back into the stream so we can read the entire name in a function.
        inStream.putback(c);
        token.setName( readName() );
    } else {
        std::cout << "Unknown character in input. ->" << c << "<-" << std::endl;
        exit(1);
    }
    _tokens.push_back(token);
    return lastToken = token;
}

void Tokenizer::ungetToken() {
    ungottenToken = true;
}

void Tokenizer::printProcessedTokens() {
    for(auto iter = _tokens.begin(); iter != _tokens.end(); ++iter) {
        iter->print();
        std::cout << std::endl;
    }
}
