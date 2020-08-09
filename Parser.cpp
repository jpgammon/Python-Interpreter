#include<vector>
#include<iostream>

#include "Token.hpp"
#include "Parser.hpp"
#include "Statements.hpp"
#include <vector>

// Parser functions
void Parser::die(std::string where, std::string message, Token &token) {
    std::cout << where << " " << message << std::endl;
    token.print();
    std::cout << std::endl;
    std::cout << "\nThe following is a list of tokens that have been identified up to this point.\n";
    tokenizer.printProcessedTokens();
    exit(1);
}

Statements *Parser::suite() {
  //Handles suites found in if statements in for statements.
  //Aimed towards handling indentation.

  Token comment = tokenizer.getToken();
  //Checks comments
  if (comment.isPoundSign())
    {
      Token get = tokenizer.getToken();
      while(!get.eol())
        get = tokenizer.getToken();
      tokenizer.ungetToken();
      }
  else if (comment.eol()){
    while (comment.eol())
      comment = tokenizer.getToken();
    tokenizer.ungetToken();
  }
  else
    tokenizer.ungetToken();

  Token inTok = tokenizer.getToken();
  if (!inTok.isIndent())
    die("Parser::suite", "Expected an Indent Token, instead got", inTok);

  comment = tokenizer.getToken();
  //Checks comments
  if (comment.isPoundSign())
    {
      Token get = tokenizer.getToken();
      while(!get.eol())
        get = tokenizer.getToken();
      tokenizer.ungetToken();
    }
  else if (comment.eol()){
    while (comment.eol())
      comment = tokenizer.getToken();
    tokenizer.ungetToken();
  }
  else
    tokenizer.ungetToken();
  
  Statements *suiteStmts = statements();

  comment = tokenizer.getToken();
  //Checks comments
  if (comment.isPoundSign())
    {
      Token get = tokenizer.getToken();
      while(!get.eol())
        get = tokenizer.getToken();
      tokenizer.ungetToken();
      }
  else if (comment.eol()){
    while (comment.eol())
      comment = tokenizer.getToken();
    tokenizer.ungetToken();
  }
  else
    tokenizer.ungetToken();

  
  Token deTok = tokenizer.getToken();
  if (!deTok.isDedent())
    die("Parser::suite", "Expected a Dedent Token, instead got", deTok);

  comment = tokenizer.getToken();
  //Checks comments
  if (comment.isPoundSign())
    {
      Token get = tokenizer.getToken();
      while(!get.eol())
        get = tokenizer.getToken();
      tokenizer.ungetToken();
    }
  else if (comment.eol()){
    while (comment.eol())
      comment = tokenizer.getToken();
    tokenizer.ungetToken();
  }
  else
    tokenizer.ungetToken();
  
  return suiteStmts;
}

Statements *Parser::statements() {
    // This function is called when we KNOW that we are about to parse
    // a series of assignment statements.

    // This function parses the grammar rules:

    // <statement> -> <assignStatement> <statement> 
    // <statement> -> Epsilon ------ empty program
  
    Statements *stmts = new Statements();
    Token tok = tokenizer.getToken();
    //Checks comments
    if (tok.isPoundSign())
      {
        while(!tok.eol()){
          tok = tokenizer.getToken();
	}
      }
    while (tok.eol()){
      tok = tokenizer.getToken();
      if (tok.isPoundSign()){
	while(!tok.eol())
	  tok = tokenizer.getToken();
      }
    }
   
    while (tok.isName()) {                      
      //Checks KeyWord()
      if (tok.isKeyWord()){
	//Checks Print
	if (tok.getName() == "print"){
	  PrintStatement *printStmt = printStatement();
	  stmts->addStatement(printStmt);
	  tok = tokenizer.getToken();
       	}
	//For loop
	else if(tok.getName() == "for") {
	  ForStatement *forStmt = forStatement();
	  stmts->addStatement(forStmt);
	  tok = tokenizer.getToken();
	}
	else if(tok.getName() == "if") {
	  IfStatement *ifStmt = ifStatement();
	  stmts->addStatement(ifStmt);
	  tok = tokenizer.getToken();
	}
	//Neither For nor Print
	else{ die("Parser::statements", "Expected a KeyWord token, instead got", tok); }
      }
      
      else {
      tokenizer.ungetToken();
      AssignmentStatement *assignStmt = assignStatement();
      stmts->addStatement(assignStmt);
      tok = tokenizer.getToken();
      }

      //Handles comments
      if (!tok.eol()){
	if (tok.isPoundSign()){
	  while (!tok.eol())
	    tok = tokenizer.getToken();
	}
      }
      while (tok.eol()){
	tok = tokenizer.getToken();
	if (tok.isPoundSign()){
	  while(!tok.eol())
	    tok=tokenizer.getToken();
	}
      }      
    }
    tokenizer.ungetToken();
    return stmts;
}

IfStatement *Parser::ifStatement() {
  // Parses the following grammar rule
  //
  // <if_stmt> -> 'if' <test> ':' <suite> {'elif' <test> ':' <suite>}* ['else' ':' <suite>]

  //Initialization for else statements
  Statements *elseStmts;
  bool elseStmtsBool = false;
  //Initializations for elif statements
  std::vector<ExprNode *> elifExprs;
  std::vector<Statements *> elifStmts;
  
  //<test>
  ExprNode *ifExpr = or_Test();

  //':'
  Token ifColon = tokenizer.getToken();
  if (!ifColon.isColon())
    die("Parser::ifStatement", "Expected a colon, instead got", ifColon);
  
  Token nL = tokenizer.getToken();
  //Checks comments
  if (nL.isPoundSign())
    {
      Token get = tokenizer.getToken();
      while(!get.eol())
	get = tokenizer.getToken();
      tokenizer.ungetToken();
    }  
  else if (!nL.eol())
    die("Parser::ifStatement", "Expected a New Line Character, instead got", nL);

  //<suite> or statements
  Statements *ifStmts = suite();

  Token eCheck = tokenizer.getToken();
  if (eCheck.isSecondaryKeyWord()){  
    //HANDLES N AMOUNT OF ELIF STATEMENTS FOLLOWING AN IF STATEMENT
    if (eCheck.getName() == "elif"){

      while (eCheck.getName() == "elif"){
	//elif <test>
	ExprNode *elifExpr = or_Test();
	elifExprs.push_back(elifExpr);

	Token elifColon = tokenizer.getToken();
	if (!elifColon.isColon())
	  die("Parser::ifStatement", "Expected a colon, instead got", elifColon);
	
	Token nL = tokenizer.getToken();
	//Checks comments
	if (nL.isPoundSign())
	  {
	    Token get = tokenizer.getToken();
	    while(!get.eol())
	      get = tokenizer.getToken();
	    tokenizer.ungetToken();
	  }
	else if (!nL.eol())
	  die("Parser::ifStatement", "Expected a New Line Character, instead got", nL);
	
	//<suite> or statements
	Statements *elifStmt = suite();
	elifStmts.push_back(elifStmt);
	
	eCheck = tokenizer.getToken();
      }
    }

    //HANDLES ELSE STATEMENTS FOLLOWING IF/ELIF STATEMENTS
    if (eCheck.getName() == "else"){

      elseStmtsBool = true;
      Token elseColon = tokenizer.getToken();
      if (!elseColon.isColon())
	die("Parser::ifStatement", "Expected a colon, instead got", elseColon);

      Token nL = tokenizer.getToken();
      //Checks comments
      if (nL.isPoundSign())
	{
	  Token get = tokenizer.getToken();
	  while(!get.eol())
	    get = tokenizer.getToken();
	  tokenizer.ungetToken();
	}
      else if (!nL.eol())
	die("Parser::ifStatement", "Expected a New Line Character, instead got", nL);

      //<suite> or statements
      elseStmts = suite();
    }
  }
  else
    tokenizer.ungetToken();
  
  return new IfStatement(ifExpr, ifStmts, elseStmts, elifExprs, elifStmts, elseStmtsBool);
}

AssignmentStatement *Parser::assignStatement() {
    // Parses the following grammar rule
    // 
    // <assign-stmtement> -> <id> = <expr>

    Token varName = tokenizer.getToken();
    if (!varName.isName())
      die("Parser::assignStatement", "Expected a name token, instead got", varName);

    Token assignOp = tokenizer.getToken();
    if (!assignOp.isAssignmentOperator())
        die("Parser::assignStatement", "Expected an equal sign, instead got", assignOp);
    
    ExprNode *rightHandSideExpr = or_Test();
  
    Token nL = tokenizer.getToken();
    //Checks comments
    if (nL.isPoundSign())
      {
	Token get = tokenizer.getToken();
	while(!get.eol())
	  get = tokenizer.getToken();
	tokenizer.ungetToken();
      }
    
    else if (!nL.eol())
     die("Parser::assignStatement", "Expected a New Line Character, instead got", nL);
    
    return new AssignmentStatement(varName.getName(), rightHandSideExpr);
}
 
AssignmentStatement *Parser::forLoopAssignStatement() {
    // Parses the following grammar rule
    //
    // <assign-stmtement> -> <id> = <expr>

    Token varName = tokenizer.getToken();
    if (!varName.isName())
        die("Parser::assignStatement", "Expected a name token, instead got", varName);

    Token assignOp = tokenizer.getToken();
    if (!assignOp.isAssignmentOperator())
        die("Parser::assignStatement", "Expected an equal sign, instead got", assignOp);

    ExprNode *rightHandSideExpr = or_Test();

    return new AssignmentStatement(varName.getName(), rightHandSideExpr);
}

std::vector<ExprNode *> Parser::parseRangeList(){

  std::vector<ExprNode *> rangeList;
  
  ExprNode *firstValue = or_Test();

  Token next = tokenizer.getToken();
  if (!next.isComma()){
    rangeList.push_back(firstValue);
    tokenizer.ungetToken();
    return rangeList;
  }

  ExprNode *secondValue = or_Test();
  next = tokenizer.getToken();
  if (!next.isComma()){
    rangeList.push_back(firstValue);
    rangeList.push_back(secondValue);
    tokenizer.ungetToken();
    return rangeList;
  }

  ExprNode *thirdValue = or_Test();
  rangeList.push_back(firstValue);
  rangeList.push_back(secondValue);
  rangeList.push_back(thirdValue);
  return rangeList;
    
}

ForStatement *Parser::forStatement(){
  //Parses rule
  //<for-statement> -> for <ID> 'in' 'range' '(' <testList> ')' ':' <suite>

  Token varName = tokenizer.getToken();
  if (!varName.isName())
    die("Parser::forStatement", "Expected a name token, instead got", varName);

  Token in = tokenizer.getToken();
  if (!(in.getName() == "in"))
     die("Parser::forStatement", "Expected keyWord 'in', instead got", in);

  Token range = tokenizer.getToken();
  if (!(range.getName() == "range"))
     die("Parser::forStatement", "Expected keyWord 'range', instead got", range);

  Token openParen = tokenizer.getToken();
  if (!openParen.isOpenParen())
     die("Parser::forStatement", "Expected an open parenthesis, instead got", openParen);

  // RangeList *rangeList;
  std::vector<ExprNode *> v = parseRangeList();
  if (v.size() < 1 || v.size() > 3){
    std::cout << "Parser::forStatement RangeList can only accept between 1 and 3 conditions. You input" << v.size() << " conditions." << std::endl;
    exit(1);
  }

  Token closedParen = tokenizer.getToken();
  if (!closedParen.isCloseParen())
     die("Parser::forStatement", "Expected a name token, instead got", closedParen);

  Token colon = tokenizer.getToken();
  if (!closedParen.isCloseParen())
     die("Parser::forStatement", "Expected a name token, instead got", closedParen);
  
  //Checks comments
  Token nL = tokenizer.getToken();
  //Checks comments
  if (nL.isPoundSign())
    {
      Token get = tokenizer.getToken();
      while(!get.eol())
	get = tokenizer.getToken();
      tokenizer.ungetToken();
    }
  else if (!nL.eol())
    die("Parser::ifStatement", "Expected a New Line Character, instead got", nL);
  
  Statements *forStmts = suite();
  
  return new ForStatement(varName.getName(), v, forStmts);

}

PrintStatement *Parser::printStatement(){
  //Variable, String, Operationshkj
  //print "a =", a, "b =", b, "and the sum of the two is", a + b 
  std::vector<ExprNode *> testList;
  
  Token nL = tokenizer.getToken();
  if (nL.eol()){
    return new PrintStatement(testList);
  }
  tokenizer.ungetToken();

  ExprNode *printExpr = or_Test();
  testList.push_back(printExpr);

  Token next = tokenizer.getToken();
  while (next.isComma()){
    printExpr = or_Test();
    testList.push_back(printExpr);
    next = tokenizer.getToken();
  }
  tokenizer.ungetToken();
  
  //Checks comments
  if (next.isPoundSign())
    {
      Token get = tokenizer.getToken();
      while(!get.eol())
        get = tokenizer.getToken();
      tokenizer.ungetToken();
    }

  else if (!next.eol())
    die("Parser::printStatement", "Expected a New Line Character, instead got", next);

  return new PrintStatement(testList);
}

ExprNode *Parser::expr() {
    // This function parses the grammar rules:

    // <expr> -> <term> { <add_op> <term> }
    // <add_op> -> + | -

    // However, it makes the <add_op> left associative.

    ExprNode *left = term();
    Token tok = tokenizer.getToken();
    while (tok.isAdditionOperator() || tok.isSubtractionOperator()) {
        InfixExprNode *p = new InfixExprNode(tok);
        p->left() = left;
        p->right() = term();
        left = p;
        tok = tokenizer.getToken();
    }
    tokenizer.ungetToken();
    return left;
}


ExprNode *Parser::term() {
    // This function parses the grammar rules:

    // <term> -> <primary> { <mult_op> <primary> }
    // <mult_op> -> * | / | %

    // However, the implementation makes the <mult-op> left associate.
    ExprNode *left = primary();
    Token tok = tokenizer.getToken();

    while (tok.isMultiplicationOperator() || tok.isDivisionOperator() || tok.isModuloOperator()) {
        InfixExprNode *p = new InfixExprNode(tok);
        p->left() = left;
        p->right() = primary();
        left = p;
        tok = tokenizer.getToken();

    }
    tokenizer.ungetToken();
    return left;
}

ExprNode *Parser::primary() {
    // This function parses the grammar rules:

    // <primary> -> [0-9]+
    // <primary> -> [_a-zA-Z][_a-zA-Z0-9]*
    // <primary> -> (<expr>)

    Token tok = tokenizer.getToken();

    //INT, DOUBLE, STRING
    if (tok.isWholeNumber() )
        return new Integer(tok);
    else if( tok.isName() )
        return new Variable(tok);
    else if( tok.isStringValue() )
      return new String(tok);
    else if( tok.isDoubleValue() )
      return new Double(tok);
    else if (tok.isOpenParen()) {
        ExprNode *p = or_Test();
        Token token = tokenizer.getToken();
        if (!token.isCloseParen())
            die("Parser::primary", "Expected close-parenthesis, instead got", token);
        return p;
    }

    return nullptr;  // Will not reach this statement!
}

ExprNode *Parser::relationalExpr() {

  ExprNode *left = relationalPrimary();
  Token tok = tokenizer.getToken();  
  while (tok.isEquivalent() || tok.isNotEquivalent() || tok.isGreaterThan() || tok.isGreaterOrEqual() || tok.isLessThan()
	 || tok.isLessOrEqual() || tok.isNotEquivalent2()) {
    InfixExprNode *p = new InfixExprNode(tok);
    p->left() = left;
    p->right() = relationalPrimary();

    left = p;
    tok = tokenizer.getToken();
  }
  tokenizer.ungetToken();
  return left;
}

ExprNode *Parser::relationalPrimary() {  return expr(); }

//Or_test
ExprNode *Parser::or_Test(){
    // This function parses the grammar rules:

    // <or_test> -> <and_test> {'or' <and_test>}*
  
  ExprNode *left = and_Test();
  Token tok = tokenizer.getToken();
  while (tok.isOrOperator()){
    InfixExprNode *p = new InfixExprNode(tok);
    p->left() = left;
    p->right() = and_Test();
    left = p;
    tok = tokenizer.getToken();

  }
  tokenizer.ungetToken();
  return left;
}

//And_test
ExprNode *Parser::and_Test(){
    // This function parses the grammar rules:

    // <and_test> -> <not_test> {'and' <not_test>}*

  ExprNode *left = not_Test();
  Token tok = tokenizer.getToken();
  while (tok.isAndOperator()){
    InfixExprNode *p = new InfixExprNode(tok);
    p->left() = left;
    p->right() = not_Test();
    left = p;
    tok = tokenizer.getToken();
    
  }
  tokenizer.ungetToken();
  return left;
}

//Not_test
ExprNode *Parser::not_Test(){
  // This function parses the grammar rules:

  // <not_test> -> 'not' <not_test> | <relationalExpr>

  ExprNode *left;
  Token tok = tokenizer.getToken();
   
  if (tok.isNotOperator()){
    InfixExprNode *p = new InfixExprNode(tok);
    p->left() = nullptr;
    p->right() = not_Test();
    left = p;
  }
  else{
    tokenizer.ungetToken();
    left = relationalExpr();
  }
  return left;
}

