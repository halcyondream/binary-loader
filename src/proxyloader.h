#ifndef PROXY_LOADER
#define PROXY_LOADER
#include "data.h"
#include "loader.h"

class ProxyLoader : public Loader {
  public:
    ProxyLoader() = default;
    void loadData(Data &data);
};

#endif