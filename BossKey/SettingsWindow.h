#pragma once

#include <Windows.h>

#include "Resource.h"


class SettingsWindow {
public:
	static INT_PTR CALLBACK SettingsProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
};