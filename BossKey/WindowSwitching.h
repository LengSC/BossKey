#pragma once

#include <Windows.h>

#define ASYNC_KEY_DOWN(VK_NONAME) ((GetAsyncKeyState(VK_NONAME) & 0x8000) ? TRUE : FALSE)


enum SelectionState {
	SS_SUCCEED = (INT)0,	/* 选择成功 */
	SS_CANCELED = (INT)1,	/* 用户取消 */
	SS_FAILED = (INT)-1		/* 选择失败 */
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