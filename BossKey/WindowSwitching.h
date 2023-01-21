#pragma once

#include <Windows.h>

#define ASYNC_KEY_DOWN(VK_NONAME) ((GetAsyncKeyState(VK_NONAME) & 0x8000) ? TRUE : FALSE)


enum SelectionState {
	SS_SUCCEED = (INT)0,	/* ѡ��ɹ� */
	SS_FAILED = (INT)-1		/* ѡ��ʧ�� */
};


enum DestructionState {
	DS_SUCCEED = (INT)0,	/* �رճɹ� */
	DS_CLOSED = (INT)1,		/* �Ѿ��ر� */
	DS_FAILED = (INT)-1		/* �ر�ʧ�� */
};


class WindowSwitching {
public:
	WindowSwitching();

	HWND Controlled() const;

	INT Select();

	BOOL Switch();

	INT Destroy();

	VOID Release();

private:
	HWND m_hControlled;

	BOOL m_showState;
	
private:
	BOOL IsControllable();

};