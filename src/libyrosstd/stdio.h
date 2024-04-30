// https://github.com/FlowerBlackG/YurongOS/blob/i386-archive-v0.0.1/src/include/stdio.h
/*
 * ��׼�������ͷ�ļ���
 * ������ 2022��7��13�ա�
 * 
 * �ο���
 *   https://pubs.opengroup.org/onlinepubs/9699919799/
 *   https://cplusplus.com/reference/cstdio/sprintf/
 *   https://cplusplus.com/reference/cstdio/vsprintf/
 *   https://cplusplus.com/reference/cstdio/printf/
 */

#pragma once

#include <libyrosstd/sys/types.h>
#include <libyrosstd/stdarg.h>

// �ļ���β��ʾֵ��
#ifndef EOF
    #define EOF -1
#endif

/*
  ��ʽ������ĸ�ʽ��
    %[flags][width][.precision][length]specifier

  flag:
    -    �ռ�������루��ȹ���ʱ����Ĭ���Ҷ��롣
    +    ǿ�ƶ����ּ��������š�
    �ո�  �Բ���Ҫ�ӷ��ŵ����֣����һ���ո�
    #    �������ʽΪ8/16����ʱ�������Ƿ�ǰ׺0/0x/0X��
         �������ʽΪ����ʱ��ǿ�������š�
    0    ��ǰ׺ 0 �����λ�á�

  width:
    ���� ��С�ַ�������ȡ�
    *   �ɵ�ǰָ�������int��ָ������һ��������Ϊ���Ŀ�ꡣ

  .precision:
    .����
    .*    �ɵ�ǰָ�������int��ָ������һ��������Ϊ���Ŀ�ꡣ

  specifier:
    d/i �з���10����������
    u   �޷���10����������
    o   �޷���8����������
    x   �޷���16����������7fa����
    X   �޷���16������������ĸ��д����
    f   10���Ƹ�������
    F   10���Ƹ���������д����
    e   ��ѧ��������3.9265e+2����
    E   ��ѧ����������д����3.9265E+2����
    g   ʹ�� %e �� %f �н϶̵ġ�
    G   ʹ�� %E �� %F �н϶̵ġ�
    a   16���Ƹ�������
    A   16���Ƹ���������д����
    c   �ַ���
    s   �ַ�����
    p   ָ���ַ��b8000����
    n   ������κζ�������Ӧ�����������з���������signed int����
    %   ����ٷֺ� % ����

  length specifier:
    ���Ըı����ݳ���
      hh + ��������/n   char
      h  + ��������/n   short
      l  + ��������/ncs long
      ll + ��������/n   long long
      j  + ��������/n   intmax_t / uintmax_t
      z  + ��������/n   size_t
      t  + ��������/n   ptrdiff_t         *��δʵ��
      L  + ��������     long double

*/

/**
 * ����ʽ������д���ַ�����
 */
int sprintf(char* buffer, const char* format, ...);

/**
 * �����Կɱ������ĸ�ʽ������д���ַ�����
 * 
 * ��ʽ�������߼���ֱ�ӷ��� -1��
 *     �����һ���ַ��ǵ����� % ʱ�����ܲ��᷵�ش��󣬵������ %��
 * ��ʽ�Ƿ������߼���
 *     width ���� int �Ͻ磺δ�������Ϊ��ѭ����10����Ȼ�������
 * 
 * @todo ��ʱ�޷�����64λ�������ͼ������������͡�
 * @todo �ݲ�֧�� %n�Ĵ���
 */
int vsprintf(char* buffer, const char* format, va_list args);
