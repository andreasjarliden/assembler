#include "stringTable.hpp"
#include <set>
#include <string>

namespace {

class SymbolTable {
public:
  const char* add(const std::string symbol) {
    return _symbols.insert(symbol).first->c_str();
  }

private:
  typedef std::set<std::string> Symbols;
  Symbols _symbols;
};

SymbolTable SYMBOL_TABLE;

}

const char* addSymbol(const char* symbol) {
  return SYMBOL_TABLE.add(symbol);
}

const char* addString(const char* s) {
  std::string strippedQuotes(s + 1, strlen(s) - 2);
  return SYMBOL_TABLE.add(strippedQuotes);
}
