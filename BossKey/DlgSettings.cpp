#include "DlgSettings.h"


INT_PTR CALLBACK DlgSettings::DialogProc(HWND hdlg, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDC_SETTINGS_OK:
		case IDC_SETTINGS_CANCEL:
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