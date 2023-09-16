#ifndef LOADER_H
#define LOADER_H
#include "data.h"

class Loader {
  public:
    virtual void loadData(Data &data) = 0;
};

#endif