#include <Windows.h>

#pragma comment (lib, "winmm.lib")

#include "Core.h"

LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);

INT APIENTRY wWinMain(	_In_		HINSTANCE	hInstance,
						_In_opt_	HINSTANCE	hPrevInstance, 
						_In_		LPWSTR		lpCmdLine, 
						_In_		INT			nCmdShow		) {

	if (SUCCEEDED(Core::Initialize(hInstance))) {
#if defined (_DEBUG)
		AllocConsole(); // 디버깅용 터미널.
		FILE* fp;
		freopen_s(&fp, "CON", "r", stdin);
		freopen_s(&fp, "CON", "w", stdout);
		freopen_s(&fp, "CON", "w", stderr);
		_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
		Core::Run(nCmdShow);
	}
    return 0;
}