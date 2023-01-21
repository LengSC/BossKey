#pragma once

#include <Windows.h>

#define ASYNC_KEY_DOWN(VK_NONAME) ((GetAsyncKeyState(VK_NONAME) & 0x8000) ? TRUE : FALSE)


enum SelectionState {
	SS_SUCCEED = (INT)0,	/* 选择成功 */
	SS_FAILED = (INT)-1		/* 选择失败 */
};


enum DestructionState {
	DS_SUCCEED = (INT)0,	/* 关闭成功 */
	DS_CLOSED = (INT)1,		/* 已经关闭 */
	DS_FAILED = (INT)-1		/* 关闭失败 */
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