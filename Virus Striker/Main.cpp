#include <Windows.h>
#include <SDKDDKVer.h>

#pragma comment (lib, "winmm.lib")

#include "SystemManagement.h"
#include "InputManagement.h"

INT		APIENTRY	wWinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPWSTR, _In_ INT);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);

INT APIENTRY wWinMain(	_In_		HINSTANCE	hInstance,
						_In_opt_	HINSTANCE	hPrevInstance, 
						_In_		LPWSTR		lpCmdLine, 
						_In_		INT			nCmdShow		) {
	UNREFERENCED_PARAMETER(hInstance);
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

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
        case WM_KEYDOWN: {
            InputManager::GetInstance().SetKeyState(
                static_cast<const int>(wParam),
                EInputState::DOWN);
        } return 0;
        case WM_KEYUP: {
            InputManager::GetInstance().SetKeyState(
                static_cast<const int>(wParam),
                EInputState::UP);
        } return 0;
        case WM_LBUTTONDOWN:
        case WM_RBUTTONDOWN: {
            InputManager::GetInstance().SetButtonState(
                static_cast<const int>(wParam),
                EInputState::DOWN);
        } return 0;
        case WM_LBUTTONUP:
        case WM_RBUTTONUP: {
            InputManager::GetInstance().SetButtonState(
                static_cast<const int>(wParam),
                EInputState::UP);
        } return 0;
        case WM_MOUSEMOVE: {
            InputManager::GetInstance().SetCursorPosition(
                static_cast<const int>(LOWORD(lParam)),
                static_cast<const int>(HIWORD(lParam)));
        } return 0;
        case WM_DESTROY: {
            PostQuitMessage(0);
        } return 0;
    }
    return DefWindowProc(hWnd, message, wParam, lParam);
}
