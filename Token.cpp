#include <string>
#include <stdlib.h>
#include <algorithm>

#include "Token.h"

using namespace std;

vector<string> Token::symbols = {"program","var","begin","end",
                                 "if","then", "else", "while","do","repeat","until",
                                 "integer","boolean",":",":=","+","-","*","div",
                                 "mod","procedure","function","=",
                                 "<>",">","<",">=","<=","not","or",
                                 "and","xor","(",")",".",
                                 ",",";","writeln","readln","true","false"};

static bool isInteger (const string & s)
{
    if (s.empty() || ((!isdigit(s[0])) && (s[0] != '-') && (s[0] != '+')))
        return false;

    char * p;
    strtol(s.c_str(), &p, 10);

    return (*p == 0);
}

bool isAlphaNumString (const std::string &str)
{
    for (int i = 0; i < (int)str.size(); i++)
        if (!isalnum(str[i]))
            return false;
    return true;
}

Token::Token(const string& word, int lin) throw () : token(word), line(lin)
{
    unsigned int i;
    for (i = 0; i < this -> symbols.size(); i++)
        if (this -> token == this -> symbols[i])
            break;
    if (i >= this -> symbols.size()){
        if (isdigit(this->token[0]))
            if (isInteger(this -> token))
                this -> type = TokenType::INTEGER;
            else
                this -> type = TokenType::UNKNOWN;
        else
            if (isAlphaNumString(this -> token))
                this -> type = TokenType::IDENTIFIER;
            else
                this -> type = TokenType::UNKNOWN;
    }
    else
        this -> type = (TokenType)i;
}

string Token::getToken () const throw ()
{
    return string(this -> token);
}

TokenType Token::getType() const throw ()
{
    return this -> type;
}

int Token::getLine() const throw ()
{
    return this -> line;
}
