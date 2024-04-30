// SPDX-License-Identifier: MulanPSL-2.0

/*
 * �ַ�����������
 * ������ 2022��7��4�ա�
 */

#include <libyrosstd/string.h>


char* strcpy(char* dest, const char* src) {
    char* pDest = dest;
    const char* pSrc = src;
    while ((*pDest++ = *pSrc++) != '\0')
        ;

    return dest;
}

char* strncpy(char* dest, const char* src, size_t count) {
    char* pDest = dest;
    const char* pSrc = src;

    while (pDest < ((char*) dest) + count) {
        *pDest = *pSrc;
        if (*pDest == '\0') {
            break;
        } else {
            ++pDest;
            ++pSrc;
        }
    }

    *pDest = '\0';

    return dest;
}


char* strcat(char* dest, const char* src) {
    strcpy(dest + strlen(dest), src);
    return dest;
}

char* strncat(char* dest, const char* src, size_t count) {
    strncpy(dest + strlen(dest), src, count);
    return dest;
}


size_t strlen(const char* str) {
    // �ο� glibc ��ʵ�ַ�ʽ��
    // ���ͼ���https://blog.csdn.net/m0_62405272/article/details/125600719
    
    /* �Ƚ�ָ�����ֽڶ��롣
       ��ָ��ת���� long ���ֲ�������Щƽ̨�ᱨ��ע�⣬���Ⱥŵ����ȼ����������㡣 */
    const char* charPtr = str;
    while (((long) charPtr & (sizeof(long) - 1)) != 0) {
        if (*charPtr == '\0') {
            return charPtr - str;
        } else {
            ++charPtr;
        }
    }

    const unsigned long* longPtr = (const unsigned long*) charPtr;

    unsigned long highMagic = 0x80808080L;
    unsigned long lowMagic = 0x01010101L;
    if (sizeof(long) > 4) {
        highMagic = ((highMagic << 16) << 16) | highMagic;
        lowMagic = ((lowMagic << 16) << 16) | lowMagic;
    }
    // �ݲ����� long �Ŀ�ȳ��� 8 �ֽڵ������

    while (true) {
        const unsigned long& longWords = *longPtr++;
        if (((longWords - lowMagic) & ~longWords & highMagic) != 0) {
            auto prevLongPtr = longPtr - 1;
            const char* p = (const char*) prevLongPtr;
            while (p < ((const char*) prevLongPtr) + sizeof(long)) {
                if (*p == '\0') {
                    return p - str;
                } else {
                    ++p;
                }
            }
        }
    }
}


int strcmp(const char* lhs, const char* rhs) {
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


int strncmp(const char* lhs, const char* rhs, size_t count) {
    const char* pLhs = lhs;
    const char* pRhs = rhs;

    while (pLhs < lhs + count) {
        if (*pLhs != *pRhs) {
            return *(const unsigned char*) pLhs - *(const unsigned char*) pRhs;
        } 
        else if (*pLhs == '\0') {
            return 0;
        } 
        else {
            ++pLhs;
            ++pRhs;
        }
    }

    return 0;
}

char* strchr(const char* str, int ch) {
    char b = (char) ch;
    const char* p = str;
    while (*p != '\0') {
        if (b == *p) {
            return (char*) p;
        } else {
            ++p;
        }
    }

    return NULL;
}

char* strrchr(const char* str, int ch) {
    char b = (char) ch;
    const char* p = str + strlen(str) - 1;

    while (str <= p) {
        if (b == *p) {
            return (char*) p;
        } else {
            --p;
        }
    }

    return NULL;
}

size_t strspn(const char* dest, const char* src) {
    bool tar[256]; // �ݲ����Ƕ� C ���Եļ������⡣
    memset(tar, 0, sizeof(tar));

    const unsigned char* p = (const unsigned char*) src;
    while (*p != '\0') {
        tar[*p++] = true;
    }

    p = (const unsigned char*) dest;
    while (tar[*p++])
        ;
    
    return p - (const unsigned char*) dest - 1;
}

size_t strcspn(const char* dest, const char* src) {
    bool tar[256]; // �ݲ����Ƕ� C ���Եļ������⡣
    memset(tar, 0xff, sizeof(tar));

    const unsigned char* p = (const unsigned char*) src;
    while (*p != '\0') {
        tar[*p++] = false;
    }

    p = (const unsigned char*) dest;
    while (tar[*p++])
        ;
    
    return p - (const unsigned char*) dest - 1;
}

char* strpbrk(const char* dest, const char* breakset) {
    bool tar[256];
    memset(tar, 0, sizeof(tar));
;
    const unsigned char* p = (const unsigned char*) breakset;
    while (*p != '\0') {
        tar[*p++] = true;
    }

    p = (const unsigned char*) dest;
    while (*p != '\0') {
        if (tar[*p]) {
            return (char*) p;
        } else {
            ++p;
        }
    }

    return NULL;
}

char* strstr(const char* str, const char* substr) {
    size_t lenOfSubstr = strlen(substr);
    size_t lenOfStr = strlen(str);
    const char* pStr = str;
    while (lenOfSubstr + (pStr - str) <= lenOfStr) {
        if (strncmp(pStr, substr, lenOfSubstr) == 0) {
            return (char*) pStr;
        } else {
            ++pStr;
        }
    }

    return NULL;

    // ע�⣺���������㷨Ч�ʺܵ͡����Կ����о����õ��㷨��
}

char* strtok(char* str, const char* delim) {
    bool tar[256];
    memset(tar, 0, sizeof(tar));

    const unsigned char* p = (const unsigned char*) delim;
    while (*p != 0) {
        tar[*p++] = true;
    }

    static unsigned char* searchPtr = (unsigned char*) str;
    if (str != NULL) {
        searchPtr = (unsigned char*) str;
    }

    char* pToken = NULL;
    while (*searchPtr != '\0') {
        if (p[*searchPtr]) {
            if (pToken != NULL) {
                *searchPtr = '\0';
                ++searchPtr;
                return pToken;
            }
        } else if (pToken == NULL) {
            pToken = (char*) str;
        }

        ++searchPtr;
    }

    return NULL;
}

void* memchr(const void* ptr, int ch, size_t count) {
    auto* cPtr = (const unsigned char*) ptr;
    unsigned char b = ch;
    while (cPtr < ((const unsigned char*) ptr) + count) {
        if (b == *cPtr) {
            return (void*) cPtr;
        } else {
            ++cPtr;
        }
    }

    return nullptr;
}

int memcmp(const void* lhs, const void* rhs, size_t count) {
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


void* memset(void* dest, int ch, size_t count) {
    char b = (char) ch;
    char* p = (char*) dest;
    while (p < ((char*) dest) + count) {
        *p++ = b;
    }

    return dest;
}

/**
 * �����ڴ濽��������ʱ�����贫����ڴ���Ի�����룬�������ܻή�͡�
 *
 * ��ͳ�ڴ濽��ʱ��һ��ֻ�ܿ���1���ֽڣ��ٶȽ�����
 * �������ڹ۲쵽Դ��ַ��Ŀ�ĵ�ַ�� n λ��ͬʱ��
 * ÿ�ο���һ�� CPU �ֳ������У�n Ϊ�����ֳ������ֽڳ��� 8��
 *
 * ���磬�� src Ϊ 0x345A, dest Ϊ 0x987A ʱ��
 * src �� dest �ĵ� 4 λ��ͬ���������ֳ�Ϊ 4 λʱ��������ʹ�õ��ֽڿ���������
 * �� src �� dest ���ڴ���롣�� src �� dest �ֱ���뵽 0b..000��
 * ���ɽ��������ֳ������ƣ�һ���Կ��������ַ���
 *
 * �������⣺memcpy �������������以�า�ǡ���ˣ�����������֤�˷��氲ȫ�ԡ�
 * �̿������� src �� dest �ľ���С��һ���Ĵ����ĳ���ʱ�������޷�������롣��ˣ������ڴ����⡣
 *
 * @param dest ����Ŀ���ַ����Ҫ�� src ������룬��Ҫ�����ڴ浥Ԫ���롣
 * @param src ����Դ��ַ��
 * @param count �����ַ�������
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


void* memcpy(void* dest, const void* src, size_t count) {

    // ��� src �� dest ���Ի�����룬��ʹ�ÿ��ٿ�����
    if (!((intptr_t(dest) ^ intptr_t(src)) & (sizeof(long) - 1))) {
        return memcpy_aligned(dest, src, count);
    }

    // ���ֽڿ�����
    char* pDest = (char*) dest;
    char* pSrc = (char*) src;
    while (pSrc < ((char*) src) + count) {
        *pDest++ = *pSrc++;
    }

    return dest;
}
