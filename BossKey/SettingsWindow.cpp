#include "SettingsWindow.h"


INT_PTR CALLBACK SettingsWindow::SettingsProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
	case WM_INITDIALOG:
		{
			INITCOMMONCONTROLSEX icex = { 0 };
			icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
			icex.dwICC = ICC_HOTKEY_CLASS;
			InitCommonControlsEx(&icex);
		}

		SendMessage(
			GetDlgItem(hDlg, IDC_SETTINGS_HKSWITCH),
			HKM_SETRULES,
			(WPARAM)(HKCOMB_NONE),
			MAKELPARAM(HOTKEYF_CONTROL | HOTKEYF_ALT, 0)
		);

		SendMessage(
			GetDlgItem(hDlg, IDC_SETTINGS_HKSWITCH),
			HKM_SETHOTKEY,
			MAKELPARAM(LOWORD(lParam), 0),
			NULL
		);

		SendMessage(
			GetDlgItem(hDlg, IDC_SETTINGS_HKDESTROY),
			HKM_SETRULES,
			(WPARAM)(HKCOMB_NONE),
			MAKELPARAM(HOTKEYF_CONTROL | HOTKEYF_ALT, 0)
		);

		SendMessage(
			GetDlgItem(hDlg, IDC_SETTINGS_HKDESTROY),
			HKM_SETHOTKEY,
			MAKELPARAM(HIWORD(lParam), 0),
			NULL
		);

		break;

	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDC_SETTINGS_OK:
			// TODO: 点击确认后设置全局热键
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
			break;

		case IDC_SETTINGS_CANCEL:
			EndDialog(hDlg, LOWORD(wParam));
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