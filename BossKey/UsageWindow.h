#pragma once

#include <Windows.h>

#include "Resource.h"


class UsageWindow {
public:
	static INT_PTR CALLBACK UsageProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);

};