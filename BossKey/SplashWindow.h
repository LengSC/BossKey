#pragma once

#include <Windows.h>

#include "Resource.h"


class SplashWindow {
public:
	static INT_PTR CALLBACK SplashProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);

};