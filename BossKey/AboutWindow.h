#pragma once

#include <Windows.h>

#include "Resource.h"


class AboutWindow  {
public:
	static INT_PTR CALLBACK AboutProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);

};