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

	m_idHotKeySwitch = NULL;
	m_idHotKeyDestroy = NULL;

	m_idHotKeySelect = NULL;
	m_idHotKeyCancel = NULL;

	m_bSelecting = FALSE;
}


PCWSTR BossKey::ClassName() const {
	return L"BossKey";
}


HWND BossKey::Window() const {
	return m_hWnd;
}


INT BossKey::Create() {
	/* ��ֹ���̶࿪ */
	m_hMutexExecuting = CreateMutex(NULL, FALSE, L"BossKeyExecuting");

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

	return (LRESULT)TRUE;
}


LRESULT BossKey::OnCreate() {
	clock_t start = clock();

	/* ��ʾ��ʼ���� */
	HWND hSplash = CreateDialog(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_DIALOG_SPLASH), m_hWnd, SplashWindow::SplashProc);

	/* ע��ȫ���ȼ� */
	m_idHotKeySwitch = GlobalAddAtom(L"BossKeyHotKeySwitchWindow");
	m_idHotKeyDestroy = GlobalAddAtom(L"BossKeyHotKeyDestroyWindow");

	m_idHotKeySelect = GlobalAddAtom(L"BossKeyHotKeySelectWindow");
	m_idHotKeyCancel = GlobalAddAtom(L"BossKeyHotKeyCancelSelection");


	if (!(RegisterHotKey(m_hWnd, m_idHotKeySwitch, MOD_CONTROL | MOD_ALT | MOD_NOREPEAT, 'S')
		&& RegisterHotKey(m_hWnd, m_idHotKeyDestroy, MOD_CONTROL | MOD_ALT | MOD_NOREPEAT, 'D')
		&& RegisterHotKey(m_hWnd, m_idHotKeySelect, MOD_CONTROL | MOD_NOREPEAT, 'S')
		&& RegisterHotKey(m_hWnd, m_idHotKeyCancel, MOD_NOREPEAT, VK_ESCAPE))
		) {
		MessageBox(m_hWnd, L"ϵͳ�ȼ�ע��ʧ�ܣ����ܷ�����ͻ��", L"����", MB_OK | MB_ICONERROR);
		DestroyWindow(m_hWnd);
	}

	/* �������ڿؼ� */
	/* ѡ�񴰿� */
	HWND hBtnSelect = CreateWindowEx(
		NULL,
		L"BUTTON",
		L"ѡ�񴰿�",
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_CENTER | BS_VCENTER,
		10, 10,
		80, 25,
		m_hWnd,
		(HMENU)IDC_MAIN_SELECT,
		GetModuleHandle(NULL),
		NULL
	);

	/* ��־��� */
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

	clock_t td = clock() - start;

	if (td < 2000) {
		Sleep((DWORD)(2000 - td));
	}

	SendMessage(hSplash, WM_CLOSE, NULL, NULL);

	return (LRESULT)FALSE;
}


LRESULT BossKey::OnClose() {
	if (MessageBox(m_hWnd, L"ȷ���˳���", L"BossKey", MB_OKCANCEL | MB_ICONWARNING | MB_DEFBUTTON2) == IDOK) {
		DestroyWindow(m_hWnd);
	}

	return 0;
}


LRESULT BossKey::OnDestroy() {
	/* �رջ������� */
	CloseHandle(m_hMutexExecuting);

	/* ��ע��ȫ���ȼ� */
	UnregisterHotKey(m_hWnd, m_idHotKeySwitch);
	GlobalDeleteAtom(m_idHotKeySwitch);

	UnregisterHotKey(m_hWnd, m_idHotKeyDestroy);
	GlobalDeleteAtom(m_idHotKeyDestroy);

	UnregisterHotKey(m_hWnd, m_idHotKeySelect);
	GlobalDeleteAtom(m_idHotKeySelect);

	UnregisterHotKey(m_hWnd, m_idHotKeyCancel);
	GlobalDeleteAtom(m_idHotKeyCancel);


	/* �ͷŰ󶨵Ĵ��� */
	m_wndSwt.Release();

	PostQuitMessage(0);

	return (LRESULT)FALSE;
}


LRESULT BossKey::OnCtlColorStatic(WPARAM wParam, LPARAM lParam) {
	/* ���þ�̬�ı������ɫ */
	HWND hEditLog = GetDlgItem(m_hWnd, IDC_MAIN_LOGGING);

	HDC hDc = (HDC)wParam;

	if ((HWND)lParam == hEditLog) {
		SetTextColor(hDc, RGB(100, 100, 100));

		SetBkMode(hDc, OPAQUE);
		SetBkColor(hDc, RGB(0, 0, 0));
		
		return (LRESULT)CreateSolidBrush(RGB(0, 0, 0));
	}

	ReleaseDC(hEditLog, hDc);

	return (LRESULT)FALSE;
}


LRESULT BossKey::OnPaint() {
	PAINTSTRUCT ps;
	HDC hDc = BeginPaint(m_hWnd, &ps);

	FillRect(hDc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));

	EndPaint(m_hWnd, &ps);

	return (LRESULT)FALSE;
}


LRESULT BossKey::OnHotkey(WPARAM wParam, LPARAM lParam) {
	/* ��ֹ�ȼ��̵�����������Ŀ�ݼ� */
	HWND hFocusWnd = GetForegroundWindow();
	DWORD dwFocusThreadId = GetWindowThreadProcessId(hFocusWnd, NULL);
	DWORD dwCurrentThreadId = GetCurrentThreadId();

	if (dwFocusThreadId != dwCurrentThreadId) {
		PostMessage(hFocusWnd, WM_KEYDOWN, HIWORD(lParam), 0);
	}

	/* �����ȼ� */
	switch (LOWORD(lParam)) {
	case NULL:
		switch (HIWORD(lParam)) {
		case VK_ESCAPE:
			if (m_bSelecting) {
				EditAddStr(GetDlgItem(m_hWnd, IDC_MAIN_LOGGING), L"[INFO]    ȡ������ѡ��\r\n");
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
					EditAddStr(GetDlgItem(m_hWnd, IDC_MAIN_LOGGING), L"[INFO]    ����ѡ��ɹ�\r\n");
					break;

				case SS_FAILED:
					EditAddStr(GetDlgItem(m_hWnd, IDC_MAIN_LOGGING), L"[INFO]    ����ѡ��ʧ��\r\n");
					break;

				}
				m_bSelecting = FALSE;
				
				if (m_wndSwt.Controlled() == m_hWnd) {
					MessageBox(m_hWnd, L"���ܰ�������ڱ���!", L"����", MB_OK | MB_ICONERROR);
					m_wndSwt.Release();
				}
			}
			break;

		}
		break;

	case (MOD_CONTROL | MOD_ALT):
		switch (HIWORD(lParam)) {
		case 'S':
			if (m_wndSwt.Switch()) {
				EditAddStr(GetDlgItem(m_hWnd, IDC_MAIN_LOGGING), L"[INFO]    �л��󶨴���״̬\r\n");
			}
			else {
				EditAddStr(GetDlgItem(m_hWnd, IDC_MAIN_LOGGING), L"[INFO]    �󶨴����ѹر�\r\n");
			}
			break;

		case 'D':
			switch (m_wndSwt.Destroy()) {
			case DS_SUCCEED:
				EditAddStr(GetDlgItem(m_hWnd, IDC_MAIN_LOGGING), L"[INFO]    �رհ󶨴���\r\n");
				break;

			case DS_CLOSED:
				EditAddStr(GetDlgItem(m_hWnd, IDC_MAIN_LOGGING), L"[INFO]    �󶨴����ѹر�\r\n");
				break;

			case DS_FAILED:
				EditAddStr(GetDlgItem(m_hWnd, IDC_MAIN_LOGGING), L"[INFO]    �رհ󶨴���ʧ��\r\n");
				break;

			}

		}
		break;

	}

	return (LRESULT)FALSE;
}


LRESULT BossKey::OnCommand(WPARAM wParam, LPARAM lParam) {
	switch (LOWORD(wParam)) {
	case ID_FILE_EXIT:
		SendMessage(m_hWnd, WM_CLOSE, NULL, NULL);
		break;

	case ID_FILE_SETTINGS:
		// TODO: ���õĶԻ�������
		DialogBox(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_DIALOG_SETTINGS), m_hWnd, SettingsWindow::SettingsProc);
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

	return (LRESULT)FALSE;
}


LRESULT BossKey::OnBtnSelect() {
	m_bSelecting = TRUE;
	EditAddStr(GetDlgItem(m_hWnd, IDC_MAIN_LOGGING), L"[INFO]    ��ʼѡ�񴰿�\r\n");

	return (LRESULT)FALSE;
}


VOID BossKey::EditAddStr(HWND hEdit, PCWSTR szAdd) {
	int nStrLen = GetWindowTextLength(hEdit);
	SendMessage(hEdit, EM_SETSEL, (WPARAM)nStrLen, (LPARAM)nStrLen);
	SendMessage(hEdit, EM_REPLACESEL, NULL, (LPARAM)szAdd);
}
