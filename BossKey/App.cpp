#include "App.h"


int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nShowCmd) {
	BossKey win;

	switch (win.Create()) {
	case CREATIONSTATE_FAILED:
		MessageBox(NULL, L"�����������������ԣ�", L"����", MB_OK | MB_ICONERROR);
		return -1;
		break;

	case CREATIONSTATE_EXECUTING:
		MessageBox(NULL, L"���г���ʵ���������У�", L"����", MB_OK | MB_ICONERROR);
		return -1;
		break;

	}

	ShowWindow(win.Window(), nShowCmd);
	UpdateWindow(win.Window());

	MSG msg = { 0 };
	while (GetMessage(&msg, NULL, 0, 0) > 0) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return (int)msg.wParam;
}