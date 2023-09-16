#ifndef SYMBOL_H
#define SYMBOL_H
#include <cstdint>
#include <string>

enum class SymbolType { UNKNOWN, FUNCTION };

class Symbol {
  public:
    Symbol();
    Symbol(SymbolType type, std::string name, uint64_t startAddress);
    void print();

  private:
    SymbolType type;
    std::string name;
    uint64_t startAddress;
};

#endif