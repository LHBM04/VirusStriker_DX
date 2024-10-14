#include <Windows.h>

#pragma comment (lib, "winmm.lib")

#include "Core.h"

LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);

INT APIENTRY wWinMain(	_In_		HINSTANCE	hInstance,
						_In_opt_	HINSTANCE	hPrevInstance, 
						_In_		LPWSTR		lpCmdLine, 
						_In_		INT			nCmdShow		) {
	if (SUCCEEDED(Core::Initialize(hInstance))) {
		Core::Run(nCmdShow);
	}
    return 0;
}