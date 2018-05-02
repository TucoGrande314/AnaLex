#ifndef ANALEX
#define ANALEX

#include <string>
#include <vector>
#include <fstream>
#include <iostream>

#include "Token.h"

using namespace std;

class Lexer
{
private:
    vector<Token> tokens;
    int it;

public:
    Lexer(string) throw (string);
    Token currentToken() const throw ();
    Token nextToken() throw (string);
    Token peekToken() const throw (string);
    Token ungetToken() throw (string);
    bool hasMoreTokens() const throw ();
};

#endif // ANALEX
