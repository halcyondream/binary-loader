#ifndef METADATA_H
#define METADATA_H

#include <string>

enum class BinaryType { BINARY_TYPE_AUTO, BINARY_TYPE_ELF, BINARY_TYPE_PE };

enum class BinaryArch { ARCH_NONE, ARCH_X86 };

class MetaData {
  public:
    MetaData() = default;
    void print();
    void setFilename(std::string filename);
    void setBinaryType(BinaryType type);
    void setArchitecture(BinaryArch arch);
    void setBits(unsigned bits);
    void setEntrypoint(uint64_t entrypoint);
    void setTypeString(std::string typeString);
    void setArchString(std::string archString);

  private:
    std::string filename; // Filename where binary exists.
    std::string type_str; // Name of binary type.
    std::string arch_str; // Name of architecture.
    unsigned bits;        // Bit width.
    uint64_t entrypoint;  // Entry point address.
    BinaryType type;      // Architecture type (from enum).
    BinaryArch arch;      // Architecture code (from enum).
};

#endif