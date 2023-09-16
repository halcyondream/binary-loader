#ifndef SECTION_H
#define SECTION_H
#include <cstdint>
#include <memory>
#include <string>

enum class SectionType { NONE, CODE, DATA };

class Section {
  public:
    Section();
    Section(SectionType section_type, uint64_t vma, uint64_t size,
            std::string name, uint8_t *data);
    std::string getName();
    bool contains(uint64_t address);
    void print();

  private:
    std::string name;
    SectionType section_type;
    uint64_t vma;
    uint64_t size;
    std::unique_ptr<uint8_t> bytes;
};

#endif