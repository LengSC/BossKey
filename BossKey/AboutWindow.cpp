#include "AboutWindow.h"


INT_PTR CALLBACK AboutWindow::AboutProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDC_ABOUT_OK:
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
			break;

		case IDC_ABOUT_REPO:
			ShellExecute(NULL, L"OPEN", L"https://github.com/LengSC/BossKey/", NULL, NULL, SW_MAXIMIZE);
			break;
		}

		break;

	default:
		return (INT_PTR)FALSE;
		break;

	}

	return (INT_PTR)FALSE;
}