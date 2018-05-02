#include <typeinfo>

#include "SymbolTable.h"

SymbolTable::SymbolTable() throw (): currentScope(0) {
    this->symbols.push_back(vector<Symbol*>()); 
}

void SymbolTable::insertSymbol (const Symbol& s) throw (string)
{

    for (auto itSymb = symbols.back().cbegin(); itSymb != symbols.back().cend(); itSymb++)
        if ((*itSymb)->getName() == s.getName())
            throw string ("Indentificador re-declarado" + s.getName());

    this->symbols.back().push_back(new Symbol(s));

    if (s.getType() == SymbolType::FUNCTION)
    {
        this->currentScope++;
        this->symbols.push_back(vector<Symbol*>());
    }
}

Symbol* SymbolTable::getSymbol (const string& name) const throw ()
{
    for (auto itScope = this->symbols.cbegin(); itScope != this->symbols.cend(); itScope++)
        for (auto itSymb = (*itScope).cbegin(); itSymb != (*itScope).cend(); itSymb++)
            if ((*itSymb)->getName() == name)
                return new Symbol(*(*itSymb));
    return nullptr;
}

void SymbolTable::clearCurrentScope() throw (string)
{
    if (this->symbols.empty())
        throw string ("Fim de bloco inválido");
    this->symbols.pop_back();
    this->currentScope--;
}