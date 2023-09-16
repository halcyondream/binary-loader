#ifndef DATA_H
#define DATA_H

#include "metadata.h"
#include "section.h"
#include "symbol.h"
#include <memory>
#include <vector>

class Data {
  public:
    Data() = default;
    void setMetadata(MetaData metadata);
    void addSection(Section *section);
    void addSymbol(Symbol *symbol);
    Section *getTextSection();
    void printMetadata();
    void printSections();
    void printSymbols();
    void unload();

  private:
    MetaData metadata;
    std::vector<Section *> sections;
    std::vector<Symbol *> symbols;
};

#endif