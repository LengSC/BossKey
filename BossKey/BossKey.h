#pragma once

#include <Windows.h>

#include "SettingsWindow.h"
#include "AboutWindow.h"
#include "WindowSwitching.h"

#include "Resource.h"


/* 窗口创建状态枚举 */
enum CreationState {
	CS_SUCCEED = (INT)0,	/* 创建成功 */
	CS_FAILED = (INT)1,		/* 创建失败 */
	CS_EXECUTING = (INT)-1	/* 已经运行 */
};


/* 程序主窗口类 */
class BossKey {
public:
	static LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

public:
	BossKey();

	PCWSTR ClassName() const;

	HWND Window() const;

    BOOL Create();	/* 创建窗口 */

	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);	/* 窗口消息处理函数 */

private:
	HWND m_hWnd;				/* 主窗口句柄 */

	HANDLE m_hMutexExecuting;	/* 互斥体防止进程多开 */

	ATOM m_idHotkeySwitch;		/* 窗口状态切换热键ID */

	WindowSwitching m_wndSwt;	/* 窗口状态切换封装类 */

private:
	LRESULT OnCreate();

	LRESULT OnClose();

	LRESULT OnDestroy();

	LRESULT OnCtlColorStatic(WPARAM wParam, LPARAM lParam);

	LRESULT OnPaint();

	LRESULT OnHotkey(WPARAM wParam, LPARAM lParam);

	LRESULT OnBtnSelect();

private:
	VOID EditAddStr(HWND hEdit, PCWSTR szAdd);

};