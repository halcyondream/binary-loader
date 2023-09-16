#ifndef BINARY_H
#define BINARY_H

#include "data.h"
#include "loader.h"
#include "section.h"
#include <string>

class Binary {
  public:
    Binary() = default;
    Section *getTextSection();
    void load(Loader &loader);
    void unload();
    void printMetadata();
    void printSections();
    void printSymbols();

  private:
    Data data;
};

#endif