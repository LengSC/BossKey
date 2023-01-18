#pragma once

#include <Windows.h>

#define ASYNC_KEY_DOWN(VK_NONAME) ((GetAsyncKeyState(VK_NONAME) & 0x8000) ? TRUE : FALSE)


enum SelectionState {
	SS_SUCCEED = (INT)0,	/* ѡ��ɹ� */
	SS_CANCELED = (INT)1,	/* �û�ȡ�� */
	SS_FAILED = (INT)-1		/* ѡ��ʧ�� */
};


class WindowSwitching {
public:
	WindowSwitching();

	INT Select();

	BOOL Switch();

	VOID Release();

private:
	HWND m_hControlled;

	BOOL m_showState;
	
private:
	BOOL IsControllable();

};