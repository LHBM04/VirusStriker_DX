#include "D3DManager.h"

HRESULT D3DManager::Initialize() {
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
        Microsoft::WRL::ComPtr<IDXGIAdapter> pWarpAdapter;
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
    // CPU와 GPU의 동기화를 위한 울타리(Fence) 생성
    if (FAILED(m_pD3DDevice->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&this->m_pFence)))) {
        return E_FAIL;
    }

    // GPU의 사양에 따라 Descriptor의 크기가 달라지므로, 이를 저장해둔다.
    this->m_rtvDescriptorSize       = this->m_pD3DDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
    this->m_dsvDescriptorSize       = this->m_pD3DDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
    this->m_cbvSrvUavDescriptorSize = this->m_pD3DDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

    D3D12_FEATURE_DATA_MULTISAMPLE_QUALITY_LEVELS multisampleQualityLevels{};
    multisampleQualityLevels.Format              = DXGI_FORMAT_R8G8B8A8_UNORM;
    multisampleQualityLevels.SampleCount         = 4;
    multisampleQualityLevels.Flags               = D3D12_MULTISAMPLE_QUALITY_LEVELS_FLAG_NONE;
    multisampleQualityLevels.NumQualityLevels    = 0;

    if (FAILED(this->m_pD3DDevice->CheckFeatureSupport(
        D3D12_FEATURE_MULTISAMPLE_QUALITY_LEVELS,
        &multisampleQualityLevels,
        sizeof(multisampleQualityLevels)))) {
        return E_FAIL;
    }
#pragma endregion
#pragma region 4. Command Object 생성
    D3D12_COMMAND_QUEUE_DESC queueDescriptor{};
    queueDescriptor.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
    queueDescriptor.Type  = D3D12_COMMAND_LIST_TYPE_DIRECT;

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
#pragma endregion
#pragma region 5. Swap Chain 생성
    DXGI_SWAP_CHAIN_DESC1 swapChainDescriptor{};
    swapChainDescriptor.BufferCount         = this->MAX_BACKBUFFER_COUNT;
    swapChainDescriptor.Width               = this->m_windowWidth;
    swapChainDescriptor.Height              = this->m_windowHeight;
    swapChainDescriptor.Format              = DXGI_FORMAT_R8G8B8A8_UNORM;
    swapChainDescriptor.SampleDesc.Count    = 1; // 멀티 샘플링 사용 안함
    swapChainDescriptor.SampleDesc.Quality  = 0; // Quality는 0으로 설정
    swapChainDescriptor.BufferUsage         = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDescriptor.SwapEffect          = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    swapChainDescriptor.Flags               = DXGI_SWAP_CHAIN_FLAG_FRAME_LATENCY_WAITABLE_OBJECT;

    DXGI_SWAP_CHAIN_FULLSCREEN_DESC fullscreenDesc{};
    fullscreenDesc.Windowed                 = static_cast<BOOL>(!this->m_isFullScreen);
    fullscreenDesc.RefreshRate.Numerator    = 60;
    fullscreenDesc.RefreshRate.Denominator  = 1;
    fullscreenDesc.ScanlineOrdering         = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    fullscreenDesc.Scaling                  = DXGI_MODE_SCALING_UNSPECIFIED;

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
#pragma endregion
#pragma region 6. Descriptor Heap 생성
    D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDescriptor{};
    rtvHeapDescriptor.NumDescriptors    = this->MAX_BACKBUFFER_COUNT;
    rtvHeapDescriptor.Type              = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
    rtvHeapDescriptor.Flags             = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
    rtvHeapDescriptor.NodeMask          = NULL;

    if (FAILED(this->m_pD3DDevice->CreateDescriptorHeap(&rtvHeapDescriptor, IID_PPV_ARGS(&m_pRTVHeap)))) {
        return E_FAIL;
    }

    D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDescriptor{};
    dsvHeapDescriptor.NumDescriptors    = 1;
    dsvHeapDescriptor.Type              = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
    dsvHeapDescriptor.Flags             = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
    dsvHeapDescriptor.NodeMask          = NULL;

    if (FAILED(this->m_pD3DDevice->CreateDescriptorHeap(&dsvHeapDescriptor, IID_PPV_ARGS(&m_pDSVHeap)))) {
        return E_FAIL;
    }
#pragma endregion
    return S_OK;
}

VOID D3DManager::FixedUpdate(const float _fixedDeltaTime) {
	return VOID();
}

VOID D3DManager::Render() {
    if (FAILED(this->m_pCommandAllocator->Reset())) {
        assert(0);
    }

    if (FAILED(this->m_pCommandList->Reset(this->m_pCommandAllocator.Get(), nullptr))) {
        assert(0);
    }

    // 그리기.
    CD3DX12_RESOURCE_BARRIER currentBarrier = CD3DX12_RESOURCE_BARRIER::Transition(
        this->GetCurrentBackBuffer(),
        D3D12_RESOURCE_STATE_PRESENT,
        D3D12_RESOURCE_STATE_RENDER_TARGET);
    this->m_pCommandList->ResourceBarrier(1, &currentBarrier);

    // 뷰 포트랑 시저 포트 설정.
    this->m_pCommandList->RSSetViewports(1, &this->m_screenViewport);
    this->m_pCommandList->RSSetScissorRects(1, &this->m_scissorRect);

    // 백 버퍼랑 깊이 버퍼 초기화.
    this->m_pCommandList->ClearRenderTargetView(this->GetCurrentBackBufferView(), DirectX::Colors::LightSteelBlue, NULL, nullptr);
    this->m_pCommandList->ClearDepthStencilView(this->GetDepthStencilView(), D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.0f, 0, 0, nullptr);

    this->m_pCommandList->OMSetRenderTargets(1, &this->GetCurrentBackBufferView(), true, &this->GetDepthStencilView());

    // 그림 교체
    CD3DX12_RESOURCE_BARRIER nextBarrier = CD3DX12_RESOURCE_BARRIER::Transition(
        this->GetCurrentBackBuffer(),
        D3D12_RESOURCE_STATE_RENDER_TARGET,
        D3D12_RESOURCE_STATE_PRESENT);
    this->m_pCommandList->ResourceBarrier(1, &nextBarrier);

    if (FAILED(this->m_pCommandList->Close())) {
        assert(0);
    }

    ID3D12CommandList* commandsLists[]{ this->m_pCommandList.Get() };
    this->m_pCommandQueue->ExecuteCommandLists(_countof(commandsLists), commandsLists);

    if (FAILED(this->m_pSwapChain->Present(0, 0))) {
        assert(0);
    }

    this->m_currentBackBuffer = (this->m_currentBackBuffer + 1) % MAX_BACKBUFFER_COUNT;
    if (FAILED(this->m_pCommandQueue->Signal(this->m_pFence.Get(), this->m_currentFence))) {
        assert(0);
    }

    if (this->m_pFence->GetCompletedValue() < this->m_currentFence) {
        HANDLE eventHandle = CreateEventExW(nullptr, nullptr, false, EVENT_ALL_ACCESS);
        if (FAILED(this->m_pFence->SetEventOnCompletion(this->m_currentFence, eventHandle))) {
            assert(0);
        }

        WaitForSingleObject(eventHandle, INFINITE);
        CloseHandle(eventHandle);
    }
}

VOID D3DManager::Release() {
    //this->m_pD3DDevice->Release();
    //this->m_pCommandQueue->Release();
    //this->m_pSwapChain->Release();
    //this->m_pCommandList->Release();
    //this->m_pCommandAllocator->Release();
}
