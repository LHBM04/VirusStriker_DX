#include "D3DManager.h"

HRESULT D3DManager::Initialize() {
#pragma region 1. DXGI 팩토리 생성
    UINT dxgiFactoryFlags = 0;
    Microsoft::WRL::ComPtr<IDXGIFactory4> pFactory;

    // DXGI 팩토리 생성
    if (FAILED(CreateDXGIFactory1(IID_PPV_ARGS(&pFactory)))) {
        return E_FAIL;
    }
#pragma endregion
#pragma region 2. D3D12 장치 생성
    // D3D12 장치 생성
    if (FAILED(D3D12CreateDevice(
        nullptr, 
        D3D_FEATURE_LEVEL_11_0, 
        IID_PPV_ARGS(&this->m_pD3DDevice)))) {
        return E_FAIL;
    }
#pragma endregion
#pragma region 3. 명령 큐 생성
    D3D12_COMMAND_QUEUE_DESC queueDesc = {};
    queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
    queueDesc.Type  = D3D12_COMMAND_LIST_TYPE_DIRECT;

    // 명령 큐 생성
    if (FAILED(m_pD3DDevice->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&this->m_pCommandQueue)))) {
        return E_FAIL;
    }
#pragma endregion
#pragma region 4. 스왑 체인 생성
    DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
    swapChainDesc.BufferCount           = this->m_bufferCount;                 
    swapChainDesc.Width                 = this->m_windowWidth;
    swapChainDesc.Height                = this->m_windowHeight;
    swapChainDesc.Format                = DXGI_FORMAT_R8G8B8A8_UNORM;
    swapChainDesc.SampleDesc.Count      = 1; // 멀티 샘플링 사용 안함
    swapChainDesc.SampleDesc.Quality    = 0; // Quality는 0으로 설정
    swapChainDesc.BufferUsage           = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.SwapEffect            = DXGI_SWAP_EFFECT_FLIP_DISCARD;
    swapChainDesc.Flags                 = DXGI_SWAP_CHAIN_FLAG_FRAME_LATENCY_WAITABLE_OBJECT;

    DXGI_SWAP_CHAIN_FULLSCREEN_DESC fullscreenDesc = {};
    fullscreenDesc.Windowed                 = static_cast<BOOL>(!this->m_isFullScreen);
    fullscreenDesc.RefreshRate.Numerator    = 60;
    fullscreenDesc.RefreshRate.Denominator  = 1;
    fullscreenDesc.ScanlineOrdering         = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    fullscreenDesc.Scaling                  = DXGI_MODE_SCALING_UNSPECIFIED;
    
    Microsoft::WRL::ComPtr<IDXGISwapChain1> pSwapChain1;
    if (FAILED(pFactory->CreateSwapChainForHwnd(
        this->m_pCommandQueue.Get(),
        Core::GetWindowHandle(),
        &swapChainDesc,
        &fullscreenDesc,
        nullptr,
        &pSwapChain1))) {
        return E_FAIL;
    }

    if (FAILED(pSwapChain1.As(&this->m_pSwapChain))) {
        return E_FAIL;
    }
#pragma endregion
#pragma region 5. Command Allocator 생성
    // Command Allocator 생성
    if (FAILED(this->m_pD3DDevice->CreateCommandAllocator(
        D3D12_COMMAND_LIST_TYPE_DIRECT,
        IID_PPV_ARGS(&this->m_pCommandAllocator)
    ))) {
        return E_FAIL;
    }
#pragma endregion
#pragma region 6. Command List 생성
    // Command List 생성
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
#pragma region 7. 렌더 타겟 뷰 생성
    D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {};
    rtvHeapDesc.NumDescriptors  = this->m_descriptorCount;
    rtvHeapDesc.Type            = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
    rtvHeapDesc.Flags           = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;

    // 렌더 타겟 뷰 생성
    if (FAILED(this->m_pD3DDevice->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&this->m_pRtvHeap)))) {
        return E_FAIL;
    }

    // 백 버퍼 및 렌더 타겟 뷰 생성
    for (UINT i = 0; i < this->m_descriptorCount; i++) {
        Microsoft::WRL::ComPtr<ID3D12Resource> backBuffer;

        if (FAILED(this->m_pSwapChain->GetBuffer(i, IID_PPV_ARGS(&backBuffer)))) {
            return E_FAIL;
        }

        // 렌더 타겟 뷰 생성
        this->m_pD3DDevice->CreateRenderTargetView(backBuffer.Get(), nullptr,
            CD3DX12_CPU_DESCRIPTOR_HANDLE(this->m_pRtvHeap->GetCPUDescriptorHandleForHeapStart(), i, this->m_pD3DDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV)));
    }
#pragma endregion
#pragma region 8. Fence 생성
    // Fence 생성
    if (FAILED(m_pD3DDevice->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&this->m_pFence)))) {
        return E_FAIL;
    }
    m_fenceValue = 1; // 초기화
#pragma endregion
    return S_OK;
}

VOID D3DManager::FixedUpdate(const float _fixedDeltaTime) {
	return VOID();
}

VOID D3DManager::Render() {
    // GPU가 이전 작업이 완료되었는지 확인
    if (FAILED(this->m_pCommandQueue->Signal(this->m_pFence.Get(), this->m_fenceValue))) {
        assert(0);
        return;
    }

    if (this->m_pFence->GetCompletedValue() < m_fenceValue) {
        HANDLE eventHandle = CreateEventEx(nullptr, FALSE, FALSE, EVENT_ALL_ACCESS);
        this->m_pFence->SetEventOnCompletion(this->m_fenceValue, eventHandle);
        WaitForSingleObject(eventHandle, INFINITE);
        CloseHandle(eventHandle);
    }

    m_fenceValue++;

    // 명령 할당자를 재설정
    if (FAILED(this->m_pCommandAllocator->Reset())) {
        assert(0); // 오류 처리 추가
        return;
    }

    // 명령 리스트를 재설정하고 백 버퍼 리소스를 렌더 타겟으로 설정
    if (FAILED(this->m_pCommandList->Reset(m_pCommandAllocator.Get(), nullptr))) {
        assert(0);
        return;
    }

    // 백 버퍼를 가져옵니다
    Microsoft::WRL::ComPtr<ID3D12Resource> pBackBuffer;
    if (FAILED(m_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer)))) {
        assert(0);
        return;
    }

    // 상태를 PRESENT에서 RENDER_TARGET으로 전환
    CD3DX12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(
        pBackBuffer.Get(),
        D3D12_RESOURCE_STATE_PRESENT,
        D3D12_RESOURCE_STATE_RENDER_TARGET
    );
    m_pCommandList->ResourceBarrier(1, &barrier);

    // 렌더 타겟을 설정
    D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = m_pRtvHeap->GetCPUDescriptorHandleForHeapStart();
    m_pCommandList->OMSetRenderTargets(1, &rtvHandle, FALSE, nullptr);

    // 렌더 타겟을 클리어
    const float clearColor[] = { 0.0f, 0.0f, 0.0f, 1.0f }; // 검은 배경
    m_pCommandList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);

    // 카메라 매트릭스를 설정
    DirectX::XMMATRIX matPos    = DirectX::XMMatrixTranslation(-(this->m_windowWidth / 2), (this->m_windowHeight), 0.0f);
    DirectX::XMMATRIX matScale  = DirectX::XMMatrixScaling(m_cameraScale.m128_f32[0], m_cameraScale.m128_f32[1], 1.0f);

    // 카메라 변환 적용 (스케일 -> 위치 -> 뷰)
    DirectX::XMMATRIX matCamera = matScale * matPos * m_matView;

    // 명령 리스트에 적용 (Direct3D12는 SetTransform 같은 고수준 함수가 없습니다)
    // 이 부분은 루트 시그니처와 상수 버퍼를 통해 직접 설정해야 합니다.
    // 예: 카메라 매트릭스를 상수 버퍼에 복사하고 바인딩

    // (이 부분에선 셰이더에 대한 설정이 포함되어야 함)

    // 렌더링 호출 (실제로 그리기)
    // SceneManager::GetInstance().Render();

    // 상태를 RENDER_TARGET에서 PRESENT로 전환합니다
    CD3DX12_RESOURCE_BARRIER presentBarrier = CD3DX12_RESOURCE_BARRIER::Transition(
        pBackBuffer.Get(),
        D3D12_RESOURCE_STATE_RENDER_TARGET,
        D3D12_RESOURCE_STATE_PRESENT
    );
    m_pCommandList->ResourceBarrier(1, &presentBarrier);

    // 명령 리스트를 종료하고 실행
    if (FAILED(m_pCommandList->Close())) {
        assert(0);
        return;
    }

    ID3D12CommandList* ppCommandLists[] = { m_pCommandList.Get() };
    m_pCommandQueue->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);

    // 프레임을 화면에 출력
    m_pSwapChain->Present(1, 0);
}

VOID D3DManager::Release() {
    this->m_pD3DDevice->Release();
    this->m_pCommandQueue->Release();
    this->m_pSwapChain->Release();
    this->m_pRtvHeap->Release();
    this->m_pCommandList->Release();
    this->m_pCommandAllocator->Release();
}
