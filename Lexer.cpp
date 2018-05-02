#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <stdlib.h>

#include "Token.h"
#include "Lexer.h"

using namespace std;

bool inline isSymbol(char c)
{
    return c == ':' || c == '+' || c == '-'  || c == '*' || c == '=' || c == '>' || c == '<' || c == '(' || c == ')' || c == '.' || c == ',' || c == ';';
}

Lexer::Lexer(string fileName) throw (string)
{
    ifstream file(fileName);
    if (file.fail())
        throw string("Não foi possível abrir o arquivo");

    string word;
    int line = 1;
    Token last(string("&"),0);

    do
    {
        char c = tolower(file.get());

        if (c == '#')
        {
            char s[256];
            file.getline(s,256);
            c = '\n';
        }
        if (isspace(c) && !word.empty())
        {
            Token newToken(word, line);
            last = newToken;
            this -> tokens.push_back(newToken);
            word.clear();
        }
        else if (last.getType() != TokenType::IDENTIFIER && last.getType() != TokenType::NUMBER && !isalnum(c) && !word.empty())
        {
            Token newToken(word, line);
            last = newToken;
            this->tokens.push_back(newToken);
            word.clear();
            if (isSymbol(c))
            {
                if (c == ':')
                {
                    word += c;
                    c = file.get();
                    if (c == '=')
                        word += c;
                    else
                        file.unget();

                    Token newToken(word, line);
                    last = newToken;
                    this -> tokens.push_back(newToken);
                    word.clear();
                }
                else
                {
                    word += c;
                    Token newToken(word, line);
                    last = newToken;
                    this -> tokens.push_back(newToken);
                    word.clear();
                }
            }
        }
        else if ((last.getType() == TokenType::IDENTIFIER || last.getType() == TokenType::NUMBER) && isalnum(c) && !word.empty())
        {
            Token newToken(word, line);
            last = newToken;
            this -> tokens.push_back(newToken);
            word.clear();

            file.unget();
        }
        else if(!isspace(c))
                word += c;

        if (c == '\n')
            line++;
    }while (file.good());

    this->it = -1;
}

Token Lexer::currentToken() const throw ()
{
    Token t = this->tokens[this -> it];
    return t;
}

Token Lexer::nextToken() throw (string)
{
    if (!this->hasMoreTokens())
        throw string("Fim de arquivo inexperado");
    if (this->it < (int)this->tokens.size() - 1)
        ++(this -> it);
    Token t = this->tokens[this -> it];
    return t;
}

Token Lexer::peekToken() const throw (string)
{
    if (!this->hasMoreTokens())
        throw string("Fim de arquivo inexperado");
    Token t = this->tokens[this->it + 1];
    return t;
}

Token Lexer::ungetToken() throw (string) {
    if (this->it <= 0)
        throw string("Não foi possível voltar um pedaço");
    Token t = this->tokens[--(this->it)];
    return t;
}

bool Lexer::hasMoreTokens() const throw ()
{
    return this->it < (int)this->tokens.size() - 1;
}
