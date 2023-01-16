#pragma once

#include <Windows.h>

#include "DlgSettings.h"
#include "DlgAbout.h"

#include "Resource.h"


/* ���ڴ���״̬ö�� */
enum CreationStates {
	CREATIONSTATE_SUCCEED = (INT)0,		/* �����ɹ� */
	CREATIONSTATE_FAILED = (INT)1,		/* ����ʧ�� */
	CREATIONSTATE_EXECUTING = (INT)-1	/* �Ѿ����� */
};


/* ������������ */
class BossKey {
public:
	static LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

public:
	BossKey();

	PCWSTR ClassName() const;

	HWND Window() const;

    BOOL Create();	/* �������� */

	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);	/* ������Ϣ������ */

private:
	HWND m_hWnd;				/* �����ھ�� */

	HANDLE m_hMutexExecuting;	/* �������ֹ���̶࿪ */

	ATOM m_idHotkeySwitch;		/* ϵͳ�ȼ�ID */

private:
	LRESULT OnCreate();

	LRESULT OnClose();

	LRESULT OnDestroy();

	LRESULT OnCtlColorStatic(WPARAM wParam, LPARAM lParam);

	LRESULT OnPaint();

	LRESULT OnHotkey(WPARAM wParam, LPARAM lParam);

private:
	VOID EditAddStr(HWND hEdit, PCWSTR szAdd);

};