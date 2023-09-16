#include "src/binary.h"
#include "src/loader.h"
#ifdef USE_PBA_LOADER
  #include "src/pbaloader.h"
#else 
  #include "src/proxyloader.h"
#endif
#include <iostream>
#include <memory>

int main(int argc, char *argv[]) {
  
  std::string fname;
  Binary binary;

  if (argc < 2) {
    printf("Usage: %s <binary>\n", argv[0]);
    return 1;
  }

  fname.assign(argv[1]);

#ifdef USE_PBA_LOADER
  PbaLoader loader{fname};
#else
  // TODO: Why does a smart pointer fail?
  //auto loader = std::unique_ptr<ProxyLoader>();
  ProxyLoader loader;
#endif

  std::cout 
    << "Loading binary from " 
    << fname 
    << " ..." 
    << std::endl; 

  binary.load(loader);

  std::cout << "Binary information" << std::endl;
  binary.printMetadata();

  std::cout << "Printing sections..." << std::endl;
  binary.printSections();

  std::cout << "Printing symbols..." << std::endl;
  binary.printSymbols();

  binary.unload();

  return 0;
}