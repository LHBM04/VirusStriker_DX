#include "D3DManager.h"

HRESULT D3DManager::Initialize() {
#if defined(DEBUG) || defined(_DEBUG) 
    // D3D12 디버그층 활성화
    Microsoft::WRL::ComPtr<ID3D12Debug> debugController;
    if (FAILED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController)))) {
        return E_FAIL;
    }

    debugController->EnableDebugLayer();
#endif
#pragma region 1. DXGI 팩토리 생성
    // DXGI 팩토리 생성
    if (FAILED(CreateDXGIFactory1(IID_PPV_ARGS(&this->m_pFactory)))) {
        return E_FAIL;
    }
#pragma endregion
#pragma region 2. D3D12 디바이스 생성
    // D3D12 디바이스 생성
    if (FAILED(D3D12CreateDevice(
        nullptr, 
        D3D_FEATURE_LEVEL_11_0,
        IID_PPV_ARGS(&this->m_pD3DDevice)))) {
        // 디바이스 생성에 실패하면 CPU에서 그래픽 연산이 가능하게끔 한다.
        Microsoft::WRL::ComPtr<IDXGIAdapter> pWarpAdapter{ nullptr };
        if (SUCCEEDED(this->m_pFactory->EnumWarpAdapter(IID_PPV_ARGS(&pWarpAdapter)))) {
            if (FAILED(D3D12CreateDevice(
                pWarpAdapter.Get(),
                D3D_FEATURE_LEVEL_11_0,
                IID_PPV_ARGS(&this->m_pD3DDevice)))) {
                return E_FAIL;
            }
        }

        return E_FAIL;
    }
#pragma endregion
#pragma region 3. Fence 생성
    this->CreateFence();
#pragma endregion
#pragma region 4. Command Object 생성
    this->CreateCommandObjects();
#pragma endregion
#pragma region 5. Swap Chain 생성
    this->CreateSwapChain();
#pragma endregion
#pragma region 6. Descriptor Heap 생성
    this->CreateDescriptorHeaps();
#pragma endregion
    return S_OK;
}

HRESULT D3DManager::CreateCommandObjects() {
    D3D12_COMMAND_QUEUE_DESC queueDescriptor;
    queueDescriptor.Flags   = D3D12_COMMAND_QUEUE_FLAG_NONE;
    queueDescriptor.Type    = D3D12_COMMAND_LIST_TYPE_DIRECT;

    // Command Queue 생성
    if (FAILED(m_pD3DDevice->CreateCommandQueue(&queueDescriptor, IID_PPV_ARGS(&this->m_pCommandQueue)))) {
        return E_FAIL;
    }

    // Command Allocator 생성
    if (FAILED(this->m_pD3DDevice->CreateCommandAllocator(
        D3D12_COMMAND_LIST_TYPE_DIRECT,
        IID_PPV_ARGS(&this->m_pCommandAllocator)))) {
        return E_FAIL;
    }

    // Command List 생성
    if (FAILED(this->m_pD3DDevice->CreateCommandList(
        0,
        D3D12_COMMAND_LIST_TYPE_DIRECT,
        this->m_pCommandAllocator.Get(),
        nullptr,
        IID_PPV_ARGS(&this->m_pCommandList)))) {
        return E_FAIL;
    }

    // Command List 닫기.
    this->m_pCommandList->Close();
    return S_OK;
}

HRESULT D3DManager::CreateSwapChain() {
    DXGI_SWAP_CHAIN_DESC1 swapChainDescriptor{};
    swapChainDescriptor.BufferCount = this->BUFFERING_COUNT;
    swapChainDescriptor.Width = this->m_windowWidth;
    swapChainDescriptor.Height = this->m_windowHeight;
    swapChainDescriptor.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swapChainDescriptor.SampleDesc.Count = 1; // 멀티 샘플링 사용 안함
    swapChainDescriptor.SampleDesc.Quality = 0; // Quality는 0으로 설정
    swapChainDescriptor.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDescriptor.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    swapChainDescriptor.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

    DXGI_SWAP_CHAIN_FULLSCREEN_DESC fullscreenDesc{};
    fullscreenDesc.Windowed = !this->m_isFullScreen;
    fullscreenDesc.RefreshRate.Numerator = 60U;
    fullscreenDesc.RefreshRate.Denominator = 1U;
    fullscreenDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    fullscreenDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

    Microsoft::WRL::ComPtr<IDXGISwapChain1> pSwapChain1{};
    if (FAILED(this->m_pFactory->CreateSwapChainForHwnd(
        this->m_pCommandQueue.Get(),
        Core::GetWindowHandle(),
        &swapChainDescriptor,
        &fullscreenDesc,
        nullptr,
        &pSwapChain1))) {
        return E_FAIL;
    }

    if (FAILED(pSwapChain1.As(&this->m_pSwapChain))) {
        return E_FAIL;
    }

    return S_OK;
}

HRESULT D3DManager::CreateDescriptorHeaps() {
    D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDescriptor{};
    rtvHeapDescriptor.NumDescriptors = this->BUFFERING_COUNT;
    rtvHeapDescriptor.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
    rtvHeapDescriptor.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
    rtvHeapDescriptor.NodeMask = NULL;

    if (FAILED(this->m_pD3DDevice->CreateDescriptorHeap(&rtvHeapDescriptor, IID_PPV_ARGS(&m_pRTVHeap)))) {
        return E_FAIL;
    }

    D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDescriptor{};
    dsvHeapDescriptor.NumDescriptors = 1;
    dsvHeapDescriptor.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
    dsvHeapDescriptor.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
    dsvHeapDescriptor.NodeMask = NULL;

    if (FAILED(this->m_pD3DDevice->CreateDescriptorHeap(&dsvHeapDescriptor, IID_PPV_ARGS(&m_pDSVHeap)))) {
        return E_FAIL;
    }

    return S_OK;
}

HRESULT D3DManager::CreateFence() {
    // CPU와 GPU의 동기화를 위한 울타리(Fence) 생성
    if (FAILED(m_pD3DDevice->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&this->m_pFence)))) {
        return E_FAIL;
    }

    // GPU의 사양에 따라 Descriptor의 크기가 달라지므로, 이를 저장해둔다.
    this->m_rtvDescriptorSize = this->m_pD3DDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
    this->m_dsvDescriptorSize = this->m_pD3DDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
    this->m_cbvSrvUavDescriptorSize = this->m_pD3DDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

    D3D12_FEATURE_DATA_MULTISAMPLE_QUALITY_LEVELS multisampleQualityLevels{
        .Format = DXGI_FORMAT_R8G8B8A8_UNORM,
        .SampleCount = 4,
        .Flags = D3D12_MULTISAMPLE_QUALITY_LEVELS_FLAG_NONE,
        .NumQualityLevels = 0
    };

    if (FAILED(this->m_pD3DDevice->CheckFeatureSupport(
        D3D12_FEATURE_MULTISAMPLE_QUALITY_LEVELS,
        &multisampleQualityLevels,
        sizeof(multisampleQualityLevels)))) {
        return E_FAIL;
    }

    return S_OK;
}

VOID D3DManager::FixedUpdate(const float _fixedDeltaTime) {
	return VOID();
}

VOID D3DManager::Render() {
    if (FAILED(this->m_pCommandAllocator->Reset())) {
        assert(0);
    }

    if (FAILED(this->m_pCommandList->Reset(
        this->m_pCommandAllocator.Get(), 
        nullptr))) {
        assert(0);
    }

    // 리소스 통지
    CD3DX12_RESOURCE_BARRIER currentResource{ 
        CD3DX12_RESOURCE_BARRIER::Transition(
        this->GetCurrentBackBuffer(),
        D3D12_RESOURCE_STATE_PRESENT,
        D3D12_RESOURCE_STATE_RENDER_TARGET)};
    this->m_pCommandList->ResourceBarrier(
        1, 
        &currentResource);

    // View 및 Scissor Rects 설정.
    // 해당 과정은 Command List가 재설정될 때마다 같이 재설정되어야 함.
    this->m_pCommandList->RSSetViewports(1, &this->m_screenViewport);
    this->m_pCommandList->RSSetScissorRects(1, &this->m_scissorRect);

    // Back Buffer 및 Depth Buffer 클리어.
    this->m_pCommandList->ClearRenderTargetView(
        this->GetCurrentBackBufferView(), 
        DirectX::Colors::LightSteelBlue, 
        NULL, 
        nullptr);
    this->m_pCommandList->ClearDepthStencilView(
        this->GetDepthStencilView(), 
        D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 
        1.0f,
        NULL,
        NULL,
        nullptr);

    // Render Buffer 타겟팅.
    this->m_pCommandList->OMSetRenderTargets(
        1, 
        &this->GetCurrentBackBufferView(), 
        true,
        &this->GetDepthStencilView());

    // 리소스 통지
    CD3DX12_RESOURCE_BARRIER nextResource{
        CD3DX12_RESOURCE_BARRIER::Transition(
        this->GetCurrentBackBuffer(),
        D3D12_RESOURCE_STATE_RENDER_TARGET,
        D3D12_RESOURCE_STATE_PRESENT)};
    this->m_pCommandList->ResourceBarrier(
        1, 
        &nextResource);

    // Command List 닫기.
    if (FAILED(this->m_pCommandList->Close())) {
        assert(0);
    }

    std::array<ID3D12CommandList*, 1> commandsLists{ this->m_pCommandList.Get() };
    this->m_pCommandQueue->ExecuteCommandLists(
        static_cast<UINT>(commandsLists.size()), 
        commandsLists.data());

    if (FAILED(this->m_pSwapChain->Present(NULL, NULL))) {
        assert(0);
    }

    this->m_currentBackBuffer = (this->m_currentBackBuffer + 1) % BUFFERING_COUNT;
    this->FlushCommandQueue();
}

VOID D3DManager::FlushCommandQueue() {
    if (FAILED(this->m_pCommandQueue->Signal(this->m_pFence.Get(), ++this->m_currentFence))) {
        assert(0);
    }

    // 명령 처리 대기.
    if (this->m_pFence->GetCompletedValue() < this->m_currentFence) {
        HANDLE eventHandle = CreateEventExW(nullptr, nullptr, false, EVENT_ALL_ACCESS);
        if (FAILED(this->m_pFence->SetEventOnCompletion(this->m_currentFence, eventHandle))) {
            assert(0);
        }

        WaitForSingleObject(eventHandle, INFINITE);
        CloseHandle(eventHandle);
    }
}

const INT D3DManager::GetWindowPositionX() const
{
    return 0;
}

const INT D3DManager::GetWindowPositionY() const
{
    return 0;
}

const DirectX::XMVECTOR D3DManager::GetWindowPosition() const
{
    return DirectX::XMVECTOR();
}

const INT D3DManager::GetWindowWidth() const
{
    return 0;
}

const INT D3DManager::GetWindowHeight() const
{
    return 0;
}

const BOOL D3DManager::GetFullScreen() const
{
    return 0;
}

const BOOL D3DManager::GetEnabl4XMSAA() const
{
    return 0;
}

ID3D12Resource2* D3DManager::GetCurrentBackBuffer() const
{
    return nullptr;
}

D3D12_CPU_DESCRIPTOR_HANDLE D3DManager::GetCurrentBackBufferView() const
{
    return D3D12_CPU_DESCRIPTOR_HANDLE();
}

D3D12_CPU_DESCRIPTOR_HANDLE D3DManager::GetDepthStencilView() const
{
    return D3D12_CPU_DESCRIPTOR_HANDLE();
}

HRESULT D3DManager::Resize() {
    assert(this->m_pD3DDevice);
    assert(this->m_pSwapChain);
    assert(this->m_pCommandAllocator);

    this->FlushCommandQueue();

    if (FAILED(this->m_pCommandList->Reset(this->m_pCommandAllocator.Get(), nullptr))) {
        assert(0);
    }

    for (int index = 0; index < this->BUFFERING_COUNT; ++index) {
        this->m_pSwapChainBuffer[index].Reset();
    }
    this->m_pDepthStencilBuffer.Reset();

    if (FAILED(this->m_pSwapChain->ResizeBuffers(
        this->BUFFERING_COUNT,
        this->m_windowWidth,
        this->m_windowHeight,
        DXGI_FORMAT_R8G8B8A8_UNORM,
        DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH))) {
        assert(0);
    }

    this->m_currentBackBuffer = 0;

    CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHeapHandle{ this->m_pRTVHeap->GetCPUDescriptorHandleForHeapStart() };
    for (UINT i = 0; i < this->BUFFERING_COUNT; i++) {
        if (FAILED(this->m_pSwapChain->GetBuffer(i, IID_PPV_ARGS(&this->m_pSwapChainBuffer[i])))) {
            return E_FAIL;
        }

        // 렌더 타겟 뷰 생성
        this->m_pD3DDevice->CreateRenderTargetView(this->m_pSwapChainBuffer[i].Get(), nullptr, rtvHeapHandle);
        rtvHeapHandle.Offset(1, this->m_rtvDescriptorSize);
    }

    D3D12_RESOURCE_DESC depthStencilDescriptor{};
    depthStencilDescriptor.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
    depthStencilDescriptor.Alignment = NULL;
    depthStencilDescriptor.Width = this->m_windowWidth;
    depthStencilDescriptor.Height = this->m_windowHeight;
    depthStencilDescriptor.DepthOrArraySize = 1;
    depthStencilDescriptor.MipLevels = 1;
    depthStencilDescriptor.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    depthStencilDescriptor.SampleDesc.Count = m_isEnable4xMsaa ? 4 : 1;
    depthStencilDescriptor.SampleDesc.Quality = this->m_isEnable4xMsaa ? (this->m_4xMsaaQuality - 1) : 0;
    depthStencilDescriptor.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

    D3D12_CLEAR_VALUE optianalClear{};
    optianalClear.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    optianalClear.DepthStencil.Depth = 1.0f;
    optianalClear.DepthStencil.Stencil = NULL;

    CD3DX12_HEAP_PROPERTIES heapProperties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
    if (FAILED(this->m_pD3DDevice->CreateCommittedResource(
        &heapProperties,
        D3D12_HEAP_FLAG_NONE,
        &depthStencilDescriptor,
        D3D12_RESOURCE_STATE_COMMON,
        &optianalClear,
        IID_PPV_ARGS(&this->m_pDepthStencilBuffer)))) {
        return E_FAIL;
    }

    this->m_pD3DDevice->CreateDepthStencilView(
        this->m_pDepthStencilBuffer.Get(),
        nullptr,
        this->GetDepthStencilView()
    );

    this->m_screenViewport.TopLeftX = 0;
    this->m_screenViewport.TopLeftY = 0;
    this->m_screenViewport.Width = this->m_windowWidth;
    this->m_screenViewport.Height = this->m_windowHeight;
    this->m_screenViewport.MinDepth = NULL;
    this->m_screenViewport.MaxDepth = 1.0f;

    this->m_scissorRect = {
        0,
        0,
        static_cast<LONG>(this->m_windowWidth / 2.0f),
        static_cast<LONG>(this->m_windowHeight / 2.0f) };
}
