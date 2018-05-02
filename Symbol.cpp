#include "Symbol.h"

using namespace std;

//SIMBOLO

Symbol::Symbol(const string& name, VariableType returnType) throw(string)
    : Symbol(name, returnType, nullptr) {}

Symbol::Symbol(const string& name, VariableType returnType, const vector<Symbol>* const params) throw(string)
    : name(name) {
    this->returnType = returnType;
    if (params != nullptr) {
        for (auto it =  params->cbegin(); it != params->cend(); it++)
            if (it->getType() != SymbolType::VARIABLE)
                throw string("Os parâmetros de um procedimento ou função devem ser uma variável");

        this->params = new vector<Symbol>(*params);
    }
    else
        this->params = nullptr;
}

Symbol::Symbol(const Symbol& other) : name(other.name), returnType(other.returnType), params(nullptr) {
    if (other.params != nullptr)
        this->params = new vector<Symbol>(*other.params);
}

Symbol::Symbol(Symbol&& other) : name(other.name), returnType(other.returnType), params(other.params) {}

Symbol& Symbol::operator= (Symbol other) {
    swap(*this, other);

    return *this;
}

Symbol::~Symbol() {
    delete this->params;
    this->params = nullptr;
}



string Symbol::getName() const throw () {
    return string(this->name);
}

SymbolType Symbol::getType() const throw() {
    if (this->params == nullptr)
        return SymbolType::VARIABLE;

    return SymbolType::FUNCTION;
}

VariableType Symbol::getReturnType() const {
    return this->returnType;
}

VariableType Symbol::getParamType(unsigned int i) const throw(string) {
    return this->getParam(i).getReturnType();
}

Symbol Symbol::getParam(unsigned int i) const throw(string) {
    if (this->params == nullptr)
        throw string("Não foi possível ler as variáveis passadas como parâmetro");
    if (this->params->size() < i)
        throw string ("Indíce fora de alcance");

    return Symbol(this->params->at(i));
}

unsigned int Symbol::getParamCount() const throw(string) {
    if (this->params == nullptr)
        throw string("Não foi possível ler as variáveis passadas como parâmetro");
    return this->params->size();
}

vector<Symbol>* Symbol::getParams() const throw() {
    if (this->params == nullptr)
        return nullptr;

    vector<Symbol> v(*this->params);
    return &v;
}

bool Symbol::operator!=(const Symbol& other) const throw () {
    return !this->operator==(other);
}

bool Symbol::operator==(const Symbol& other) const throw () {
    return this->name != other.name;
}
