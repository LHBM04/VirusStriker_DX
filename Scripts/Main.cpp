#include <Windows.h>
#include <iostream>
#include <SDKDDKVer.h>

#pragma comment (lib, "winmm.lib")

#include "D3DManager.h"
#include "InputManager.h"

INT		APIENTRY	wWinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPWSTR, _In_ INT);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);

INT APIENTRY wWinMain(	_In_		HINSTANCE	hInstance,
						_In_opt_	HINSTANCE	hPrevInstance, 
						_In_		LPWSTR		lpCmdLine, 
						_In_		INT			nCmdShow		) {
	UNREFERENCED_PARAMETER(hInstance);
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

    // 윈도우 클래스 생성
    WNDCLASSEXW windowClass;
    windowClass.cbSize = sizeof(WNDCLASSEXW);
    windowClass.style = CS_HREDRAW | CS_VREDRAW;
    windowClass.lpfnWndProc = WndProc;
    windowClass.cbClsExtra = NULL;
    windowClass.cbWndExtra = NULL;
    windowClass.hInstance = hInstance;
    windowClass.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
    windowClass.hCursor = LoadCursor(nullptr, IDC_ARROW);
    windowClass.hbrBackground = CreateSolidBrush(RGB(0, 0, 0));
    windowClass.lpszMenuName = nullptr;
    windowClass.lpszClassName = SystemManager::GetInstance().GetWindowClassName();
    windowClass.hIconSm = LoadIcon(nullptr, IDI_APPLICATION);
    RegisterClassExW(&windowClass);

    HWND windowHandle = CreateWindowEx(
        0,
        SystemManager::GetInstance().GetWindowClassName(),
        SystemManager::GetInstance().GetWindowName(),
        WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        SystemManager::GetInstance().GetWindowWidth(),
        SystemManager::GetInstance().GetWindowHeight(),
        nullptr,
        nullptr,
        windowClass.hInstance,
        nullptr
    );

    if (SUCCEEDED(SystemManager::GetInstance().Initialize(windowHandle))) {
        if (!IsWindowVisible(windowHandle)) {
            ShowWindow(windowHandle, nCmdShow);
            UpdateWindow(windowHandle);
        }
        InputManager::GetInstance().Initialize(windowHandle);
        MSG message = { 0 };
        while (message.message != WM_QUIT) {
            if (PeekMessage(&message, NULL, 0U, 0U, PM_REMOVE)) {
                TranslateMessage(&message);
                DispatchMessage(&message);
            }
            else {
                static DWORD prevTime = timeGetTime();
                //if (SceneManager::GetInstance().IsResetDeltaTime())
                //    prevTime = timeGetTime();

                DWORD curTime = timeGetTime();
                const float deltaTime = (curTime - prevTime) / 1000.0f;
                static float fixedUpdateTime = 1.0f / 50.0f;
                static float fixedDeltaTime = 0.0f;
                fixedDeltaTime += deltaTime;
                if (fixedDeltaTime >= 2.0f) {
                    fixedDeltaTime = 2.0f;
                }
                while (fixedDeltaTime > fixedUpdateTime) {
                    fixedDeltaTime -= fixedUpdateTime;
                    SystemManager::GetInstance().FixedUpdate(fixedUpdateTime);
                }
                SystemManager::GetInstance().Update(deltaTime);
                InputManager::GetInstance().Update();
                SystemManager::GetInstance().Render();
                prevTime = curTime;
            }
        }
    }

    SystemManager::GetInstance().Release();
    UnregisterClass(SystemManager::GetInstance().GetWindowClassName(), windowClass.hInstance);
    return 0;
}

LRESULT WndProc(HWND    hWnd, 
                UINT    message, 
                WPARAM  wParam, 
                LPARAM  lParam) {
    switch (message) {
        case WM_DESTROY: {
            PostQuitMessage(0);
        } return 0;
        default: {
            InputManager::GetInstance().InputProc(message, wParam, lParam);
        } break;
    }
    return DefWindowProc(hWnd, message, wParam, lParam);
}
