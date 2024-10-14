#include "Core.h"

const LPCTSTR Core::m_windowClassName   { TEXT("Virus Striker Class") };
const LPCTSTR Core::m_windowName        { TEXT("Virus Striker") };

HINSTANCE   Core::m_instanceHandle  { nullptr };
WNDCLASSEXW Core::m_windowClass     { };
HWND        Core::m_windowHandle    { nullptr };

HRESULT WINAPI Core::Initialize(const HINSTANCE& _instanceHandle) {
    Core::m_instanceHandle = _instanceHandle;

    // 윈도우 클래스 생성
    Core::m_windowClass.cbSize          = sizeof(WNDCLASSEXW);
    Core::m_windowClass.style           = CS_HREDRAW | CS_VREDRAW;
    Core::m_windowClass.lpfnWndProc     = Core::ProceedMessage;
    Core::m_windowClass.cbClsExtra      = NULL;
    Core::m_windowClass.cbWndExtra      = NULL;
    Core::m_windowClass.hInstance       = Core::m_instanceHandle;
    Core::m_windowClass.hIcon           = LoadIcon(nullptr, IDI_APPLICATION);
    Core::m_windowClass.hCursor         = LoadCursor(nullptr, IDC_ARROW);
    Core::m_windowClass.hbrBackground   = CreateSolidBrush(RGB(0, 0, 0));
    Core::m_windowClass.lpszMenuName    = nullptr;
    Core::m_windowClass.lpszClassName   = Core::m_windowClassName;
    Core::m_windowClass.hIconSm         = LoadIcon(nullptr, IDI_APPLICATION);
    RegisterClassExW(&Core::m_windowClass);

    // 윈도우 핸들 생성.
    Core::m_windowHandle = CreateWindowEx(
        0,
        Core::m_windowClassName,
        Core::m_windowName,
        WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        D3DManager::GetInstance().GetWindowWidth(),
        D3DManager::GetInstance().GetWindowHeight(),
        nullptr,
        nullptr,
        Core::m_windowClass.hInstance,
        nullptr
    );

    // D3DX 디바이스 생성이 실패할 경우
    if (FAILED(D3DManager::GetInstance().Initialize())) {
        return E_FAIL;
    }

    InputManager::GetInstance().Initialize();
    return S_OK;
}

int WINAPI Core::Run(const INT _commandShow) {
    if (!IsWindowVisible(Core::m_windowHandle)) {
        ShowWindow(Core::m_windowHandle, _commandShow);
        UpdateWindow(Core::m_windowHandle);
    }

    MSG message{ 0 };
    while (message.message != WM_QUIT) {
        if (PeekMessage(&message, nullptr, 0U, 0U, PM_REMOVE)) {
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
                D3DManager::GetInstance().FixedUpdate(fixedUpdateTime);
            }
            InputManager::GetInstance().Update();
            D3DManager::GetInstance().Render();
            prevTime = curTime;
        }
    }

    D3DManager::GetInstance().Release();
    UnregisterClass(Core::m_windowClassName, Core::m_windowClass.hInstance);
    return 0;
}

VOID WINAPI Core::Quit() {
    PostQuitMessage(0);
}

LRESULT CALLBACK Core::ProceedMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
        case WM_DESTROY: {
            PostQuitMessage(0);
        } return 0;
        default: {
            InputManager::GetInstance().ProceedInput(message, wParam, lParam);
        } break;
    }
    return DefWindowProc(hWnd, message, wParam, lParam);
}
