// ELFParser.h
// 2051565 GTY
// created on 2024.7.14


// https://www.wikiwand.com/en/Executable_and_Linkable_Format
// https://wiki.osdev.org/ELF

#pragma once

#include "../libyrosstd/sys/types.h"

#include "INode.h"

namespace ELF {


enum class ObjectFileType : uint16_t {
    ET_NONE = 0x00,
    ET_REL = 0x01,
    ET_EXEC = 0x02,
    ET_DYN = 0x03,
    ET_CORE = 0x04,
    ET_LOOS = 0xFE00,
    ET_FEFF = 0xFEFF,
    ET_LOPROC = 0xFF00,
    ET_HIPROC = 0xFFFF
};


enum class MachineISA : uint16_t {
    x86 = 0x03,
    amd_x86_64 = 0x3E,
    risc_v = 0xF3,
    aarch64 = 0xB7,
    loongarch = 0x102
};


struct ELFHeaderCommon {
    /* 0x7F 'E' 'L' 'F' */
    int8_t magic[4];

    /* 1 for 32-bit, 2 for 64-bit */
    int8_t bits;

    /* 1 for little, 2 for big endianness */
    int8_t endianness;

    int8_t version; // should be 1

    int8_t abi;

    int8_t abiVersion;

    int8_t paddings[7];

    ObjectFileType type;

    MachineISA isa;

    int32_t elfVersion; // should be 1

} __packed;


struct ELFHeader32 : ELFHeaderCommon {
    // here lies ...
    // shared fields before e_entry at offset 0x18
    // ...


    // memory address of the entry point from where the process starts executing.
    int32_t entry;

    /* start of the program header table */
    int32_t phoff;

    /* start of the section header table */
    int32_t shoff;
    
    int32_t flags;
    int16_t ehsize;

    /* size of the program header table entry */
    int16_t phentsize;

    /* number of entries in the program header table */
    int16_t phnum;

    /* size of a section header table entry */
    int16_t shentsize;

    /* number of entries in the section header table */
    int16_t shnum;

    /* index of the section header table entry that contains the section names */
    int16_t shstrndx;

} __packed;


enum class ProgramHeaderType : int32_t {
    PT_NULL = 0x00000000,
    PT_LOAD = 0x00000001
};


const int PROGRAM_HEADER32_FLAGS_PF_X_BIT = 0x1;
const int PROGRAM_HEADER32_FLAGS_PF_W_BIT = 0x2;
const int PROGRAM_HEADER32_FLAGS_PF_R_BIT = 0x4;


struct ProgramHeader32 {
    ProgramHeaderType type;
    int32_t offset;
    int32_t vaddr;
    int32_t paddr;
    int32_t fileSize;
    int32_t memorySize;
    int32_t flags;
    int32_t align;
} __packed;


struct SectionHeader32 {
    /* An offset to a string in the .shstrtab section that represents the name of this section */
    int32_t name;

    int32_t type;
    int32_t flags;
    int32_t addr;
    int32_t offset;
    int32_t size;
    int32_t link;
    int32_t info;
    int32_t addrAlign;
    int32_t entSize;

} __packed;


class Parser {

public:
    int load(Inode* inode);
    int relocate(Inode* inode, bool sharedText);

    ~Parser();

    const char* nameOf(const SectionHeader32&);

public:
    uintptr_t entryPointAddr = 0;
    uintptr_t textAddr = 0;
    size_t textSize = 0;
    uintptr_t dataAddr = 0;
    size_t dataSize = 0;
    size_t stackSize = 0;
    size_t heapSize = 0;

    ELFHeader32 elfHeader32;
    ProgramHeader32* programHeaders = nullptr;
    SectionHeader32* sectionHeaders = nullptr;

    char* strtabData = nullptr;

protected:


};


#if 0
static void __test() {
    auto x = sizeof(ProgramHeader32);
}
#endif


}  // namespace ELF

