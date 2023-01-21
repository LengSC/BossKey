#pragma once

#include <Windows.h>
//#include <CommCtrl.h>

#include "Resource.h"

//#pragma comment(lib, "comctl32.lib")


class SettingsWindow {
public:
	static INT_PTR CALLBACK SettingsProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	static INT_PTR OnInitDialog(HWND hDlg, WPARAM wParam, LPARAM lParam);

	static INT_PTR OnCommand(HWND hDlg, WPARAM wParam, LPARAM lParam);

};