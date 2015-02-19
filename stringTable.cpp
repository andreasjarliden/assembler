#include "stringTable.hpp"
#include <set>
#include <string>

namespace {

class SymbolTable {
public:
  const char* add(const char* symbol) {
    std::string stringSymbol(symbol);
    return _symbols.insert(symbol).first->c_str();
  }

private:
  typedef std::set<std::string> Symbols;
  Symbols _symbols;
};

}

const char* addSymbol(const char* symbol) {
  static SymbolTable SYMBOL_TABLE;
  return SYMBOL_TABLE.add(symbol);
}
