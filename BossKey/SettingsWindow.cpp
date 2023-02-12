#include "SettingsWindow.h"


INT_PTR CALLBACK SettingsWindow::SettingsProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
	case WM_INITDIALOG:
		return OnInitDialog(hDlg, wParam, lParam);
		break;

	case WM_COMMAND:
		return OnCommand(hDlg, wParam, lParam);
		break;

	default:
		return (INT_PTR)FALSE;
		break;

	}

	return (INT_PTR)FALSE;
}


INT_PTR SettingsWindow::OnInitDialog(HWND hDlg, WPARAM wParam, LPARAM lParam) {
	return (INT_PTR)TRUE;
}


INT_PTR SettingsWindow::OnCommand(HWND hDlg, WPARAM wParam, LPARAM lParam) {
	switch (LOWORD(wParam)) {
	case IDC_SETTINGS_OK:
		EndDialog(hDlg, LOWORD(wParam));
		return (INT_PTR)TRUE;
		break;

	case IDC_SETTINGS_CANCEL:
		EndDialog(hDlg, LOWORD(wParam));
		return (INT_PTR)TRUE;
		break;

	}

	return (INT_PTR)TRUE;
}