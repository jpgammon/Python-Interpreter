#include <iostream>
#include<vector>

#include "Token.hpp"
#include "Tokenizer.hpp"
#include "Parser.hpp"

using namespace std;

int main(int argc, char *argv[]) {

  int count = 0;
  
  if (argc != 2) {
        cout << "usage: " << argv[0] << " nameOfAnInputFile\n";
        exit(1);
    }

    ifstream inputStream;

    inputStream.open(argv[1], ios::in);
    if (!inputStream.is_open()) {
        cout << "Unable to open " << argv[1] << ". Terminating...";
        perror("Error when attempting to open the input file.");
	cout << endl;
        exit(2);
    }

    Tokenizer tokenizer(inputStream);
    Parser parser(tokenizer);
    Statements *statements = parser.statements();
    Token lastToken = tokenizer.getToken();
    
    if (!lastToken.eof()) {
        cout << "Unexpected token in input." << endl;
        lastToken.print();
        exit(1);
    }

    SymTab symTab;

    statements->print();
    statements->evaluate(symTab);
    cout << endl << "Symbol table contains the following variables.\n";
    symTab.print();
    
    cout << endl;

    return 0;
}
