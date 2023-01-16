#pragma once

#include <Windows.h>

#include "Resource.h"


class DlgSettings {
public:
	static INT_PTR CALLBACK DialogProc(HWND hdlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
};