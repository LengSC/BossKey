#include "App.h"


int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nShowCmd) {
	BossKey mainWindow;

	switch (mainWindow.Create()) {
	case CS_FAILED:
		MessageBox(NULL, L"�����������������ԣ�", L"����", MB_OK | MB_ICONERROR);
		return -1;
		break;

	case CS_EXECUTING:
		MessageBox(NULL, L"���г���ʵ���������У�", L"����", MB_OK | MB_ICONERROR);
		return -1;
		break;

	}

	ShowWindow(mainWindow.Window(), nShowCmd);
	UpdateWindow(mainWindow.Window());

	MSG msg = { 0 };
	while (GetMessage(&msg, NULL, 0, 0) > 0) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return (int)msg.wParam;
}