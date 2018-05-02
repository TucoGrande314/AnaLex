#include <sstream>
#include <forward_list>

#include "Parser.h"

const string TokenTypeNames[] = {"PROGRAM","VAR","BEGIN","END","IF","THEN","ELSE","WHILE","DO","REPEAT","UNTIL","INTEGER","BOOLEAN","COLON",
                                 "ATTRIBUTION","SUM",
                                 "SUBTRACTION","MULTIPLICATION","DIVISION","MODULE","PROCEDURE","FUNCTION","=","!=",">","<",
                                 ">=","<=","NOT","OR","AND","XOR","(",")",".",",",";","WRITE","READ","TRUE","FALSE",
                                 "IDENTIFIER","CONSTANT","UNKNOWN"};

Parser::Parser(string path) throw () : lex(path), st() {}


template <typename T>
std::string to_string(T value)
{
	std::ostringstream os ;
	os << value ;
	return os.str() ;
}

void Parser::throwExpected(TokenType expected, int line, TokenType found) throw (string) {
    throw string (" Era esperado \"" + TokenTypeNames[(int)expected] +
                  "\" na linha " +
                  to_string(line) +
                  " mas \"" +
                  TokenTypeNames[(int)found] +
                  "\" foi encontrado");
}

void Parser::throwUndeclared(string name, int line) throw (string) {
    throw string("Indentificador não declarado "+name+" linha:" +/*std::*/to_string(line));
}

void Parser::throwIncompatibleType(int line) throw (string) {
    throw string("Não foi possível realizar a conversão de tipo na linha?"+/*std::*/to_string(line));
}

void Parser::compile() throw (string) {
    compileProgramStart();

    for (Token t(this->lex.peekToken()); this->lex.hasMoreTokens(); t = this->lex.peekToken())
    {

        if (t.getType() == TokenType::VARIABLE)
            this->compileVariableDeclaration();
        if (t.getType() == TokenType::PROCEDURE)
            this->compileProcedureDeclaration();
        if (t.getType() == TokenType::FUNCTION)
            this->compileFunctionDeclaration();

        if (t.getType() == TokenType::BEGIN) {
            this->compileCompoundCommand();
            t = this->lex.nextToken();

            if (t.getType() != TokenType::PERIOD)
                throwExpected(TokenType::PERIOD, t.getLine(), t.getType());

            break;
        }
    }
}

void Parser::compileProgramStart () throw (string) {
    Token next(this->lex.nextToken());
    if (next.getType() != TokenType::PROGRAM)
        this->throwExpected(TokenType::PROGRAM, next.getLine(), next.getType());

    next = this->lex.nextToken();
    if (next.getType() != TokenType::IDENTIFIER)
        this->throwExpected(TokenType::IDENTIFIER, next.getLine(), next.getType());

    next = this->lex.nextToken();
    if (next.getType() != TokenType::SEMICOLON)
        this->throwExpected(TokenType::SEMICOLON, next.getLine(), next.getType());
}

void Parser::compileVariableDeclaration () throw (string) {
    Token next(this->lex.nextToken());
    if (next.getType() != TokenType::VARIABLE)
        this->throwExpected(TokenType::VARIABLE, next.getLine(), next.getType());

    next = this->lex.nextToken();
    if (next.getType() != TokenType::IDENTIFIER)
        this->throwExpected(TokenType::IDENTIFIER, next.getLine(), next.getType());

    while (true)
    {
        bool sameType = true;
        forward_list<Token> pars;
        while (sameType)
        {
            pars.push_front(next);
            next = this->lex.nextToken();
            if (next.getType() == TokenType::COLON)
                sameType = false;
            else if (next.getType() == TokenType::COMMA)
                next = this->lex.nextToken();
            else
                this->throwExpected(TokenType::COMMA, next.getLine(), next.getType());
        }
        next = this->lex.nextToken();
        if (next.getType() ==TokenType::INTEGER)
        {
            for (auto it = pars.cbegin(); it != pars.cend(); it++)
            {
                const Symbol p (it->getToken(), VariableType::INTEGER);
                this->st.insertSymbol(p);
            }
            pars.clear();
        }
        else if  (next.getType() == TokenType::BOOLEAN)
        {
            for (auto it = pars.cbegin(); it != pars.cend(); it++)
            {
                const Symbol p(it->getToken(), VariableType::BOOLEAN);
                this->st.insertSymbol(p);
            }
            pars.clear();
        }
        else
            this->throwExpected(TokenType::VARIABLE, next.getLine(), next.getType());
        next = this->lex.nextToken();
        if (next.getType() == TokenType::SEMICOLON)
        {
            if (this->lex.currentToken().getType() != TokenType::IDENTIFIER)
                break;
            next = this->lex.nextToken();
        }
        else
            this->throwExpected(TokenType::SEMICOLON, next.getLine(), next.getType());
    }
}

void Parser::compileProcedureDeclaration () throw (string) {
    Token next(this->lex.nextToken());
    if (next.getType() != TokenType::PROCEDURE)
        this->throwExpected(TokenType::PROCEDURE, next.getLine(), next.getType());

    next = this->lex.nextToken();
    if (next.getType() != TokenType::IDENTIFIER)
        this->throwExpected(TokenType::IDENTIFIER, next.getLine(), next.getType());
    string name = next.getToken();

    next = this->lex.nextToken();
    if (next.getType() != TokenType::LEFT_PARENTHESIS)
        this->throwExpected(TokenType::LEFT_PARENTHESIS, next.getLine(), next.getType());

    next = this->lex.nextToken();
    vector<Symbol> params;
    if (next.getType() != TokenType::RIGHT_PARENTHESIS)
    {
        if (next.getType() == TokenType::VARIABLE || next.getType() == TokenType::IDENTIFIER)
        {
            while (true)
            {
                bool sameType = true;
                forward_list<Token> pars;
                while (sameType)
                {
                    pars.push_front(next);
                    next = this->lex.nextToken();
                    if (next.getType() == TokenType::COLON)
                        sameType = false;
                    else if (next.getType() == TokenType::COMMA)
                        next = this->lex.nextToken();
                    else
                        this->throwExpected(TokenType::COMMA, next.getLine(), next.getType());
                }
                next = this->lex.nextToken();
                if (next.getType() ==TokenType::INTEGER)
                {
                    for (auto it = pars.cbegin(); it != pars.cend(); it++)
                    {
                        const Symbol p (it->getToken(), VariableType::INTEGER);
                        params.push_back(p);
                    }
                    pars.clear();
                }
                else if  (next.getType() == TokenType::BOOLEAN)
                {
                    for (auto it = pars.cbegin(); it != pars.cend(); it++)
                    {
                        const Symbol p(it->getToken(), VariableType::BOOLEAN);
                        params.push_back(p);
                    }
                    pars.clear();
                }
                else
                    this->throwExpected(TokenType::VARIABLE, next.getLine(), next.getType());
                next = this->lex.nextToken();
                if (next.getType() == TokenType::SEMICOLON || next.getType() == TokenType::RIGHT_PARENTHESIS)
                {
                    if (this->lex.peekToken().getType() != TokenType::IDENTIFIER)
                        break;
                    next = this->lex.nextToken();
                }
                else
                    this->throwExpected(TokenType::RIGHT_PARENTHESIS, next.getLine(), next.getType());
            }
        }
        else
            this->throwExpected(TokenType::RIGHT_PARENTHESIS, next.getLine(), next.getType());
    }

    next = this->lex.nextToken();
    if (next.getType() != TokenType::SEMICOLON)
        this->throwExpected(TokenType::SEMICOLON, next.getLine(), next.getType());

    this->st.insertSymbol(Symbol(name, VariableType::VOID, &params));
    for (auto it = params.cbegin(); it != params.cend(); it++)
        this->st.insertSymbol(*it);

    if (this->lex.peekToken().getType() == TokenType::VARIABLE)
        this->compileVariableDeclaration();

    next = this->lex.peekToken();
    bool end = false;
	while (!end)
	{
		switch (next.getType())
		{
        case TokenType::PROCEDURE:
            this->compileProcedureDeclaration();
            break;
        case TokenType::FUNCTION:
            this->compileFunctionDeclaration();
            break;
        case TokenType::BEGIN :
            end = true;
            break;
        default:
            this->throwExpected(TokenType::BEGIN, next.getLine(), next.getType());
		}
		next = this->lex.peekToken();
	}
	this->compileCompoundCommand();
	next = this->lex.nextToken();
	if (next.getType() != TokenType::SEMICOLON)
        this->throwExpected(TokenType::SEMICOLON, next.getLine(), next.getType());
    this->st.clearCurrentScope();
}

void Parser::compileFunctionDeclaration () throw (string) {
    Token next(this->lex.nextToken());
    if (next.getType() != TokenType::FUNCTION)
        this->throwExpected(TokenType::FUNCTION, next.getLine(), next.getType());

    next = this->lex.nextToken();
    if (next.getType() != TokenType::IDENTIFIER)
        this->throwExpected(TokenType::IDENTIFIER, next.getLine(), next.getType());
    string name = next.getToken();

    next = this->lex.nextToken();
    if (next.getType() != TokenType::LEFT_PARENTHESIS)
        this->throwExpected(TokenType::LEFT_PARENTHESIS, next.getLine(), next.getType());

    next = this->lex.nextToken();
    vector<Symbol> params;
    if (next.getType() != TokenType::RIGHT_PARENTHESIS)
    {
        if (next.getType() == TokenType::VARIABLE || next.getType() == TokenType::IDENTIFIER)
        {
            while (true)
            {
                if(next.getType()!=TokenType::VARIABLE)
                    this->throwExpected(TokenType::VARIABLE, next.getLine(), next.getType());
                next = lex.nextToken();
                if(next.getType()!=TokenType::IDENTIFIER)
                    this->throwExpected(TokenType::IDENTIFIER, next.getLine(), next.getType());
                bool sameType = true;
                forward_list<Token> pars;
                while (sameType)
                {
                    pars.push_front(next);
                    next = this->lex.nextToken();
                    if (next.getType() == TokenType::COLON)
                        sameType = false;
                    else if (next.getType() == TokenType::COMMA)
                        next = this->lex.nextToken();
                    else
                        this->throwExpected(TokenType::COMMA, next.getLine(), next.getType());
                }
                next = this->lex.nextToken();
                if (next.getType() ==TokenType::INTEGER)
                {
                    for (auto it = pars.cbegin(); it != pars.cend(); it++)
                    {
                        const Symbol p (it->getToken(), VariableType::INTEGER);
                        params.push_back(p);
                    }
                    pars.clear();
                }
                else if  (next.getType() == TokenType::BOOLEAN)
                {
                    for (auto it = pars.cbegin(); it != pars.cend(); it++)
                    {
                        const Symbol p(it->getToken(), VariableType::BOOLEAN);
                        params.push_back(p);
                    }
                    pars.clear();
                }
                else
                    this->throwExpected(TokenType::VARIABLE, next.getLine(), next.getType());
                next = this->lex.nextToken();
                if (next.getType() == TokenType::SEMICOLON || next.getType() == TokenType::RIGHT_PARENTHESIS)
                {
                    if (this->lex.peekToken().getType() != TokenType::IDENTIFIER)
                        break;
                    next = this->lex.nextToken();
                }
                else
                    this->throwExpected(TokenType::RIGHT_PARENTHESIS, next.getLine(), next.getType());
            }
        }
        else
            this->throwExpected(TokenType::RIGHT_PARENTHESIS, next.getLine(), next.getType());
    }

    next = this->lex.nextToken();
    if (next.getType() != TokenType::COLON)
        this->throwExpected(TokenType::COLON, next.getLine(), next.getType());

    VariableType type;
    next = this->lex.nextToken();
    if (next.getType() == TokenType::INTEGER)
        type = VariableType::INTEGER;
    else if(next.getType() == TokenType::BOOLEAN)
        type = VariableType::BOOLEAN;
    else
        this->throwExpected(TokenType::VARIABLE, next.getLine(), next.getType());


    next = this->lex.nextToken();
    if (next.getType() != TokenType::SEMICOLON)
        this->throwExpected(TokenType::SEMICOLON, next.getLine(), next.getType());

    this->st.insertSymbol(Symbol(name, type, &params));
    for (auto it = params.cbegin(); it != params.cend(); it++)
        this->st.insertSymbol(*it);

    if (this->lex.peekToken().getType() == TokenType::VARIABLE)
        this->compileVariableDeclaration();

    next = this->lex.peekToken();
    bool end = false;
	while (!end)
	{
		switch (next.getType())
		{
        case TokenType::PROCEDURE:
            this->compileProcedureDeclaration();
            break;
        case TokenType::FUNCTION:
            this->compileFunctionDeclaration();
            break;
        case TokenType::BEGIN :
            end = true;
            break;
        default:
            this->throwExpected(TokenType::BEGIN, next.getLine(), next.getType());
		}
		next = this->lex.peekToken();
	}
	this->compileCompoundCommand();
	next = this->lex.nextToken();
	if (next.getType() != TokenType::SEMICOLON)
        this->throwExpected(TokenType::SEMICOLON, next.getLine(), next.getType());
    this->st.clearCurrentScope();

}

void Parser::compileCompoundCommand () throw (string) {
    Token next(this->lex.nextToken());
    if (next.getType() != TokenType::BEGIN)
        this->throwExpected(TokenType::BEGIN, next.getLine(), next.getType());

    while (this->lex.peekToken().getType() != TokenType::END)
        this->compileCommand();
    next = this->lex.nextToken();
    if (next.getType() != TokenType::END)
        this->throwExpected(TokenType::END, next.getLine(), next.getType());
}

void Parser::compileCommand() throw (string) {
    Token next = this->lex.peekToken();
    switch (next.getType())
    {
    case TokenType::IDENTIFIER:
    {
        Symbol *s = this->st.getSymbol(next.getToken());
        if (s == nullptr)
            throwUndeclared(next.getToken(), next.getLine());
        switch(s->getType())
        {
        case SymbolType::VARIABLE:
            this->compileAttr();
            break;
        case SymbolType::FUNCTION:
            this->compileFuncCall();
            break;
        }
        break;
    }
    case TokenType::IF:
    {
        this->compileIf();
        break;
    }
    case TokenType::WHILE:
    {
        this->compileWhile();
        break;
    }
    case TokenType::REPEAT:
    {
        this->compileWhile();
        break;
    }
    case TokenType::WRITE:
    {
        //this->compileWrite();
        break;
    }
    case TokenType::READ:
    {
        //this->compileRead();
        break;
    }
    default:
        this->throwExpected(TokenType::VARIABLE, next.getLine(), next.getType());
        break;
    }


    next = this->lex.nextToken();
    if (next.getType() != TokenType::SEMICOLON)
        throwExpected(TokenType::SEMICOLON, next.getLine(), next.getType());
}

VariableType Parser::compileFuncCall () throw (string) {
    Token next(this->lex.nextToken());
    Symbol *func = this->st.getSymbol(next.getToken());
    if (func == nullptr)
        throwUndeclared(next.getToken(), next.getLine());


    next = this->lex.nextToken();
    if (next.getType() == TokenType::LEFT_PARENTHESIS)
    {
        vector<Symbol> params = *func->getParams();

        // TODO: Refazer isso aqui
        for (auto i = params.cbegin(); i != params.cend(); i++) {
            if (i->getReturnType() != this->compileTypedSymbol())
                throwIncompatibleType(next.getLine());

            next = this->lex.nextToken();
            if (next.getType() != TokenType::COMMA)
                throwExpected(TokenType::COMMA, next.getLine(), next.getType());
        }

        next = this->lex.nextToken();
        if (next.getType() != TokenType::RIGHT_PARENTHESIS)
            throwExpected(TokenType::RIGHT_PARENTHESIS, next.getLine(), next.getType());
    }
    else if (func->getParamCount() > 0)
        throw string("O método" + func->getName() + " não aceita tais parâmetros na linha" + to_string(next.getLine()));

    return func->getReturnType();
}

void Parser::compileAttr() throw (string) {
    Token next(this->lex.nextToken());

    Symbol* s = this->st.getSymbol(next.getToken());
    if (s == nullptr)
        throwUndeclared(s->getName(), next.getLine());


    VariableType type = s->getReturnType();

    next = this->lex.nextToken();
    if (next.getType() != TokenType::ATTRIBUTION)
        throwExpected(TokenType::ATTRIBUTION, next.getLine(), next.getType());

    if (type != this->compileTypedSymbol())
        throwIncompatibleType(next.getLine());
}

VariableType Parser::compileTypedSymbol() throw (string) {
    Token next(this->lex.nextToken());
    if (next.getType() == TokenType::TRUE || next.getType() == TokenType::FALSE)
        return VariableType::BOOLEAN;
    if (next.getType() == TokenType::INTEGER)
        return VariableType::INTEGER;

    if (next.getType() != TokenType::IDENTIFIER)
        throwExpected(TokenType::IDENTIFIER, next.getLine(), next.getType());

    Symbol* s = this->st.getSymbol(next.getToken());
    if (s == nullptr)
        throwUndeclared(next.getToken(), next.getLine());

    if (s->getType() == SymbolType::FUNCTION) {
        this->lex.ungetToken();
        this->compileFuncCall();
    }

    return s->getReturnType();
}

void Parser::compileIf() throw (string) {
    Token next(this->lex.nextToken());

    // this->compileRelationalExpression();

    next = this->lex.nextToken();
    if (next.getType() != TokenType::THEN)
        throwExpected(TokenType::THEN, next.getLine(), next.getType());

    this->compileCommand();

    next = this->lex.peekToken();
    if (next.getType() == TokenType::ELSE) {
        this->lex.nextToken();
        this->compileCommand();
    }
}

void Parser::compileWhile() throw (string) {
    Token next(this->lex.nextToken());
    if (next.getType() == TokenType::WHILE) {
        //this->compileRelationalExpression();

        next = this->lex.nextToken();
        if (next.getType() != TokenType::DO)
            throwExpected(TokenType::DO, next.getLine(), next.getType());

        this->compileCommand();
    }
    else if (next.getType() == TokenType::REPEAT) {
        this->compileCommand();

        next = this->lex.nextToken();
        if (next.getType() != TokenType::UNTIL)
            throwExpected(TokenType::UNTIL, next.getLine(), next.getType());
        //this->compileRelationalExpression();

        next = this->lex.nextToken();
        if (next.getType() != TokenType::SEMICOLON)
            throwExpected(TokenType::SEMICOLON, next.getLine(), next.getType());
    }
}

void Parser::compileRead() throw (string)
{
    Token next(this->lex.nextToken());
    if (next.getType() != TokenType::READ)
        this->throwExpected(TokenType::READ, next.getLine(), next.getType());

    next = this->lex.nextToken();
    if (next.getType() != TokenType::LEFT_PARENTHESIS)
        this->throwExpected(TokenType::LEFT_PARENTHESIS, next.getLine(), next.getType());

    next = this->lex.nextToken();
    if (next.getType() != TokenType::IDENTIFIER)
        this->throwExpected(TokenType::IDENTIFIER, next.getLine(), next.getType());

    Symbol *s = this->st.getSymbol(next.getToken());
    if (s == nullptr)
        this->throwUndeclared(next.getToken(), next.getLine());
    if (s->getType() != SymbolType::VARIABLE || s->getReturnType() != VariableType::INTEGER)
        this->throwExpected(TokenType::INTEGER, next.getLine(),next.getType());

    next = this->lex.nextToken();
    if (next.getType() != TokenType::RIGHT_PARENTHESIS)
        this->throwExpected(TokenType::RIGHT_PARENTHESIS, next.getLine(), next.getType());

    next = this->lex.nextToken();
    if (next.getType() != TokenType::SEMICOLON)
        this->throwExpected(TokenType::SEMICOLON, next.getLine(), next.getType());
}


/*void Parser::compileExpression() throw (string)
{
    VariableType expressionType = VariableType::VOID;
    bool hasParenthesis = false;

    Token next = this->lex.nextToken();
    if (next.getType() == TokenType::LEFT_PARENTHESIS)
} */
