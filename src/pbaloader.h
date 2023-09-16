#ifndef PBA_LOADER_H
#define PBA_LOADER_H
#include "data.h"
#include "loader.h"

class PbaLoader : public Loader {
  public:
    PbaLoader(std::string filename);
    void loadData(Data &data);

  private:
    std::string filename;
};

#endif
