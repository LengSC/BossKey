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
		return CREATIONSTATE_FAILED;
	}
	else if (GetLastError() == ERROR_ALREADY_EXISTS) {
		CloseHandle(m_hMutexExecuting);
		return CREATIONSTATE_EXECUTING;
	}

	WNDCLASSEX wc = { 0 };

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.lpfnWndProc = BossKey::WindowProc;
	wc.hInstance = GetModuleHandle(NULL);
	wc.lpszClassName = ClassName();

	wc.style = CS_NOCLOSE;
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc.hCursor = LoadCursor(GetModuleHandle(NULL), IDC_ARROW);
	wc.hIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON_16));
	wc.hIconSm = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_ICON_16));

	RegisterClassEx(&wc);

	m_hWnd = CreateWindowEx(
		NULL,
		ClassName(), L"BossKey",
		WS_CAPTION | WS_SYSMENU | WS_OVERLAPPED | WS_MINIMIZEBOX,
		CW_USEDEFAULT, CW_USEDEFAULT,
		325, 510,
		NULL, LoadMenu(GetModuleHandle(NULL), MAKEINTRESOURCE(IDR_MENU_MAIN)), GetModuleHandle(NULL),
		this
	);

	return (m_hWnd ? CREATIONSTATE_SUCCEED : CREATIONSTATE_FAILED);
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
		switch (LOWORD(wParam)) {
		case ID_FILE_EXIT:
			SendMessage(m_hWnd, WM_CLOSE, NULL, NULL);
			break;

		case ID_FILE_SETTINGS:
			DialogBox(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_DIALOG_SETTINGS), m_hWnd, DlgSettings::DialogProc);
			break;

		case ID_HELP_ABOUT:
			DialogBox(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_DIALOG_ABOUT), m_hWnd, DlgAbout::DialogProc);
			break;

		case ID_HELP_USAGE:
			// TODO 使用方法
			break;

		}
		break;

	default:
		return DefWindowProc(m_hWnd, uMsg, wParam, lParam);
		break;

	}

	return TRUE;
}


LRESULT BossKey::OnCreate() {
	/* 注册全局热键 */
	m_idHotkeySwitch = GlobalAddAtom(L"HotkeySwitchWindowShowState");
	RegisterHotKey(m_hWnd, m_idHotkeySwitch, MOD_CONTROL | MOD_ALT, 'D');

	/* 用于日志输出 */
	HWND hEditLog = CreateWindowEx(
		WS_EX_CLIENTEDGE,
		L"EDIT",
		NULL,
		WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_HSCROLL | ES_MULTILINE | ES_LEFT | ES_READONLY,
		10, 10,
		200, 400,
		m_hWnd,
		(HMENU)IDC_MAIN_EDIT_LOG,
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

	PostQuitMessage(0);

	return 0;
}


LRESULT BossKey::OnCtlColorStatic(WPARAM wParam, LPARAM lParam) {
	HWND hEditLog = GetDlgItem(m_hWnd, IDC_MAIN_EDIT_LOG);

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
	case (MOD_CONTROL | MOD_ALT):
		switch (HIWORD(lParam)) {
		case 'D':
			// 切换窗口显示状态
			//DEBUG
			EditAddStr(GetDlgItem(m_hWnd, IDC_MAIN_EDIT_LOG), L"114514\r\n");
			break;

		}
		break;

	}

	return 0;
}


VOID BossKey::EditAddStr(HWND hEdit, PCWSTR szAdd) {
	/* 向编辑控件添加字符串 */
	int nLogLen = GetWindowTextLength(hEdit);

	PWSTR szResult = (PWSTR)malloc((nLogLen + lstrlen(szAdd) + 1) * sizeof(WCHAR));

	if (szResult == NULL) {
		return;
	}

	ZeroMemory(szResult, sizeof(szResult));
	GetWindowText(hEdit, szResult, nLogLen + 1);
	lstrcat(&szResult[nLogLen], szAdd);
	SetWindowText(hEdit, szResult);

	free(szResult);

	/* 滚动到最后一行 */
	SCROLLINFO scrInfo = { 0 };

	scrInfo.cbSize = sizeof(SCROLLINFO);
	scrInfo.fMask = SIF_RANGE;

	GetScrollInfo(hEdit, SB_VERT, &scrInfo);

	int nMax = scrInfo.nMax;
	scrInfo.fMask = SIF_POS;
	scrInfo.nPos = nMax;

	SetScrollInfo(hEdit, SB_VERT, &scrInfo, TRUE);
}