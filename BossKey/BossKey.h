#pragma once

#include <Windows.h>

#include "SettingsWindow.h"
#include "AboutWindow.h"
#include "UsageWindow.h"

#include "WindowSwitching.h"

#include "Resource.h"


/* ���ڴ���״̬ö�� */
enum CreationState {
	CS_SUCCEED = (INT)0,	/* �����ɹ� */
	CS_FAILED = (INT)1,		/* ����ʧ�� */
	CS_EXECUTING = (INT)-1	/* �Ѿ����� */
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

	ATOM m_idHotkeySwitch;		/* �л��󶨴���״̬�ȼ�ID */
	ATOM m_idHotkeyDestroy;		/* ���ٰ󶨴����ȼ�ID */

	ATOM m_idHotkeySelect;		/* ѡ�񴰿��ȼ�ID */
	ATOM m_idHotkeyCancel;		/* ȡ��ѡ���ȼ�ID */

	BYTE m_btVKHotkeySwitch;	/* �л��󶨴���״̬�ȼ���������Կ���� */
	BYTE m_btVKHotkeyDestroy;	/* ���ٰ󶨴����ȼ���������Կ���� */

	BYTE m_btHKFHotkeySwitch;	/* �л��󶨴���״̬�ȼ��ļ����η� */
	BYTE m_btHKFHotkeyDestroy;	/* ���ٰ󶨴����ȼ��ļ����η� */

	WindowSwitching m_wndSwt;	/* ����״̬�л���װ�� */

	BOOL m_bSelecting;			/* �Ƿ�����ѡ�񴰿� */

private:
	LRESULT OnCreate();

	LRESULT OnClose();

	LRESULT OnDestroy();

	LRESULT OnCtlColorStatic(WPARAM wParam, LPARAM lParam);

	LRESULT OnPaint();

	LRESULT OnHotkey(WPARAM wParam, LPARAM lParam);

	LRESULT OnCommand(WPARAM wParam, LPARAM lParam);

	LRESULT OnBtnSelect();

private:
	VOID EditAddStr(HWND hEdit, PCWSTR szAdd);

};