#include "Utility.h"
#include "Kernel.h"
#include "User.h"
#include "PageManager.h"
#include "Machine.h"
#include "MemoryDescriptor.h"
#include "Video.h"
#include "Assembly.h"

void Utility::MemCopy(unsigned long src, unsigned long des, unsigned int count)
{
	unsigned char* psrc = (unsigned char*)src;
	unsigned char* pdes = (unsigned char*)des;
	
	for ( unsigned int i = 0; i < count; i++ ) 
		pdes[i] = psrc[i];
}


/**
 * 快速内存拷贝。调用时，假设传入的内存可以互相对齐，否则性能会降低。
 *
 * 传统内存拷贝时，一次只能拷贝1个字节，速度较慢。
 * 本方法在观察到源地址和目的地址低 n 位相同时，
 * 每次拷贝一个 CPU 字长。其中，n 为机器字长除以字节长度 8。
 *
 * 例如，当 src 为 0x345A, dest 为 0x987A 时，
 * src 和 dest 的低 4 位相同。当机器字长为 4 位时，可以先使用单字节拷贝方法，
 * 将 src 和 dest 与内存对齐。将 src 和 dest 分别对齐到 0b..000，
 * 即可借助机器字长的优势，一次性拷贝更多字符。
 *
 * 覆盖问题：memcpy 本身不允许拷贝区间互相覆盖。因此，本方法不保证此方面安全性。
 * 短拷贝：当 src 和 dest 的距离小于一个寄存器的长度时，根本无法互相对齐。因此，不存在此问题。
 *
 * @param dest 拷贝目标地址。需要与 src 距离对齐，不要求与内存单元对齐。
 * @param src 拷贝源地址。
 * @param count 拷贝字符总数。
 * @return dest
 */
static inline void* memcpy_aligned(void* dest, const void* src, size_t count) {
    char* pDest = (char*) dest;
    char* pSrc = (char*) src;
    while (pSrc < ((char*) src) + count && (intptr_t(pSrc) & (sizeof(long) - 1))) {
        *pDest++ = *pSrc++;
    }

    while (pSrc + sizeof(long) - 1 < ((char*) src) + count) {
        *(long*) pDest = *(long*) pSrc;
        pSrc += sizeof(long);
        pDest += sizeof(long);
    }

    return dest;

}


void* Utility::MemCpy(void* dest, const void* src, size_t count) {

    // 如果 src 和 dest 可以互相对齐，则使用快速拷贝。
    if (!((intptr_t(dest) ^ intptr_t(src)) & (sizeof(long) - 1))) {
        return memcpy_aligned(dest, src, count);
    }

    // 单字节拷贝。
    char* pDest = (char*) dest;
    char* pSrc = (char*) src;
    while (pSrc < ((char*) src) + count) {
        *pDest++ = *pSrc++;
    }

    return dest;
}


void* Utility::MemSet(void* dest, int ch, size_t count) {
	char b = (char) ch;
    char* p = (char*) dest;
    while (p < ((char*) dest) + count) {
        *p++ = b;
    }

    return dest;
}


int Utility::MemCmp(const void* lhs, const void* rhs, long count)
{
	auto pLhs = (const unsigned char*) lhs;
    auto pRhs = (const unsigned char*) rhs;
    while (pRhs < ((const unsigned char*) rhs) + count) {
        if (*pLhs != *pRhs) {
            return *pLhs - *pRhs;
        } else {
            ++pLhs;
            ++pRhs;
        }
    }

    return 0;
}


int Utility::StrCmp(const char* lhs, const char* rhs) {
	const char* pLhs = lhs;
    const char* pRhs = rhs;

    while (*pLhs == *pRhs) {
        if (*pLhs == '\0') {
            return 0;
        } else {
            ++pLhs;
            ++pRhs;
        }
    }

    return *(const unsigned char*) pLhs - *(const unsigned char*) pRhs;
}


int Utility::CaluPageNeed(unsigned int memoryneed, unsigned int pagesize)
{
	int pageRequired = memoryneed / pagesize;
	pageRequired += memoryneed % pagesize ? 1 : 0;

	return pageRequired;
}

void Utility::StringCopy(char* src, char* dst)
{
	while ( (*dst++ = *src++) != 0 ) ;
}

int Utility::StringLength(char* pString)
{
	int length = 0;
	char* pChar = pString;

	while( *pChar++ )
	{
		length++;
	}

	/* 返回字符串长度 */
	return length;
}

void Utility::CopySeg2(unsigned long src, unsigned long des)
{
	PageTableEntry* userPageTable = (PageTableEntry*)Machine::Instance().GetUserPageTableArray();
	

	/*
	 * 先保存原用户态第一页与第二页PageTableEntry，因为下面的操作
	 * 将会将src所在页映射到0#目录表项，des映射到1#表项，最后进行copy
	 */
	unsigned long oriEntry1 = userPageTable[0].m_PageBaseAddress;
	unsigned long oriEntry2 = userPageTable[1].m_PageBaseAddress;	

	userPageTable[0].m_PageBaseAddress = src / PageManager::PAGE_SIZE;
	userPageTable[1].m_PageBaseAddress = des / PageManager::PAGE_SIZE;

	unsigned char* addressSrc = (unsigned char*)(src % PageManager::PAGE_SIZE);	
	//第二页virtual addess从4096开始
	unsigned char* addressDes = (unsigned char*)(PageManager::PAGE_SIZE + des % PageManager::PAGE_SIZE);	
	//需要刷新页表缓存
	FlushPageDirectory();

	*addressDes = *addressSrc;
	
	//恢复原页表映射
	userPageTable[0].m_PageBaseAddress = oriEntry1;
	userPageTable[1].m_PageBaseAddress = oriEntry2;
	FlushPageDirectory();
}

void Utility::CopySeg(unsigned long src, unsigned long des)
{
	PageTableEntry* PageTable = Machine::Instance().GetKernelPageTable().m_Entrys;

	/*
	 * 先保存原用户态第一页与第二页PageTableEntry，因为下面的操作
	 * 将会将src所在页映射到0#目录表项，des映射到1#表项，最后进行copy
	 */
	unsigned long oriEntry1 = PageTable[borrowedPTE].m_PageBaseAddress;
	unsigned long oriEntry2 = PageTable[borrowedPTE + 1].m_PageBaseAddress;

	PageTable[256].m_PageBaseAddress = src / PageManager::PAGE_SIZE;
	PageTable[257].m_PageBaseAddress = des / PageManager::PAGE_SIZE;

	unsigned char* addressSrc = (unsigned char*)(0xC0000000 + borrowedPTE*PageManager::PAGE_SIZE + src % PageManager::PAGE_SIZE);

	unsigned char* addressDes = (unsigned char*)(0xC0000000 + (borrowedPTE + 1)*PageManager::PAGE_SIZE + des % PageManager::PAGE_SIZE);
	//需要刷新页表缓存
	FlushPageDirectory();

	*addressDes = *addressSrc;

	//恢复原页表映射
	PageTable[borrowedPTE].m_PageBaseAddress = oriEntry1;
	PageTable[(borrowedPTE + 1)].m_PageBaseAddress = oriEntry2;
	FlushPageDirectory();
}

short Utility::GetMajor(const short dev)
{
	short major;
	major = dev >> 8;
	return major;
}

short Utility::GetMinor(const short dev)
{
	short minor;
	minor = dev & 0x00FF;
	return minor;
}

short Utility::SetMajor(short dev, const short value)
{
	dev &= 0x00FF;	/*  清除dev中原先高8比特 */
	dev |= (value << 8);
	return dev;
}

short Utility::SetMinor(short dev, const short value)
{
	dev &= 0xFF00;	/*  清除dev中原先低8比特 */
	dev |= (value & 0x00FF);	/* 仅保留value中的低8位 */
	return dev;
}

void Utility::Panic(const char* str)
{
	Diagnose::TraceOn();
	Diagnose::Write("%s\n", str);
	X86Assembly::CLI();
	for(;;);
}

void Utility::DWordCopy(int *src, int *dst, int count)
{
	while(count--)
	{
		*dst++ = *src++;
	}
	return;
}

int Utility::Min(int a, int b)
{
	if(a < b)
		return a;
	return b;
}

int Utility::Max(int a, int b)
{
	if(a > b)
		return a;
	return b;
}

int Utility::BCDToBinary( int value )
{
	return ( (value >> 4) * 10 + (value & 0xF) );
}

void Utility::IOMove(unsigned char* from, unsigned char* to, int count)
{
	while(count--)
	{
		*to++ = *from++;
	}
	return;
}

unsigned int Utility::MakeKernelTime( struct SystemTime* pTime )
{
	unsigned int timeInSeconds = 0;
	unsigned int days;
	int currentYear = 2000 + pTime->Year;	/* Year中只有年份后2位 */

	/* compute hours, minutes, seconds */
	timeInSeconds += pTime->Second;
	timeInSeconds += pTime->Minute * Utility::SECONDS_IN_MINUTE;
	timeInSeconds += pTime->Hour * Utility::SECONDS_IN_HOUR;

	/* compute days in current year */
	days = pTime->DayOfMonth - 1;
	days += Utility::DaysBeforeMonth[pTime->Month];
	if (Utility::IsLeapYear(currentYear) && pTime->Month >= 3 /* After February */)
		days++;

	/* compute days in previous years */
	for (int year = 1970; year < currentYear; year++)
	{
		days += Utility::DaysInYear(year);
	}
	timeInSeconds += days * Utility::SECONDS_IN_DAY;
	
	return timeInSeconds;
}

/* 某个月份前经过的天数，第0项不使用，未纳入计算闰年2月份29天 */
const unsigned int Utility::DaysBeforeMonth[13] = {0xFFFFFFFF/* Unused */, 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334};

bool Utility::IsLeapYear( int year )
{
	return (year % 4) == 0 && ( (year % 100) != 0 || (year % 400) == 0 );
}

unsigned int Utility::DaysInYear( int year )
{
	return IsLeapYear(year) ? 366 : 365;
}
