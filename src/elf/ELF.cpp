// ELFParser.cpp
// 2051565 GTY
// created on 2024.7.15


#include "ELF.h"
#include "Kernel.h"
#include "Video.h"
#include "Machine.h"
#include "libyrosstd/string.h"


namespace ELF {

Parser::~Parser() {
    auto& kpm = Kernel::Instance().GetKernelPageManager();

    if (this->programHeaders) {

        uintptr_t addr = (uintptr_t) this->programHeaders;
        addr -= 0xC0000000;  // sure to be hard-coded?

        size_t size = elfHeader32.phentsize * elfHeader32.phnum;
        
        size += 4095;
        size &= 0xFFFFF000;  // for 32-bit
        
        kpm.FreeMemory(size, addr);

        this->programHeaders = nullptr;
    }

    if (this->sectionHeaders) {
        
        uintptr_t addr = (uintptr_t) this->sectionHeaders;
        addr -= 0xC0000000;  // sure to be hard-coded?

        size_t size = elfHeader32.shentsize * elfHeader32.shnum;
        
        size += 4095;
        size &= 0xFFFFF000;  // for 32-bit
        
        kpm.FreeMemory(size, addr);

        this->sectionHeaders = nullptr;
    }

    if (this->strtabData) {
        uintptr_t addr = (uintptr_t) this->strtabData;
        addr -= 0xC0000000;
        size_t size = sectionHeaders[elfHeader32.shstrndx].size;
        size += 4095;
        size &= 0xFFFFF000;
        kpm.FreeMemory(size, addr);
        this->strtabData = nullptr;
    }

}


int Parser::load(Inode* inode) {

// todo
    Utility::Panic("ELF Parser is buggy. Check include/ elf/elf.h for details.\n");
    
    User& u = Kernel::Instance().GetUser();
    KernelPageManager& kpm = Kernel::Instance().GetKernelPageManager();

    // read elf header
    u.u_IOParam.m_Base = (unsigned char*) &this->elfHeader32;
    u.u_IOParam.m_Offset = 0;
    u.u_IOParam.m_Count = sizeof(this->elfHeader32);  // read 52 bytes for 32-bit elf header.

    inode->ReadI();

    // check magic.

    if (
        elfHeader32.magic[0] != 0x7F
        || elfHeader32.magic[1] != 'E'
        || elfHeader32.magic[2] != 'L'
        || elfHeader32.magic[3] != 'F'
    ) {
        Diagnose::Write("ELF Parser: Bad magic!\n");
        return -1;
    }

    if (elfHeader32.isa != ELF::MachineISA::x86) {
        Diagnose::Write("ELF Parser: Bad ISA!\n");
        return -2;
    }

    if ((uintptr_t) elfHeader32.entry >= 0xC0000000) {

        Diagnose::Write("ELF Parser: Don't hack kernel space!\n");
        return -3;
    }

    // todo: validate other fields


    this->entryPointAddr = elfHeader32.entry;


    // read program header table

    {
        size_t size = elfHeader32.phentsize * elfHeader32.phnum;
        size += PageManager::PAGE_SIZE - 1;
        size = size / PageManager::PAGE_SIZE * PageManager::PAGE_SIZE;
        uintptr_t alloc = kpm.AllocMemory(size);
        if (alloc) {
            alloc += 0xC0000000;
        } else {
            Diagnose::Write("ELF Parser: Failed to alloc program header table!\n");
            return -1;
        }

        this->programHeaders = (ProgramHeader32*) alloc;
        u.u_IOParam.m_Base = (unsigned char*) this->programHeaders;
        u.u_IOParam.m_Offset = elfHeader32.phoff;
        u.u_IOParam.m_Count = elfHeader32.phentsize * elfHeader32.phnum;
        inode->ReadI();
    }


    // read section header table


    {
        size_t size = elfHeader32.shentsize * elfHeader32.shnum;
        size += PageManager::PAGE_SIZE - 1;
        size = size / PageManager::PAGE_SIZE * PageManager::PAGE_SIZE;
        uintptr_t alloc = kpm.AllocMemory(size);
        if (alloc) {
            alloc += 0xC0000000;
        } else {
            Diagnose::Write("ELF Parser: Failed to alloc section header table!\n");
            return -1;
        }

        this->sectionHeaders = (SectionHeader32*) alloc;
        u.u_IOParam.m_Base = (unsigned char*) this->sectionHeaders;
        u.u_IOParam.m_Offset = elfHeader32.shoff;
        u.u_IOParam.m_Count = elfHeader32.shentsize * elfHeader32.shnum;
        inode->ReadI();
    }

    // load section names.

    {
        auto& sec = sectionHeaders[elfHeader32.shstrndx];
        uintptr_t off = sec.offset;
        size_t sizeReal = sec.size;
        size_t sizeAlloc = sizeReal + PageManager::PAGE_SIZE - 1;
        sizeAlloc /= PageManager::PAGE_SIZE;
        sizeAlloc *= PageManager::PAGE_SIZE;
        auto buf = (char*) kpm.AllocMemory(sizeAlloc);
        
        if (buf) {
            buf += 0xC0000000;
        } else {
            return -1;
        }

        this->strtabData = buf;
        
        u.u_IOParam.m_Base = (unsigned char*) buf;
        u.u_IOParam.m_Offset = off;
        u.u_IOParam.m_Count = sizeReal;
        inode->ReadI();

    }


    this->stackSize = 4096;  // todo: really set to this number?
    this->heapSize = 4096;  // todo: 4096 is just copied from Portable Executable generated by GNU Ld.


    // we assume that data is following read-only things...
    // todo: this logic should be corrected.


#define NUM_MAX(a, b) (a > b ? a : b)
#define NUM_MIN(a, b) (a < b ? a : b)


    for (int idx = 0; idx < elfHeader32.phnum; idx++) {
        auto& ent = programHeaders[idx];
        if (ent.type != ProgramHeaderType::PT_LOAD) {
            continue;
        }



        if (
            (ent.flags & PROGRAM_HEADER32_FLAGS_PF_R_BIT)
            && !(ent.flags & PROGRAM_HEADER32_FLAGS_PF_W_BIT)
        ) {
            // is read-only thing.

            if (!textAddr) {
                textAddr = ent.vaddr;
            } else {
                textAddr = NUM_MIN(textAddr, ent.vaddr);
            }

            textSize = NUM_MAX(
                textSize,
                ent.vaddr + ent.memorySize - textAddr
            );
        } 
        else 
        {
            // is RW thing.

            if (!dataAddr) {
                dataAddr = ent.vaddr;
            } else {
                dataAddr = NUM_MIN(dataAddr, ent.vaddr);
            }


            dataSize = NUM_MAX(
                dataSize,
                ent.vaddr + ent.memorySize - dataAddr
            );
        }

    }

#undef NUM_MIN
#undef NUM_MAX

    return 0;
}


int Parser::relocate(Inode* inode, bool sharedText) {

    // Copied from PEParser::Relocate. Modified just a little.
    // It's 11:31 PM now when I'm writing this. Tired aaaaaaaa.... 

    User& u = Kernel::Instance().GetUser();
    uintptr_t srcAddr, dstAddr;


    PageTable* pUserPageTable = Machine::Instance().GetUserPageTableArray();
    uintptr_t textBegin = this->textAddr >> 12;
    uintptr_t textLength = this->textSize >> 12;
    auto ptr = (PageTableEntry*) pUserPageTable;

    if (!sharedText) {
        for (int i = textBegin; i < textBegin + textLength; i++) {
            ptr[i].m_ReadWriter = 1;
        }

        FlushPageDirectory();
    }

    for (int i = 0; i < elfHeader32.phnum; i++) {
        auto& ent = programHeaders[i];
        if (ent.type != ProgramHeaderType::PT_LOAD) {
            continue;
        }

        if (ent.flags & PROGRAM_HEADER32_FLAGS_PF_W_BIT) {
            // data or bss. all set to 0.

            memset((void*) ent.vaddr, 0, ent.memorySize);
        
        } else if (sharedText) {
            // read-only things

            continue;
        }

        if (ent.fileSize) {
            u.u_IOParam.m_Base = (unsigned char*) ent.vaddr;
            u.u_IOParam.m_Offset = ent.offset;
            u.u_IOParam.m_Count = ent.fileSize;

            inode->ReadI();
        }
    }


    if (!sharedText) {
        for (int i = textBegin; i < textBegin + textLength; i++) {
            ptr[i].m_ReadWriter = 0;
        }

        FlushPageDirectory();
    }

    return 0;
}


const char* Parser::nameOf(const SectionHeader32& sec) {
    return &this->strtabData[sec.name];
}


}  // namespace ELF
