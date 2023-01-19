#pragma once

#include <Windows.h>
#include <CommCtrl.h>

#include "Resource.h"

#pragma comment(lib, "comctl32.lib")


class SettingsWindow {
public:
	static INT_PTR CALLBACK SettingsProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);

};