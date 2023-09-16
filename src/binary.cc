#include "binary.h"
#include <stdio.h>

Section *Binary::getTextSection() { return data.getTextSection(); }

void Binary::load(Loader &loader) { loader.loadData(data); }

void Binary::unload() {
  printf("TODO: Implement Binary::unload()\n");
  data.unload();
}

void Binary::printMetadata() { data.printMetadata(); }

void Binary::printSections() { data.printSections(); }

void Binary::printSymbols() { data.printSymbols(); }