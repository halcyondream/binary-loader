#include "data.h"
#include <memory>

void Data::unload() {
  for (auto section : sections) {
    delete section;
  }
  for (auto symbol : symbols) {
    delete symbol;
  }
}

void Data::setMetadata(MetaData metadata) { this->metadata = metadata; }
void Data::addSection(Section *section) { sections.push_back(section); }
void Data::addSymbol(Symbol *symbol) { symbols.push_back(symbol); }

void Data::printMetadata() { metadata.print(); }

void Data::printSections() {
  for (auto section : sections) {
    section->print();
  }
}

void Data::printSymbols() {
  for (auto symbol : symbols) {
    symbol->print();
  }
}

Section *Data::getTextSection() {
  for (auto s : sections) {
    if (s->getName() == "text") {
      return s;
    }
  }
  return nullptr;
}
