#ifndef PARSER
#define PARSER

#include <vector>
#include <string>

#include "Lexer.h"
#include "SymbolTable.h"

class Parser
{
private:
    Lexer lex;
    SymbolTable st;

    void throwExpected(TokenType, int, TokenType) throw (string);
    void throwUndeclared(string, int) throw (string);
    void throwIncompatibleType(int) throw (string);
    void throwWtf() throw (string);

    void compileProgramStart () throw (string);
    void compileVariableDeclaration () throw (string);
    void compileProcedureDeclaration () throw (string);
    void compileFunctionDeclaration () throw (string);
    void compileCompoundCommand () throw (string);
    void compileCommand () throw (string);
    void compileAttr () throw (string);
    VariableType compileFuncCall () throw (string);
    VariableType compileTypedSymbol () throw (string);
    void compileIf () throw (string);
    void compileWhile () throw (string);
    void compileWrite () throw (string);
    void compileRead () throw (string);
    void compileRelationalExpression() throw (string);
    void compileArithmeticExpression() throw (string);
public:
    Parser(string) throw ();
    void compile() throw (string);
};

#endif
