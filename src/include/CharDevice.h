#ifndef CHAR_DEVICE_H
#define CHAR_DEVICE_H

#include "TTy.h"

class CharDevice
{
public:
	CharDevice();
	virtual ~CharDevice();
	/* 
	 * ����Ϊ�麯���������������overrideʵ���豸
	 * �ض���������������£������к�����Ӧ�����õ���
	 */
	virtual void Open(short dev, int mode) = 0;
	virtual void Close(short dev, int mode) = 0;
	virtual void Read(short dev) = 0;
	virtual void Write(short dev) = 0;
	virtual void SgTTy(short dev, TTy* pTTy) = 0;

public:
	TTy* m_TTy;		/* ָ���ַ��豸TTy�ṹ��ָ�� */
};


class ConsoleDevice : public CharDevice
{
public:
	ConsoleDevice();
	virtual ~ConsoleDevice();
	/* 
	 * Override����CharDevice�е��麯����ʵ��
	 * ������ConsoleDevice�ض����豸�����߼���
	 */
	void Open(short dev, int mode);
	void Close(short dev, int mode);
	void Read(short dev);
	void Write(short dev);
	void SgTTy(short dev, TTy* pTTy);
};

#endif
