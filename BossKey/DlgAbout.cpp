#include "DlgAbout.h"


INT_PTR CALLBACK DlgAbout::DialogProc(HWND hdlg, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDC_ABOUT_OK:
			EndDialog(hdlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
			break;
		}

		break;

	default:
		return (INT_PTR)FALSE;
		break;

	}

	return (INT_PTR)FALSE;
}