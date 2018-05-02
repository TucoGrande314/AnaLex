#include <iostream>
#include<string>

#include "Parser.h"
#include "Symbol.h"
//#include "ExpressionSolver.h"

using namespace std;

const string TipoPedacoNames[] = {"PROGRAM","VAR","BEGIN","END","IF","THEN","ELSE","WHILE","DO","REPEAT","UNTIL","INTEGER","BOOLEAN","COLON","ATTRIBUTION","SUM",
                                 "SUBTRACTION","MULTIPLICATION","DIVISION","MODULE","PROCEDURE","FUNCTION","=","!=",">","<",
                                 ">=","<=","NOT","OR","AND","XOR","(",")",".",",",";","WRITE","READ","TRUE","FALSE",
                                 "IDENTIFIER","CONSTANT","UNKNOWN"};

int main()
{
    cout << "Digite o nome do arquivo: ";
    string nomeArq;
    cin >> nomeArq;

    try
    {
        Parser p(nomeArq);
        p.compile();
        cout << "A compilação foi realizada com exito";
    }
    catch (string& s) {
        cout << s;
    }

    return 0;
}
