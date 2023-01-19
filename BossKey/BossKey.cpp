#include "BossKey.h"


LRESULT CALLBACK BossKey::WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	BossKey* pThis = NULL;

	if (uMsg == WM_NCCREATE) {
		CREATESTRUCT* pCreate = (CREATESTRUCT*)lParam;
		pThis = (BossKey*)pCreate->lpCreateParams;
		SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)pThis);

		pThis->m_hWnd = hWnd;
	}
	else {
		pThis = (BossKey*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
	}
	
	if (pThis) {
		return pThis->HandleMessage(uMsg, wParam, lParam);
	}
	else {
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
}


BossKey::BossKey() {
	m_hWnd = NULL;

	m_hMutexExecuting = NULL;

	m_idHotkeySwitch = NULL;
	m_idHotkeyDestroy = NULL;

	m_idHotkeySelect = NULL;
	m_idHotkeyCancel = NULL;

	m_btVKHotkeySwitch = 'S';
	m_btVKHotkeyDestroy = 'D';

	m_btHKFHotkeySwitch = HOTKEYF_CONTROL | HOTKEYF_ALT;
	m_btHKFHotkeyDestroy = HOTKEYF_CONTROL | HOTKEYF_ALT;

	m_bSelecting = FALSE;
}


PCWSTR BossKey::ClassName() const {
	return L"BossKey";
}


HWND BossKey::Window() const {
	return m_hWnd;
}


INT BossKey::Create() {
	/* 防止进程多开 */
	m_hMutexExecuting = CreateMutex(NULL, FALSE, L"EXECUTING");

	if (m_hMutexExecuting == NULL) {
		return CS_FAILED;
	}
	else if (GetLastError() == ERROR_ALREADY_EXISTS) {
		CloseHandle(m_hMutexExecuting);
		return CS_EXECUTING;
	}

	WNDCLASSEX wc = { 0 };

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.lpfnWndProc = BossKey::WindowProc;
	wc.hInstance = GetModuleHandle(NULL);
	wc.lpszClassName = ClassName();

	wc.style = CS_NOCLOSE;
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc.hCursor = LoadCursor(GetModuleHandle(NULL), IDC_ARROW);
	wc.hIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON_APP));
	wc.hIconSm = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON_APP));

	if (!RegisterClassEx(&wc)) {
		return CS_FAILED;
	}

	m_hWnd = CreateWindowEx(
		NULL,
		ClassName(), L"BossKey",
		WS_CAPTION | WS_SYSMENU | WS_OVERLAPPED | WS_MINIMIZEBOX,
		CW_USEDEFAULT, CW_USEDEFAULT,
		325, 500,
		NULL, LoadMenu(GetModuleHandle(NULL), MAKEINTRESOURCE(IDR_MENU_MAIN)), GetModuleHandle(NULL),
		this
	);

	return (m_hWnd ? CS_SUCCEED : CS_FAILED);
}


LRESULT BossKey::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
	case WM_CREATE:
		return OnCreate();
		break;

	case WM_CLOSE:
		return OnClose();
		break;

	case WM_DESTROY:
		return OnDestroy();
		break;

	case WM_CTLCOLORSTATIC:
		return OnCtlColorStatic(wParam, lParam);
		break;

	case WM_PAINT:
		return OnPaint();
		break;

	case WM_HOTKEY:
		return OnHotkey(wParam, lParam);
		break;
	
	case WM_COMMAND:
		return OnCommand(wParam, lParam);
		break;

	default:
		return DefWindowProc(m_hWnd, uMsg, wParam, lParam);
		break;

	}

	return TRUE;
}


LRESULT BossKey::OnCreate() {
	/* 注册全局热键 */
	m_idHotkeySwitch = GlobalAddAtom(L"HotkeySwitchWindow");
	m_idHotkeyDestroy = GlobalAddAtom(L"HotkeyDestroyWindow");

	m_idHotkeySelect = GlobalAddAtom(L"HotkeySelectWindow");
	m_idHotkeyCancel = GlobalAddAtom(L"HotkeyCancelSelection");


	if (!(RegisterHotKey(m_hWnd, m_idHotkeySwitch, MOD_CONTROL | MOD_ALT | MOD_NOREPEAT, 'S')
		&& RegisterHotKey(m_hWnd, m_idHotkeyDestroy, MOD_CONTROL | MOD_ALT | MOD_NOREPEAT, 'D')
		&& RegisterHotKey(m_hWnd, m_idHotkeySelect, MOD_CONTROL | MOD_NOREPEAT, 'S')
		&& RegisterHotKey(m_hWnd, m_idHotkeyCancel, MOD_NOREPEAT, VK_ESCAPE))
		) {
		MessageBox(m_hWnd, L"系统热键注册失败，可能发生冲突", L"错误", MB_OK | MB_ICONERROR);
		DestroyWindow(m_hWnd);
	}

	/* 创建窗口控件 */
	/* 选择窗口 */
	HWND hBtnSelect = CreateWindowEx(
		NULL,
		L"BUTTON",
		L"选择窗口",
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_CENTER | BS_VCENTER,
		10, 10,
		80, 25,
		m_hWnd,
		(HMENU)IDC_MAIN_SELECT,
		GetModuleHandle(NULL),
		NULL
	);

	/* 日志输出 */
	HWND hEditLog = CreateWindowEx(
		WS_EX_CLIENTEDGE,
		L"EDIT",
		NULL,
		WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_HSCROLL | ES_MULTILINE | ES_LEFT | ES_READONLY,
		100, 10,
		200, 420,
		m_hWnd,
		(HMENU)IDC_MAIN_LOGGING,
		GetModuleHandle(NULL),
		NULL
	);

	return 0;
}


LRESULT BossKey::OnClose() {
	if (MessageBox(m_hWnd, L"确认退出？", L"BossKey", MB_OKCANCEL | MB_ICONWARNING | MB_DEFBUTTON2) == IDOK) {
		DestroyWindow(m_hWnd);
	}

	return 0;
}


LRESULT BossKey::OnDestroy() {
	/* 关闭互斥体句柄 */
	CloseHandle(m_hMutexExecuting);

	/* 反注册全局热键 */
	UnregisterHotKey(m_hWnd, m_idHotkeySwitch);
	GlobalDeleteAtom(m_idHotkeySwitch);

	UnregisterHotKey(m_hWnd, m_idHotkeyDestroy);
	GlobalDeleteAtom(m_idHotkeyDestroy);

	UnregisterHotKey(m_hWnd, m_idHotkeySelect);
	GlobalDeleteAtom(m_idHotkeySelect);

	UnregisterHotKey(m_hWnd, m_idHotkeyCancel);
	GlobalDeleteAtom(m_idHotkeyCancel);


	/* 释放绑定的窗口 */
	m_wndSwt.Release();

	PostQuitMessage(0);

	return 0;
}


LRESULT BossKey::OnCtlColorStatic(WPARAM wParam, LPARAM lParam) {
	/* 设置静态文本框的颜色 */
	HWND hEditLog = GetDlgItem(m_hWnd, IDC_MAIN_LOGGING);

	HDC hDc = (HDC)wParam;

	if ((HWND)lParam == hEditLog) {
		SetTextColor(hDc, RGB(100, 100, 100));

		SetBkMode(hDc, OPAQUE);
		SetBkColor(hDc, RGB(0, 0, 0));
		
		return (LRESULT)CreateSolidBrush(RGB(0, 0, 0));
	}

	ReleaseDC(hEditLog, hDc);

	return 0;
}


LRESULT BossKey::OnPaint() {
	PAINTSTRUCT ps;
	HDC hDc = BeginPaint(m_hWnd, &ps);

	FillRect(hDc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));

	EndPaint(m_hWnd, &ps);

	return 0;
}


LRESULT BossKey::OnHotkey(WPARAM wParam, LPARAM lParam) {
	switch (LOWORD(lParam)) {
	case NULL:
		switch (HIWORD(lParam)) {
		case VK_ESCAPE:
			if (m_bSelecting) {
				EditAddStr(GetDlgItem(m_hWnd, IDC_MAIN_LOGGING), L"[INFO]    取消窗口选择\r\n");
				m_bSelecting = FALSE;
			}
			break;

		}
		break;

	case MOD_CONTROL:
		switch (HIWORD(lParam)) {
		case 'S':
			if (m_bSelecting) {
				switch (m_wndSwt.Select()) {
				case SS_SUCCEED:
					EditAddStr(GetDlgItem(m_hWnd, IDC_MAIN_LOGGING), L"[INFO]    窗口选择成功\r\n");
					break;

				case SS_FAILED:
					EditAddStr(GetDlgItem(m_hWnd, IDC_MAIN_LOGGING), L"[INFO]    窗口选择失败\r\n");
					break;

				}
				m_bSelecting = FALSE;
			}
			break;

		}
		break;

	case (MOD_CONTROL | MOD_ALT):
		switch (HIWORD(lParam)) {
		case 'S':
			if (m_wndSwt.Switch()) {
				EditAddStr(GetDlgItem(m_hWnd, IDC_MAIN_LOGGING), L"[INFO]    切换绑定窗口状态\r\n");
			}
			else {
				EditAddStr(GetDlgItem(m_hWnd, IDC_MAIN_LOGGING), L"[INFO]    绑定窗口已关闭\r\n");
			}
			break;

		case 'D':
			switch (m_wndSwt.Destroy()) {
			case DS_SUCCEED:
				EditAddStr(GetDlgItem(m_hWnd, IDC_MAIN_LOGGING), L"[INFO]    关闭绑定窗口\r\n");
				break;

			case DS_CLOSED:
				EditAddStr(GetDlgItem(m_hWnd, IDC_MAIN_LOGGING), L"[INFO]    绑定窗口已关闭\r\n");
				break;

			case DS_FAILED:
				EditAddStr(GetDlgItem(m_hWnd, IDC_MAIN_LOGGING), L"[INFO]    关闭绑定窗口失败\r\n");
				break;

			}

		}
		break;

	}

	return 0;
}


LRESULT BossKey::OnCommand(WPARAM wParam, LPARAM lParam) {
	switch (LOWORD(wParam)) {
	case ID_FILE_EXIT:
		SendMessage(m_hWnd, WM_CLOSE, NULL, NULL);
		break;

	case ID_FILE_SETTINGS:
		// TODO: 设置的对话框内容
		DialogBoxParam(
			GetModuleHandle(NULL),
			MAKEINTRESOURCE(IDD_DIALOG_SETTINGS),
			m_hWnd,
			SettingsWindow::SettingsProc,
			MAKELPARAM(MAKEWORD(m_btVKHotkeySwitch, m_btHKFHotkeySwitch), MAKEWORD(m_btVKHotkeyDestroy, m_btHKFHotkeyDestroy))
		);

		break;

	case ID_HELP_ABOUT:
		DialogBox(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_DIALOG_ABOUT), m_hWnd, AboutWindow::AboutProc);
		break;

	case ID_HELP_USAGE:
		DialogBox(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_DIALOG_USAGE), m_hWnd, UsageWindow::UsageProc);
		break;

	case IDC_MAIN_SELECT:
		OnBtnSelect();
		break;

	}

	return 0;
}


LRESULT BossKey::OnBtnSelect() {
	m_bSelecting = TRUE;
	EditAddStr(GetDlgItem(m_hWnd, IDC_MAIN_LOGGING), L"[INFO]    开始选择窗口\r\n");

	return 0;
}


VOID BossKey::EditAddStr(HWND hEdit, PCWSTR szAdd) {
	int nStrLen = GetWindowTextLength(hEdit);
	SendMessage(hEdit, EM_SETSEL, (WPARAM)nStrLen, (LPARAM)nStrLen);
	SendMessage(hEdit, EM_REPLACESEL, NULL, (LPARAM)szAdd);
}