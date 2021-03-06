#include<vector>
#include<iostream>

#include "Token.hpp"
#include "Parser.hpp"
#include "Statements.hpp"


// Parser functions

void Parser::die(std::string where, std::string message, Token &token) {
    std::cout << where << " " << message << std::endl;
    token.print();
    std::cout << std::endl;
    std::cout << "\nThe following is a list of tokens that have been identified up to this point.\n";
    tokenizer.printProcessedTokens();
    exit(1);
}

Statements *Parser::statements() {
    // This function is called when we KNOW that we are about to parse
    // a series of assignment statements.

    // This function parses the grammar rules:

    // <statement> -> <assignStatement> <statement> 
    // <statement> -> Epsilon ------ empty program

    Statements *stmts = new Statements();
    Token tok = tokenizer.getToken();

    while (tok.eol()){
      tok = tokenizer.getToken();
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
      }

      else {
      tokenizer.ungetToken();
      AssignmentStatement *assignStmt = assignStatement();
      stmts->addStatement(assignStmt);
      tok = tokenizer.getToken();
      }
      if (!tok.eol())
	;
      while (tok.eol()){
	tok = tokenizer.getToken();
      }
    }
    tokenizer.ungetToken();
    return stmts;
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
    
    ExprNode *rightHandSideExpr = relationalExpr();
    
    return new AssignmentStatement(varName.getName(), rightHandSideExpr);
}

ForStatement *Parser::forStatement(){
  //Parses rule
  //<for-statement> -> for ( <assign-statement> ; <rel-expr> ; <assign-statement> ) { <statements> }

  //Checks first token == '('
  Token openP = tokenizer.getToken();
  if (!openP.isOpenParen())
    die("Parser::forStatement", "Expected an Open Parentheses, instead got", openP);

  //Checks first statement == assign-statement
  AssignmentStatement *firstA = assignStatement();

  //Checks second token == ';'
  Token semiColon1 = tokenizer.getToken();
  if (!semiColon1.isSemiColon())
    die("Parser::forStatement", "Expected a SemiColon, instead got", semiColon1);

  //Checks second statement == rel-expr
  ExprNode *forRelationalExpr = relationalExpr();
  
  //Checks third token == ';'
  Token semiColon2 = tokenizer.getToken();
  if (!semiColon2.isSemiColon())
    die("Parser::forStatement", "Expected a SemiColon, instead got", semiColon2);

  //Checks third statement == assign-statement
  AssignmentStatement *secondA = assignStatement();
  
  //Checks fourth token == ')'
  Token closedP = tokenizer.getToken();
  if (!closedP.isCloseParen())
    die("Parser::forStatement", "Expected an Closed Parentheses, instead got", closedP);

  //Checks fifth token == '{'
  Token openCurly = tokenizer.getToken();
  if (!openCurly.isOpenCurly())
    die("Parser::forStatement", "Expected an Open Curly Brace, instead got", openCurly);

  //Enforces syntax of for loop
  Token nL1 = tokenizer.getToken();
  if (!nL1.eol())
    die("Parser::forStatement", "Expected a New Line Character, instead got", nL1);
  
  //Checks fourth statement == vector of statements
  Statements *forStmts = statements();
  
  //Checks fifth token == '}' 
  Token closedCurly = tokenizer.getToken();
  if (!closedCurly.isCloseCurly())
    die("Parser::forStatement", "Expected a Closed Curly Brace, instead got", closedCurly);

  //Enforces syntax of for loop
 Token nL2 = tokenizer.getToken();
  if (!nL2.eol())
    die("Parser::forStatement", "Expected a New Line Character, instead got", nL2);
  
  return new ForStatement(firstA, forRelationalExpr, secondA, forStmts);
  
}

PrintStatement *Parser::printStatement(){
  Token varName = tokenizer.getToken();
  if (!varName.isName())
    die("Parser::printStatement", "Expected a name token, instead got", varName);
  
  return new PrintStatement(varName.getName());
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

    if (tok.isWholeNumber() )
        return new WholeNumber(tok);
    else if( tok.isName() )
        return new Variable(tok);
    else if (tok.isOpenParen()) {
        ExprNode *p = expr();
        Token token = tokenizer.getToken();
        if (!token.isCloseParen())
            die("Parser::primary", "Expected close-parenthesis, instead got", token);
        return p;
    }
    die("Parser::primary", "Unexpected token", tok);

    return nullptr;  // Will not reach this statement!
}

ExprNode *Parser::relationalExpr() {

    ExprNode *left = relationalTerm();
    Token tok = tokenizer.getToken();
    while (tok.isEquivalent() || tok.isNotEquivalent()) {
        InfixExprNode *p = new InfixExprNode(tok);
        p->left() = left;
        p->right() = relationalTerm();
        left = p;
        tok = tokenizer.getToken();
    }
    tokenizer.ungetToken();
    return left;
}

ExprNode *Parser::relationalTerm() {

  ExprNode *left = relationalPrimary();
    Token tok = tokenizer.getToken();

    while (tok.isGreaterThan() || tok.isGreaterOrEqual() || tok.isLessThan() || tok.isLessOrEqual()) {
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
