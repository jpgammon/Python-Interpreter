James Gammon
rdParser Project 1

To Run:
Simply call make and then ./statements.x <.txt>

Expected output without additional execution statements inputExpressions.txt:

size = 20 * 10 - 81
width = size * 5 - 3 + 2 * 5 + 2
a = 10 * 5 / 15

Symbol table contains the following variables.
a = 3
size = 119
width = 584


Expected output with additional execution statements on inputExpressions.txt:

size = 20 * 10 - 81
width = size * 5 - 3 + 2 * 5 + 2
a = 10 * 5 / 15
WholeNumber::evaluate: returning 20
WholeNumber::evaluate: returning 10
InfixExprNode::evaluate: 20 * 10
WholeNumber::evaluate: returning 81
InfixExprNode::evaluate: 200 - 81
size <- 119
Variable::evaluate: returning SymTab::getValueFor: size contains 119
119
SymTab::getValueFor: size contains 119
WholeNumber::evaluate: returning 5
InfixExprNode::evaluate: 119 * 5
WholeNumber::evaluate: returning 3
WholeNumber::evaluate: returning 2
WholeNumber::evaluate: returning 5
InfixExprNode::evaluate: 2 * 5
InfixExprNode::evaluate: 3 + 10
InfixExprNode::evaluate: 595 - 13
WholeNumber::evaluate: returning 2
InfixExprNode::evaluate: 582 + 2
width <- 584
WholeNumber::evaluate: returning 10
WholeNumber::evaluate: returning 5
InfixExprNode::evaluate: 10 * 5
WholeNumber::evaluate: returning 15
InfixExprNode::evaluate: 50 / 15
a <- 3

Symbol table contains the following variables.
a = 3
size = 119
width = 584
