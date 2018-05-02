#ifndef TOKEN
#define TOKEN

#include <string>
#include <vector>

using namespace std;

enum class TokenType {
     PROGRAM,
     VARIABLE,
     BEGIN,
     END,
     IF,
     THEN,
     ELSE,
     WHILE,
     DO,
     REPEAT,
     UNTIL,
     INTEGER,
     BOOLEAN,
     COLON,
     ATTRIBUTION,
     SUM,
     SUBTRACTION,
     MULTIPLICATION,
     DIVISION,
     MODULE,
     PROCEDURE,
     FUNCTION,
     EQUALS,
     DIFFERENT,
     BIGGER,
     SMALLER,
     BIGGER_EQUAL,
     SMALLER_EQUAL,
     NOT,
     OR,
     AND,
     XOR,
     LEFT_PARENTHESIS,
     RIGHT_PARENTHESIS,
     PERIOD,
     COMMA,
     SEMICOLON,
     WRITE,
     READ,
     TRUE,
     FALSE,
     IDENTIFIER,
     NUMBER,
     UNKNOWN
};

class Token
{
private:
    TokenType type;
    string token;
    static vector<string> symbols;
    int line;

public:
    Token(const string&, int) throw ();
    string getToken() const throw ();
    TokenType getType() const throw ();
    int getLine() const throw ();
};
#endif // TOKEN
