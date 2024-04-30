#include "CRT.h"
#include "IOPort.h"

#include "vesa/console.h"

unsigned short* CRT::m_VideoMemory = (unsigned short *)(0xB8000 + 0xC0000000);
unsigned int CRT::m_CursorX = 0;
unsigned int CRT::m_CursorY = 0;
char* CRT::m_Position = 0;
char* CRT::m_BeginChar = 0;

unsigned int CRT::ROWS = 15;

void CRT::CRTStart(TTy* pTTy)
{
	char ch;
	if ( 0 == CRT::m_BeginChar)
	{
		m_BeginChar = pTTy->t_outq.CurrentChar();
	}
	if ( 0 == m_Position )
	{
		m_Position = m_BeginChar;
	}

	while ( (ch = pTTy->t_outq.GetChar()) != TTy::GET_ERROR )
	{


#ifdef USE_VESA
		video::console::writeOutput(&ch, 1);
#else


		switch (ch)
		{
		case '\n':
			NextLine();
			CRT::m_BeginChar = pTTy->t_outq.CurrentChar();
			m_Position = CRT::m_BeginChar;
			break;

		case 0x15:
			//del_line();
			break;

		case '\b':
			if ( m_Position != CRT::m_BeginChar )
			{
				BackSpace();
				m_Position--;
			}
			break;

		case '\t':
			Tab();
			m_Position++;
			break;

		default:	/* ����Ļ�ϻ�����ͨ�ַ� */
			WriteChar(ch);
			m_Position++;
			break;
		}
#endif
	}
}

void CRT::MoveCursor(unsigned int col, unsigned int row)
{

#ifndef USE_VESA
	if ( (col < 0 || col >= CRT::COLUMNS) || (row < 0 || row >= CRT::ROWS) )
	{
		return;
	}

	/* ������ƫ���� */
	unsigned short cursorPosition = row * CRT::COLUMNS + col;

	/* ѡ�� r14��r15�Ĵ������ֱ�Ϊ���λ�õĸ�8λ�͵�8λ */
	IOPort::OutByte(CRT::VIDEO_ADDR_PORT, 14);
	IOPort::OutByte(CRT::VIDEO_DATA_PORT, cursorPosition >> 8);
	IOPort::OutByte(CRT::VIDEO_ADDR_PORT, 15);
	IOPort::OutByte(CRT::VIDEO_DATA_PORT, cursorPosition & 0xFF);
#endif
}

void CRT::NextLine()
{

#ifndef USE_VESA
	m_CursorX = 0;
	m_CursorY += 1;

	/* ����������� */
	if ( m_CursorY >= CRT::ROWS )
	{
		m_CursorY = 0;
		ClearScreen();
	}
	MoveCursor(m_CursorX, m_CursorY);
#endif
}

void CRT::BackSpace()
{
	
#ifndef USE_VESA
	m_CursorX--;

	/* �ƶ���꣬���Ҫ�ص���һ�еĻ� */
	if ( m_CursorX < 0 )
	{
		m_CursorX = CRT::COLUMNS - 1;
		m_CursorY--;
		if ( m_CursorY < 0 )
		{
			m_CursorY = 0;
		}
	}
	MoveCursor(m_CursorX, m_CursorY);

	/* �ڹ������λ�����Ͽո� */
	m_VideoMemory[m_CursorY * COLUMNS + m_CursorX] = ' ' | CRT::COLOR;
#endif
}

void CRT::Tab()
{
	
#ifndef USE_VESA
	auto oldCursorX = m_CursorX;

	m_CursorX &= 0xFFFFFFF8;	/* ������뵽ǰһ��Tab�߽� */
	m_CursorX += 8;
	// const int TabWidth = 10;
	// m_CursorX -= m_CursorX % TabWidth;
	// m_CursorX += TabWidth;
	if ( m_CursorX >= CRT::COLUMNS )
		NextLine();
	else {
		MoveCursor(m_CursorX, m_CursorY);
	}
#endif
}

void CRT::WriteChar(char ch)
{
#ifndef USE_VESA
	m_VideoMemory[m_CursorY * CRT::COLUMNS + m_CursorX] = (unsigned char) ch | CRT::COLOR;
	m_CursorX++;
	
	if ( m_CursorX >= CRT::COLUMNS )
	{
		NextLine();
	}
	MoveCursor(m_CursorX, m_CursorY);
#endif
}

void CRT::ClearScreen()
{

#ifdef USE_VESA
	// todo
#else

	unsigned int i;

	for ( i = 0; i < COLUMNS * ROWS; i++ )
	{
		m_VideoMemory[i] = (unsigned short)' ' | CRT::COLOR;
	}
#endif
}

