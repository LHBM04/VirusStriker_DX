#include "D3DManager.h"

HRESULT D3DManager::Initialize() {
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
    if (FAILED(D3D12CreateDevice(
        nullptr, 
        D3D_FEATURE_LEVEL_11_0, 
        IID_PPV_ARGS(&this->m_pD3DDevice)))) {
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
#pragma region 8. Fence ����
    // Fence ����
    if (FAILED(m_pD3DDevice->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&this->m_pFence)))) {
        return E_FAIL;
    }
    m_fenceValue = 1; // �ʱ�ȭ
#pragma endregion
    return S_OK;
}

VOID D3DManager::FixedUpdate(const float _fixedDeltaTime) {
	return VOID();
}

VOID D3DManager::Render() {
    // GPU�� ���� �۾��� �Ϸ�Ǿ����� Ȯ��
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

    // ��� �Ҵ��ڸ� �缳��
    if (FAILED(this->m_pCommandAllocator->Reset())) {
        assert(0); // ���� ó�� �߰�
        return;
    }

    // ��� ����Ʈ�� �缳���ϰ� �� ���� ���ҽ��� ���� Ÿ������ ����
    if (FAILED(this->m_pCommandList->Reset(m_pCommandAllocator.Get(), nullptr))) {
        assert(0);
        return;
    }

    // �� ���۸� �����ɴϴ�
    Microsoft::WRL::ComPtr<ID3D12Resource> pBackBuffer;
    if (FAILED(m_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer)))) {
        assert(0);
        return;
    }

    // ���¸� PRESENT���� RENDER_TARGET���� ��ȯ
    CD3DX12_RESOURCE_BARRIER barrier = CD3DX12_RESOURCE_BARRIER::Transition(
        pBackBuffer.Get(),
        D3D12_RESOURCE_STATE_PRESENT,
        D3D12_RESOURCE_STATE_RENDER_TARGET
    );
    m_pCommandList->ResourceBarrier(1, &barrier);

    // ���� Ÿ���� ����
    D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = m_pRtvHeap->GetCPUDescriptorHandleForHeapStart();
    m_pCommandList->OMSetRenderTargets(1, &rtvHandle, FALSE, nullptr);

    // ���� Ÿ���� Ŭ����
    const float clearColor[] = { 0.0f, 0.0f, 0.0f, 1.0f }; // ���� ���
    m_pCommandList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);

    // ī�޶� ��Ʈ������ ����
    DirectX::XMMATRIX matPos    = DirectX::XMMatrixTranslation(-(this->m_windowWidth / 2), (this->m_windowHeight), 0.0f);
    DirectX::XMMATRIX matScale  = DirectX::XMMatrixScaling(m_cameraScale.m128_f32[0], m_cameraScale.m128_f32[1], 1.0f);

    // ī�޶� ��ȯ ���� (������ -> ��ġ -> ��)
    DirectX::XMMATRIX matCamera = matScale * matPos * m_matView;

    // ��� ����Ʈ�� ���� (Direct3D12�� SetTransform ���� ����� �Լ��� �����ϴ�)
    // �� �κ��� ��Ʈ �ñ״�ó�� ��� ���۸� ���� ���� �����ؾ� �մϴ�.
    // ��: ī�޶� ��Ʈ������ ��� ���ۿ� �����ϰ� ���ε�

    // (�� �κп��� ���̴��� ���� ������ ���ԵǾ�� ��)

    // ������ ȣ�� (������ �׸���)
    // SceneManager::GetInstance().Render();

    // ���¸� RENDER_TARGET���� PRESENT�� ��ȯ�մϴ�
    CD3DX12_RESOURCE_BARRIER presentBarrier = CD3DX12_RESOURCE_BARRIER::Transition(
        pBackBuffer.Get(),
        D3D12_RESOURCE_STATE_RENDER_TARGET,
        D3D12_RESOURCE_STATE_PRESENT
    );
    m_pCommandList->ResourceBarrier(1, &presentBarrier);

    // ��� ����Ʈ�� �����ϰ� ����
    if (FAILED(m_pCommandList->Close())) {
        assert(0);
        return;
    }

    ID3D12CommandList* ppCommandLists[] = { m_pCommandList.Get() };
    m_pCommandQueue->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);

    // �������� ȭ�鿡 ���
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
