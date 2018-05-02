#include "ExpressionSolver.h"

vector<vector<TokenType>> ExpressionSolver::precedences = {
    { TokenType::MULTIPLICATION, TokenType::DIVISION },
    { TokenType::SUM, TokenType::SUBTRACTION},
    { TokenType::SMALLER, TokenType::SMALLER_EQUAL, TokenType::EQUALS, TokenType::DIFFERENT, TokenType::BIGGER_EQUAL, TokenType::BIGGER},
    { TokenType::AND },
    { TokenType::OR },
    { TokenType::LEFT_PARENTHESIS},
    { TokenType::RIGHT_PARENTHESIS}
};

ExpressionSolver::ExpressionSolver(const vector<TokenType>& exp) throw (string) : exp(exp) {
    for (auto it = this->exp.cbegin(); it != this->exp.cend(); it++)
        if (isError(*it))
            throw string("Expected identifier or operator na linha "+ line);
}

bool ExpressionSolver::isOperand(const Token& t) throw () {
    return t.getType == TokenType::IDENTIFIER ||
           t.getType == TokenType::NUMBER ||
           t.getType == TokenType::TRUE ||
           t.getType == TokenType::FALSE;
}

vector<Token> ExpressionSolver::convertToPosfixe(vector<Token> expression) throw(){
    std::stack<Token> operators;
    std::queue<Token> output;
    for(int i =0; i<expression.length; i++){
        Token token = expression::at(i);
        if(isOperand(token))
            output.emplace(token);
        else if(isOperator(token))
                if(operators.empty())
                    operators.swap(token);
                else
                {
                    Token topoDaPilha =(Token)operators::top();
                    while((getPrecedence(topoDaPilha, token)<0)&&(topoDaPilha.getType!=TokenType::LEFT_PARENTHESIS))
                    {
                        output.emplace(operators::top());
                        operators.pop();
                    }
                    if(token.getType==TokenType::LEFT_PARENTHESIS)
                        operators.swap(token);
                    if(token.getType==TokenType::RIGHT_PARENTHESIS)
                    {
                        while(operators.top()!=TokenType::LEFT_PARENTHESIS||!(operators::empty())
                        {
                            output.emplace(operators.top());
                            operators.pop();
                        }
                        if((Token)(operators.top()).getType==TokenType::LEFT_PARENTHESIS)
                            operators.pop();
                        else
                            throw (string) ("Fecha de parenteses inesperada na linha "+ line);
                    }

                }

    }

    while(!operators::empty()){
        if((Token)(operators.top()).getType==TokenType::LEFT_PARENTHESIS)
            throw (string) ("Abertura de parenteses inesperada na linha "+ line);
        if((Token)(operators.top()).getType==TokenType::RIGHT_PARENTHESIS)
            throw (string) ("Fecha de parenteses inesperada na linha "+ line);
        output.emplace(operators.top());
        operators.pop();
    }
    std::vector<Token> ret;
    while(!output.empty())
    {
        ret.push_back(output.front());
        output.pop();
    }
}


bool ExpressionSolver::isOperator(const Token& t) throw () {
    return t.getType() == TokenType::MULTIPLICATION || t.getType() == TokenType::DIVISION ||
           t.getType() == TokenType::SUM || t.getType() == TokenType::SUBTRACTION ||
           t.getType() == TokenType::SMALLER || t.getType() == TokenType::SMALLER_EQUAL ||
           t.getType() == TokenType::EQUALS || t.getType() == TokenType::DIFFERENT ||
           t.getType() == TokenType::BIGGER_EQUAL || t.getType() == TokenType::BIGGER ||
           t.getType() == TokenType::AND ||
           t.getType() == TokenType::OR;
}
ExpressionSolver::ExpressionSolver(SymbolTable* table, int line) throw (){
    ExpressionSolver->table = table;
    ExpressionSolver->line = line;
}

bool ExpressionSolver::isParenthesis(const Token& t) throw () {
    return t.getType() == TokenType::LEFT_PARENTHESIS || t == TokenType::RIGHT_PARENTHESIS;
}

bool ExpressionSolver::isError(const Token& t) throw () {
    return !isOperand(t) && !isOperator(t) && !isParenthesis(t);
}


char ExpressionSolver::getPrecedence(const Token& a, const Token& b) throw () {
    char ap = -1,
         bp = -1;
    bool stop = false;
    for (auto i = precedences.cbegin(); !stop && i != precedences.cend(); i++)
        for (auto j = i->cbegin(); !stop && j != i->cend(); j++)
            if (*j == a.getType()) {
                ap = i - precedences.cbegin();
                stop = true;
            }

    stop = false;
    for (auto i = precedences.cbegin(); !stop && i != precedences.cend(); i++)
        for (auto j = i->cbegin(); !stop && j != i->cend(); j++)
            if (*j == b.getType()) {
                bp = i - precedences.cbegin();
                stop = true;
            }

    return bp - ap;
}
bool ExpressionSolver::isArithimetic(const TokenType& t)
{
    return t == TokenType::MULTIPLICATION || t == TokenType::DIVISION ||
           t == TokenType::SUM || t == TokenType::SUBTRACTION;
}

VariableType ExpressionSolver::evaluate(const vector<TokenType>& expression) throw () {
    for (auto it = this->expression.cbegin(); it != this->expression.cend(); it++)
        if (isError(*it))
            throw string("Operador ou operando esperado na linha "+ line);

    std::vector<TokenType> posfixe = convertToPosfixe(expression);
    std::stack<VariableType> operands;
    for(int i=0;i<posfixe.lenght; i++){
        TokenType token = posfixe.[i];
        if(isOperand(token)
        {
           //operands.swap(token);
           if(token==TokenType.NUMBER)
                operands.swap(VariableType.INTEGER);
           else
            if(token==TokenType.TRUE||if(token==TokenType.FALSE))
                operands.swap(VariableType.BOOLEAN);
            else
            {
                VariableType varType = (table.getSymbol(token.getToken())).getReturnedType();
                if(varType==VariableType.VOID)
                    throw (string) ("Variável ou função esperada, procedimento recebido na linha "+ line);
                else
                    operands.swap(varType);
            }
        }
        else
        if(isOperator(token))
        {
            if(operands.empty())
                throw (string) ("Operador inexperado na linha "+ line);
            VariableType op1 = operands.top();
            operands.pop();
            if(operands.empty())
                throw (string) ("Operador inexperado na linha "+ line);
            VariableType op2 = operands.top();
            operands.pop();

            if(op1.getType()!=op2.getType())
                throw (string) ("Operadores devem ser de mesmo tipo em uma expressão na linha"+ line);

            VariableType varTp = op1.getType();
            TokenType tonkenTp = token.getType();

            if(varTp==VariableType::BOOLEAN)
            {
                if(tokenTp==TokenType.SUM||tokenTp==TokenType.SUBTRACTION||tokenTp==TokenType.MULTIPLICATION||tokenTp==TokenType.DIVISION)
                    throw (string) ("Operadores aritméticos esperam por operandos do tipo inteiro na linha "+ line);

                if(tokenTp==TokenType.BIGGER||tokenTp==TokenType.BIGGER_EQUAL||tokenTp==TokenType.SMALLER||tokenTp==TokenType.SMALLER_EQUAL)
                    throw (string) ("Operadores relacionais esperam por operandos do tipo inteiro na linha "+ line);


                if(tonkenTp==TokenType.AND||tonkenTp==TokenType.OR||tonkenTp==TokenType.XOR||tokenTp==TokenType.EQUALS)
                    operands.swap(VariableType.BOOLEAN);
            }
            else
            if(varTp==VariableType.INTEGER)
            {
                if(tokenTp==TokenType.SUM||tokenTp==TokenType.SUBTRACTION||tokenTp==TokenType.MULTIPLICATION||tokenTp==TokenType.DIVISION||tokenTp==TokenType.EQUALS)
                    operands.swap(VariableType.INTEGER);

                if(tokenTp==TokenType.BIGGER||tokenTp==TokenType.BIGGER_EQUAL||tokenTp==TokenType.SMALLER||tokenTp==TokenType.SMALLER_EQUAL)
                    operands.swap(VariableType.BOOLEAN);

                if(tonkenTp==TokenType.AND||tonkenTp==TokenType.OR||tonkenTp==TokenType.XOR)
                    throw (string) ("Operadores lógicos esperam por operandos do tipo booleanos na linha "+ line);
            }
        }
        else
            throw (string) ("Expressão inválida, operador ou operando esperado na linha "+ line);


    }
    if(operands.empty())
        throw (string) ("Expressão inválida na linha "+ line);
    VariableType ret = operands.top();
    operands.pop();
    if(!operands.empty())
        throw (string) ("Há mais operandos do que operadores na linha "+ line);
    return ret;
}
