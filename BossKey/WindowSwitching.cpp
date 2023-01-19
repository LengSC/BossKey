#include "WindowSwitching.h"


WindowSwitching::WindowSwitching() {
	m_hControlled = NULL;
	m_showState = TRUE;
}


INT WindowSwitching::Select() {
	POINT pt = { 0 };
	HWND hTmp = NULL;
	
	/* 获取鼠标所指的窗口句柄 */
	GetCursorPos(&pt);
	hTmp = WindowFromPoint(pt);

	/* 判断窗口句柄是否有效 */
	if (IsWindow(hTmp)) {

		/* 与之前绑定窗口是否相同 */
		if (m_hControlled != hTmp) {
			Release();
			m_hControlled = hTmp;
		}

		return SS_SUCCEED;
	}
	else {
		return SS_FAILED;
	}
}


BOOL WindowSwitching::Switch() {
	if (IsControllable()) {
		ShowWindow(m_hControlled, ((m_showState ^= TRUE) ? SW_RESTORE : SW_HIDE));
		return TRUE;
	}
	else {
		return FALSE;
	}
}


INT WindowSwitching::Destroy() {
	if (IsControllable()) {
		HANDLE hProcess;
		DWORD dwProcessId;

		if (GetWindowThreadProcessId(m_hControlled, &dwProcessId) == 0) {
			return DS_FAILED;
		}

		hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwProcessId);

		if (hProcess == NULL) {
			return DS_FAILED;
		}

		if (TerminateProcess(hProcess, 0)) {
			return DS_SUCCEED;
		}
		else {
			return DS_FAILED;
		}
	}
	else {
		return DS_CLOSED;
	}
}


VOID WindowSwitching::Release() {
	if (IsControllable()) {
		ShowWindow(m_hControlled, SW_RESTORE);
	}

	m_hControlled = NULL;
	m_showState = TRUE;
}


BOOL WindowSwitching::IsControllable() {
	return IsWindow(m_hControlled);
}