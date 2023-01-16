#pragma once

#include <Windows.h>

#include "Resource.h"


class DlgAbout  {
public:
	static INT_PTR CALLBACK DialogProc(HWND hdlg, UINT uMsg, WPARAM wParam, LPARAM lParam);
};