#include "metadata.h"
#include <cstdio>

void MetaData::print() {
  printf("Binary data: '%s' %s/%s (%u bits) entry@0x%016lx\n", filename.c_str(),
         type_str.c_str(), arch_str.c_str(), bits, entrypoint);
}

void MetaData::setFilename(std::string filename) { this->filename = filename; }
void MetaData::setBinaryType(BinaryType type) { this->type = type; }
void MetaData::setArchitecture(BinaryArch arch) { this->arch = arch; }
void MetaData::setBits(unsigned bits) { this->bits = bits; }
void MetaData::setEntrypoint(uint64_t entrypoint) {
  this->entrypoint = entrypoint;
}
void MetaData::setTypeString(std::string typeString) {
  this->type_str = typeString;
}
void MetaData::setArchString(std::string archString) {
  this->arch_str = archString;
}