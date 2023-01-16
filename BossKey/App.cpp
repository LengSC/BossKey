#include "App.h"


int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nShowCmd) {
	BossKey win;

	switch (win.Create()) {
	case CREATIONSTATE_FAILED:
		MessageBox(NULL, L"程序启动错误，请重试！", L"错误", MB_OK | MB_ICONERROR);
		return -1;
		break;

	case CREATIONSTATE_EXECUTING:
		MessageBox(NULL, L"已有程序实例正在运行！", L"错误", MB_OK | MB_ICONERROR);
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