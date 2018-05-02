#ifndef SYMBOL_INCLUDED
#define SYMBOL_INCLUDED

#include <string>
#include <vector>

using namespace std;

enum class VariableType {
    BOOLEAN,
    INTEGER,
    VOID
};

enum class SymbolType {
    VARIABLE,
    FUNCTION
};

class Symbol
{
protected:
    string name;
    VariableType returnType;
    vector<Symbol>* params;
public:
    Symbol(const string& name, VariableType returnType) throw (string);
    Symbol(const string& name, VariableType returnType, const vector<Symbol> * const params) throw (string);
    Symbol(const Symbol&);
    Symbol(Symbol&&);
    Symbol& operator=(Symbol);
    virtual ~Symbol();

    string getName() const throw ();
    SymbolType getType() const throw();
    Symbol getParam(unsigned int) const throw(string);
    unsigned int getParamCount () const throw (string);
    VariableType getReturnType() const;
    VariableType getParamType(unsigned int) const throw(string);
    vector<Symbol>* getParams() const throw();
    bool operator== (const Symbol&) const throw ();
    bool operator!= (const Symbol&) const throw ();

    friend void swap(Symbol& a, Symbol& b) throw()
    {
        using std::swap;

        swap(a.name, b.name);
        swap(a.returnType, b.returnType);
        swap(a.params, b.params);
    }
};

#endif // SYMBOL_INCLUDED
