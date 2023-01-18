#include "WindowSwitching.h"


WindowSwitching::WindowSwitching() {
	m_hControlled = NULL;
	m_showState = TRUE;
}


INT WindowSwitching::Select() {
	POINT pt = { 0 };
	HWND hTmp = NULL;

	while (!ASYNC_KEY_DOWN('S')) {
		if (ASYNC_KEY_DOWN(VK_ESCAPE)) {
			return SS_CANCELED;
		}
	}

	GetCursorPos(&pt);
	hTmp = WindowFromPoint(pt);

	if (IsWindow(hTmp)) {
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