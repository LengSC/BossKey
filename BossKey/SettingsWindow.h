#pragma once

#include <Windows.h>

#include "Resource.h"


class SettingsWindow {
public:
	static INT_PTR CALLBACK SettingsProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	static INT_PTR OnInitDialog(HWND hDlg, WPARAM wParam, LPARAM lParam);

	static INT_PTR OnCommand(HWND hDlg, WPARAM wParam, LPARAM lParam);

};