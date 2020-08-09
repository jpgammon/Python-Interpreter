//
// Created by Ali A. Kooshesh on 1/30/19.
//

#include <iostream>
#include <string>
#include <stack>
#include "Tokenizer.hpp"

int indentCount = 0;
int dedentCount = 0;

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

Tokenizer::Tokenizer(std::ifstream &stream): ungottenToken{false}, inStream{stream}, lastToken{}, parsingANewLine{true} {}

Token Tokenizer::getToken() {

  if (indentS.empty())
    indentS.push(0);

  if(ungottenToken) {
    ungottenToken = false;
    return lastToken;
  }
  
  char c;
  int spaceCount = 0;
  Token tok; 
  std::stack<int> indentSCopy;
  bool indentFail = false;
  
  while( inStream.get(c) && isspace(c) && c != '\n' ){  // Skip spaces but not new-line chars.
      spaceCount++;
  }
 
  if (parsingANewLine == true && c != '\n' && c != '#'){
    //Case 1 -- if amount of spaces is == to top element on stack.
    //Same indentation level.
    if (spaceCount == indentS.top()){
      parsingANewLine = false;
    }
    //Case 2 -- if amount of spaces is > top element on stack.
    //Higher indentation level.
    else if (spaceCount > indentS.top()){
      indentS.push(spaceCount);
      tok.isIndent() = true;
      parsingANewLine = false;
      inStream.putback(c);
      _tokens.push_back(tok);
      indentCount++;
      return lastToken = tok;
    }
    //Case 3 -- if amount of spaces is < top element of stack.
    //Lower Indentation level.
    else if (indentS.top() > spaceCount){      
      indentS.pop();
      //Occurs when there is a drop in indentation levels greater than 1.
      if (indentS.top() != spaceCount){
	std::cout << "DROP MORE THAN 1" << std::endl;
	indentSCopy = indentS;
	//Checks if indent level is part of stack. 
	while (indentSCopy.size() != 0){
	  std::cout << "WHILE" << std::endl;
	  if (indentSCopy.top() == spaceCount){
      	    indentS.pop();
	    tok.isDedent() = true;
	    dedentCount++; 
	    parsingANewLine = false;
	    _tokens.push_back(tok);
	    indentFail = false;
	    break;
	  }
	  else{
	    indentSCopy.pop();
	    indentFail = true;
	  }
	}
	if (indentFail){
	  std::cout << "Invalid indentation." << std::endl;
	  exit(2);
	}
	else{
	  tok.isDedent() = true;
	  for (int i = 0; i < dedentCount; i++){
	    indentS.pop();
	    _tokens.push_back(tok);
	  }
	  inStream.putback(c);
	  parsingANewLine = false;
	  return lastToken = tok;	
	}
      }
      //Normal case if second elem in stack is == number of indent spaces
      else{
	indentS.pop();
	tok.isDedent() = true;
	dedentCount++;
	parsingANewLine = false;
	inStream.putback(c);
	_tokens.push_back(tok);
	return lastToken = tok;
      }
    }      
  
    //If none of the cases apply. 
    else{
      std::cout << "Error." << std::endl;
      exit(2);
    }
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
  std::string checkReserved;

  if( inStream.eof()) {  
    if (indentS.size() > 1 || !parsingANewLine){
      indentS.pop();
      token.isDedent() = true;
      dedentCount++;
      token.eof() = true;
      _tokens.push_back(token);
      parsingANewLine = false;
    }
    
    else{
	token.isDedent() = true;
	dedentCount++;
	token.eof() = true;
	parsingANewLine = false;
	_tokens.push_back(token);
	return lastToken = token;
    } 
  }
 
  else if( c == '\n' ) {  // will not ever be the case unless new-line characters are not supressed.
    token.eol() = true;
    parsingANewLine = true;    
  }
  
  //Reads in Strings
  else if( c == '"' ) {
    token.setStringValue( readString() ); 
  }

  //Reads in Integers and Doubles
  else if( isdigit(c) ) {// an integer?
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
  //Tests for "<", "<=", or "<>"
  else if (c == '<' ){       
    p = inStream.peek();
    if (p == '='){
      rOp = "<=";
      token.relationalOp(rOp);
      inStream.get(c);
    }
    else if (p == '>'){
      rOp = "<>";
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
  
  else if( c == ';' || c == ':')
    token.symbol(c);
  else if( c == '(' || c == ')')
    token.symbol(c);
  else if (c == '{' || c == '}')
    token.symbol(c); 
  else if(isalpha(c)) {  // an identifier?
    // put c back into the stream so we can read the entire name in a function.
    inStream.putback(c);
    
    checkReserved = readName();
    if (checkReserved == "and" || checkReserved == "or" || checkReserved == "not")
      token.setLogicalOp(checkReserved);
    else
      if (!(token.isDedent() && token.isIndent())){
	token.setName( checkReserved );
	parsingANewLine = false;
      }
    
  } else {
    //    parsingANewLine = true;
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
