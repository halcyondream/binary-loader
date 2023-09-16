#include "symbol.h"
#include "scopedenums.h"
#include <cstdio>

Symbol::Symbol() : type(SymbolType::UNKNOWN), name(), startAddress(0) {}

Symbol::Symbol(SymbolType type, std::string name, uint64_t startAddress)
    : type(type), name(name), startAddress(startAddress) {}

void Symbol::print() {
  printf("  %-40s 0x%016lx %s\n", name.c_str(), startAddress,
         (type & SymbolType::FUNCTION) ? "FUNC" : "");
}