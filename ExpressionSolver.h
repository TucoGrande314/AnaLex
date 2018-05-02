#ifndef EXPRESSION_SOLVER_INCLUDED
#define EXPRESSION_SOLVER_INCLUDED

#include <stack>
#include <vector>

#include "Token.h"
#include "SymbolTable.h"

class ExpressionSolver {
    private:
        static vector<vector<TokenType>> precedences;
        vector<TokenType> exp;
        SymbolTable* table;
        int line;
        static bool isOperand(const Token&) throw ();
        static bool isOperator(const Token&) throw ();
        static bool isParenthesis(const Token&) throw ();
        static bool isError(const Token&) throw ();
        static string convertToPosfixe(std::string) throw();
        static char getPrecedence(const Token&, const Token&) throw ();
        static bool isArithimetic(const Token&) throw ();
    public:
        ExpressionSolver(SymbolTable* table, int line);
        vector<TokenType> evaluate(const vector<TokenType>&) throw ();
};

#endif // EXPRESSION_SOLVER_INCLUDED
