// SPDX-License-Identifier: MulanPSL-2.0
// https://github.com/FlowerBlackG/YurongOS/tree/master/src/lib/AsciiChar.h

/*
 * ASCII �����ַ����塣
 * ������ 2022��7��12�ա�
 * 
 * �ο���
 *   https://www.asciitable.com/
 */

#pragma once
#include "sys/types.h"

class AsciiChar {
public:
    static const uint8_t NUL = 0;
    static const uint8_t SOH = 1;
    static const uint8_t STX = 2;
    static const uint8_t ETX = 3;
    static const uint8_t EOT = 4;
    static const uint8_t ENQ = 5;
    static const uint8_t ACK = 6;
    static const uint8_t BEL = 7;
    static const uint8_t BS = 8;
    static const uint8_t TAB = 9;

    /** NL line feed, new line (n). */
    static const uint8_t LF = 10;
    static const uint8_t VT = 11;
    static const uint8_t FF = 12;

    /** carriage return (r). */
    static const uint8_t CR = 13;
    static const uint8_t SO = 14;
    static const uint8_t SI = 15;
    static const uint8_t DLE = 16;
    static const uint8_t DC1 = 17;
    static const uint8_t DC2 = 18;
    static const uint8_t DC3 = 19;
    static const uint8_t DC4 = 20;
    static const uint8_t NAK = 21;
    static const uint8_t SYN = 22;
    static const uint8_t ETB = 23;
    static const uint8_t CAN = 24;
    static const uint8_t EM = 25;
    static const uint8_t SUB = 26;
    static const uint8_t ESC = 27;
    static const uint8_t FS = 28;
    static const uint8_t GS = 29;
    static const uint8_t RS = 30;
    static const uint8_t US = 31;

    /** �ɼ��ַ���ʼλ�á����ո� */
    static const uint8_t START_OF_VISIBLE_CHARS = 32;
    static const uint8_t END_OF_VISIBLE_CHARS = 126;
};
