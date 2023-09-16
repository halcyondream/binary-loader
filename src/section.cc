#include "section.h"
#include <stdio.h>

Section::Section()
    : section_type(SectionType::NONE), vma(0), size(0), name("<unnamed>"),
      bytes(nullptr) {}

Section::Section(SectionType section_type, uint64_t vma, uint64_t size,
                 std::string name, uint8_t *data)
    : section_type(section_type), vma(vma), size(size), name(name) {
  bytes.reset(data);
}

bool Section::contains(uint64_t address) {
  return (address >= vma) && ((address - vma) < size);
}

std::string Section::getName() { return name; }

void Section::print() {
  printf("  0x%016lx %-8lu %-20s %s\n", vma, size, name.c_str(),
         section_type == SectionType::CODE ? "CODE" : "DATA");
}
