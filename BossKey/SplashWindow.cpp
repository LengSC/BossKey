#include "SplashWindow.h"


INT_PTR CALLBACK SplashWindow::SplashProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
	case WM_CLOSE:
		EndDialog(hDlg, NULL);
		return (INT_PTR)TRUE;
		break;

	default:
		return (INT_PTR)FALSE;
		break;

	}

	return (INT_PTR)FALSE;
}