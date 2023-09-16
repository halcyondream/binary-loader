// Workaround for 'config.h' error. See readme.
// TODO: Fix later, innocuous.
#define PACKAGE "your-program-name"
#define PACKAGE_VERSION "1.2.3"
#include "pbaloader.h"
#include "metadata.h"
#include "section.h"
#include "symbol.h"
#include <bfd.h>
#include <string>

enum class BfdStatus {
  OK,
  OUT_OF_MEMORY,
  FAILED_TO_OPEN_BINARY,
  UNSUPPORTED_BINARY,
  INITIALIZATION_FAILURE,
  SYMBOL_LOAD_FAILURE,
  SYMTAB_LOAD_FAILURE,
  SECTION_LOAD_FAILURE,
  METADATA_LOAD_FAILURE,
  SYMTAB_READ_FAILURE,
  DYNAMIC_SYMTAB_READ_FAILURE,
  SECTION_READ_FAILURE,
  UNSUPPORTED_ARCHITECTURE
};

PbaLoader::PbaLoader(std::string filename) : filename(filename) {}

static bfd *open_bfd(std::string filename) {
  static int bfd_initialized = 0;
  bfd *bfd_h;
  if (!bfd_initialized) {
    bfd_init();
    bfd_initialized = 1;
  }
  bfd_h = bfd_openr(filename.c_str(), NULL);
  if (!bfd_h) {
    fprintf(stderr, "Failed to open binary '%s' (%s)\n", filename.c_str(),
            bfd_errmsg(bfd_get_error()));
    return nullptr;
  }
  if (!bfd_check_format(bfd_h, bfd_object)) {
    fprintf(stderr, "File '%s' does not look like an executable (%s)\n",
            filename.c_str(), bfd_errmsg(bfd_get_error()));
    return nullptr;
  }
  bfd_set_error(bfd_error_no_error);
  if (bfd_get_flavour(bfd_h) == bfd_target_unknown_flavour) {
    fprintf(stderr, "Unrecognized format for binary '%s' (%s)\n",
            filename.c_str(), bfd_errmsg(bfd_get_error()));
    return nullptr;
  }
  return bfd_h;
}

static BfdStatus load_metadata_bfd(bfd *bfd_h, Data &data,
                                   std::string filename) {
  BfdStatus status = BfdStatus::OK;
  const bfd_arch_info_type *bfd_info;
  MetaData metadata;
  metadata.setFilename(filename);
  metadata.setEntrypoint(bfd_get_start_address(bfd_h));
  metadata.setTypeString(std::string(bfd_h->xvec->name));
  switch (bfd_h->xvec->flavour) {
  case bfd_target_elf_flavour:
    metadata.setBinaryType(BinaryType::BINARY_TYPE_ELF);
    break;
  case bfd_target_coff_flavour:
    metadata.setBinaryType(BinaryType::BINARY_TYPE_PE);
  default:
    fprintf(stderr, "Unsupported binary type (%s)\n", bfd_h->xvec->name);
    status = BfdStatus::UNSUPPORTED_BINARY;
  }
  bfd_info = bfd_get_arch_info(bfd_h); // TODO: Does this need to be free'd?
  metadata.setArchString(std::string(bfd_info->printable_name));
  switch (bfd_info->mach) {
  case bfd_mach_i386_i386:
    metadata.setArchitecture(BinaryArch::ARCH_X86);
    metadata.setBits(32);
    break;
  case bfd_mach_x86_64:
    metadata.setArchitecture(BinaryArch::ARCH_X86);
    metadata.setBits(64);
    break;
  default:
    fprintf(stderr, "Unsupported architecture (%s)\n",
            bfd_info->printable_name);
    status = BfdStatus::UNSUPPORTED_ARCHITECTURE;
  }
  data.setMetadata(metadata);
  return status;
}

static BfdStatus load_symbols_bfd(bfd *bfd_h, Data &data) {
  BfdStatus status = BfdStatus::OK;
  long n;
  long nsyms;
  asymbol **bfd_symtab = NULL;
  n = bfd_get_symtab_upper_bound(bfd_h);
  if (n < 0) {
    fprintf(stderr, "Failed to read symtab (%s)\n",
            bfd_errmsg(bfd_get_error()));
    status = BfdStatus::SYMTAB_LOAD_FAILURE;
    goto cleanup;
  }
  bfd_symtab = (asymbol **)malloc(n);
  if (!bfd_symtab) {
    fprintf(stderr, "Out of memory\n");
    status = BfdStatus::OUT_OF_MEMORY;
    goto cleanup;
  }
  nsyms = bfd_canonicalize_symtab(bfd_h, bfd_symtab);
  if (nsyms < 0) {
    fprintf(stderr, "Failed to read symtab (%s)\n",
            bfd_errmsg(bfd_get_error()));
    status = BfdStatus::SYMTAB_LOAD_FAILURE;
    goto cleanup;
  }
  for (long int i = 0; i < nsyms; i++) {
    if (!(bfd_symtab[i]->flags & BSF_FUNCTION)) {
      break;
    }
    data.addSymbol(new Symbol{SymbolType::FUNCTION,
                              std::string(bfd_symtab[i]->name),
                              bfd_asymbol_value(bfd_symtab[i])});
  }
cleanup:
  if (bfd_symtab) {
    free(bfd_symtab);
  }
  return status;
}

static BfdStatus load_dynsym_bfd(bfd *bfd_h, Data &data) {
  BfdStatus status = BfdStatus::OK;
  long n;
  long nsyms;
  asymbol **bfd_dynsym;
  bfd_dynsym = NULL;
  n = bfd_get_dynamic_symtab_upper_bound(bfd_h);
  if (n < 0) {
    fprintf(stderr, "Failed to read dynamic symtab (%s)\n",
            bfd_errmsg(bfd_get_error()));
    status = BfdStatus::DYNAMIC_SYMTAB_READ_FAILURE;
    goto cleanup;
  }
  bfd_dynsym = (asymbol **)malloc(n);
  if (!bfd_dynsym) {
    fprintf(stderr, "Out of memory\n");
    status = BfdStatus::OUT_OF_MEMORY;
    goto cleanup;
  }
  nsyms = bfd_canonicalize_dynamic_symtab(bfd_h, bfd_dynsym);
  if (nsyms < 0) {
    fprintf(stderr, "Failed to read dynamic symtab (%s)\n",
            bfd_errmsg(bfd_get_error()));
    status = BfdStatus::DYNAMIC_SYMTAB_READ_FAILURE;
    goto cleanup;
  }
  for (long int i = 0; i < nsyms; i++) {
    if (bfd_dynsym[i]->flags & BSF_FUNCTION) {
      data.addSymbol(new Symbol{SymbolType::FUNCTION,
                                std::string(bfd_dynsym[i]->name),
                                bfd_asymbol_value(bfd_dynsym[i])});
    }
  }
cleanup:
  if (bfd_dynsym) {
    free(bfd_dynsym);
  }
  return status;
}

static BfdStatus load_sections_bfd(bfd *bfd_h, Data &data) {
  BfdStatus status = BfdStatus::OK;
  int bfd_flags;
  uint64_t vma;
  uint64_t size;
  uint8_t *sectionData;
  const char *secname;
  asection *bfd_sec;
  SectionType sectype;
  sectionData = (uint8_t *)malloc(size);
  if (!sectionData) {
    status = BfdStatus::OUT_OF_MEMORY;
    goto cleanup;
  }
  for (bfd_sec = bfd_h->sections; bfd_sec; bfd_sec = bfd_sec->next) {
    bfd_flags = bfd_section_flags(bfd_sec);
    sectype = SectionType::NONE;
    if (bfd_flags & SEC_CODE) {
      sectype = SectionType::CODE;
    } else if (bfd_flags & SEC_DATA) {
      sectype = SectionType::DATA;
    } else {
      continue;
    }
    vma = bfd_section_vma(bfd_sec);
    size = bfd_section_size(bfd_sec);
    secname = bfd_section_name(bfd_sec);
    if (!bfd_get_section_contents(bfd_h, bfd_sec, sectionData, 0, size)) {
      fprintf(stderr, "Failed to read section '%s' (%s)\n", secname,
              bfd_errmsg(bfd_get_error()));
      status = BfdStatus::SECTION_READ_FAILURE;
      goto cleanup;
    }
    if (!secname) {
      secname = "<unnamed>";
    }
    data.addSection(new Section{sectype, vma, size, secname, sectionData});
  }
cleanup:
  if (sectionData) {
    free(sectionData);
  }
  return status;
}

void PbaLoader::loadData(Data &data) {
  bfd *bfd_h = open_bfd(filename); // TODO: Does bfd_h need to be free'd?
  if (bfd_h == nullptr) {
    return;
  }
  load_metadata_bfd(bfd_h, data, filename);
  load_symbols_bfd(bfd_h, data);
  load_dynsym_bfd(bfd_h, data);
  load_sections_bfd(bfd_h, data);
  if (bfd_h) {
    bfd_close(bfd_h);
  }
}
