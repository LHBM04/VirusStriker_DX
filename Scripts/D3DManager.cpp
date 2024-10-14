#include "D3DManager.h"

HRESULT SystemManager::Initialize(const HWND& hWnd) {
    this->m_windowHandle = hWnd;

#if defined (_DEBUG)
    AllocConsole(); // ������ �͹̳�.
    FILE* fp;
    freopen_s(&fp, "CON", "r", stdin);
    freopen_s(&fp, "CON", "w", stdout);
    freopen_s(&fp, "CON", "w", stderr);
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

#pragma region 1. DXGI ���丮 ����
    UINT dxgiFactoryFlags = 0;
    Microsoft::WRL::ComPtr<IDXGIFactory4> pFactory;

    // DXGI ���丮 ����
    if (FAILED(CreateDXGIFactory1(IID_PPV_ARGS(&pFactory)))) {
        return E_FAIL;
    }
#pragma endregion
#pragma region 2. D3D12 ��ġ ����
    // D3D12 ��ġ ����
    if (FAILED(D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&this->m_pD3DDevice)))) {
        return E_FAIL;
    }
#pragma endregion
#pragma region 3. ��� ť ����
    D3D12_COMMAND_QUEUE_DESC queueDesc = {};
    queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
    queueDesc.Type  = D3D12_COMMAND_LIST_TYPE_DIRECT;

    // ��� ť ����
    if (FAILED(m_pD3DDevice->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&this->m_pCommandQueue)))) {
        return E_FAIL;
    }
#pragma endregion
#pragma region 4. ���� ü�� ����
    DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
    swapChainDesc.BufferCount           = this->m_bufferCount;                 
    swapChainDesc.Width                 = this->m_windowWidth;
    swapChainDesc.Height                = this->m_windowHeight;
    swapChainDesc.Format                = DXGI_FORMAT_R8G8B8A8_UNORM;
    swapChainDesc.SampleDesc.Count      = 1; // ��Ƽ ���ø� ��� ����
    swapChainDesc.SampleDesc.Quality    = 0; // Quality�� 0���� ����
    swapChainDesc.BufferUsage           = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.SwapEffect            = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    swapChainDesc.Flags                 = DXGI_SWAP_CHAIN_FLAG_FRAME_LATENCY_WAITABLE_OBJECT;

    Microsoft::WRL::ComPtr<IDXGISwapChain1> pSwapChain1;

    HRESULT hr = pFactory->CreateSwapChainForHwnd(
        this->m_pCommandQueue.Get(),
        this->m_windowHandle,
        &swapChainDesc,
        nullptr,
        nullptr,
        &pSwapChain1);

    if (FAILED(pSwapChain1.As(&this->m_pSwapChain))) {
        return E_FAIL;
    }
#pragma endregion
#pragma region 5. Command Allocator ����
    // Command Allocator ����
    if (FAILED(this->m_pD3DDevice->CreateCommandAllocator(
        D3D12_COMMAND_LIST_TYPE_DIRECT,
        IID_PPV_ARGS(&this->m_pCommandAllocator)
    ))) {
        return E_FAIL;
    }
#pragma endregion
#pragma region 6. Command List ����
    // Command List ����
    if (FAILED(this->m_pD3DDevice->CreateCommandList(
        0,
        D3D12_COMMAND_LIST_TYPE_DIRECT,
        this->m_pCommandAllocator.Get(),
        nullptr,
        IID_PPV_ARGS(&this->m_pCommandList)
    ))) {
        return E_FAIL;
    }
#pragma endregion
#pragma region 7. ���� Ÿ�� �� ����
    D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {};
    rtvHeapDesc.NumDescriptors  = this->m_descriptorCount;
    rtvHeapDesc.Type            = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
    rtvHeapDesc.Flags           = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;

    // ���� Ÿ�� �� ����
    if (FAILED(this->m_pD3DDevice->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&this->m_pRtvHeap)))) {
        return E_FAIL;
    }

    // �� ���� �� ���� Ÿ�� �� ����
    for (UINT i = 0; i < this->m_descriptorCount; i++) {
        Microsoft::WRL::ComPtr<ID3D12Resource> backBuffer;

        if (FAILED(this->m_pSwapChain->GetBuffer(i, IID_PPV_ARGS(&backBuffer)))) {
            return E_FAIL;
        }

        // ���� Ÿ�� �� ����
        this->m_pD3DDevice->CreateRenderTargetView(backBuffer.Get(), nullptr,
            CD3DX12_CPU_DESCRIPTOR_HANDLE(this->m_pRtvHeap->GetCPUDescriptorHandleForHeapStart(), i, this->m_pD3DDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV)));
    }
#pragma endregion
    return S_OK;
}

VOID SystemManager::Update(const float _deltaTime) {
    return VOID();
}

VOID SystemManager::FixedUpdate(const float _fixedDeltaTime) {
	return VOID();
}

VOID SystemManager::Render() {
   
}

VOID SystemManager::Release() {
    this->m_pD3DDevice->Release();
    this->m_pCommandQueue->Release();
    this->m_pSwapChain->Release();
    this->m_pRtvHeap->Release();
    this->m_pCommandList->Release();
    this->m_pCommandAllocator->Release();
}
