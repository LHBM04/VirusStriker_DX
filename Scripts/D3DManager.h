#pragma once

#include <array>
#include <cassert>
#include <d3d12.h>
#include <d3dx12.h>
#include <DirectXColors.h>
#include <DirectXMath.h>
#include <dxgi1_4.h>
#include <iostream>
#include <Windows.h>
#include <wrl.h>

#pragma comment (lib, "d3d12.lib")
#pragma comment (lib, "winmm.lib")

#include "Core.h"
#include "Singleton.h"
#include "InputManager.h"

#define SAFE_RELEASE(x) if (x) x->Release()

class D3DManager final : public Singleton<D3DManager> {
private:
	Microsoft::WRL::ComPtr<IDXGIFactory4>				m_pFactory;
	Microsoft::WRL::ComPtr<ID3D12Device>				m_pD3DDevice;

	static const UINT MAX_BACKBUFFER_COUNT{ 2 };	// 이중 버퍼링.
	UINT m_currentBackBuffer	{ 0 };	// 현재 백버퍼 인덱스.
	Microsoft::WRL::ComPtr<IDXGISwapChain3>										m_pSwapChain;
	std::array<Microsoft::WRL::ComPtr<ID3D12Resource2>, MAX_BACKBUFFER_COUNT>	m_pSwapChainBuffer;
	Microsoft::WRL::ComPtr<ID3D12Resource2>										m_pDepthStencilBuffer;

	Microsoft::WRL::ComPtr<ID3D12CommandQueue>			m_pCommandQueue;
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>	m_pCommandList; 
	Microsoft::WRL::ComPtr<ID3D12CommandAllocator>		m_pCommandAllocator;

	Microsoft::WRL::ComPtr<ID3D12Fence>                 m_pFence;              // 동기화를 위한 펜스
	UINT m_currentFence{ 0 };

	UINT m_rtvDescriptorSize = 0;
	UINT m_dsvDescriptorSize = 0;
	UINT m_cbvSrvUavDescriptorSize = 0;

	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_pRTVHeap;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_pDSVHeap;

	// Set true to use 4X MSAA (?.1.8).  The default is false.
	bool      m_4xMsaaState = false;    // 4X MSAA enabled
	UINT      m_4xMsaaQuality = 0;      // quality level of 4X MSAA

	D3D12_VIEWPORT m_screenViewport;
	D3D12_RECT m_scissorRect;

private:
	FLOAT m_windowPositionX = 100;	// 윈도우 생성 위치(X).
	FLOAT m_windowPositionY = 100;	// 윈도우 생성 위치(Y).
	
	FLOAT m_windowWidth	= 1280;		// 윈도우 가로 너비.
	FLOAT m_windowHeight	= 720;		// 윈도우 세로 높이.

	BOOL m_isFullScreen = false;	// 풀스크린 여부.

public:
	HRESULT Initialize();
	void FixedUpdate(const float _fixedDeltaTime);
	void Render();
	void Release();

public:
	inline const int				GetWindowPositionX()	const;
	inline const int				GetWindowPositionY()	const;
	inline const DirectX::XMFLOAT2	GetWindowPosition()		const;

	inline const int GetWindowWidth()	const;
	inline const int GetWindowHeight()	const;

	inline const bool CheckFullScreen() const;

	inline ID3D12Resource2* GetCurrentBackBuffer() const;
	inline const D3D12_CPU_DESCRIPTOR_HANDLE& GetCurrentBackBufferView() const;
	inline const D3D12_CPU_DESCRIPTOR_HANDLE& GetDepthStencilView() const ;
};


inline const int D3DManager::GetWindowPositionX() const {
	return this->m_windowPositionX;
}

inline const int D3DManager::GetWindowPositionY() const {
	return this->m_windowPositionY;
}

inline const DirectX::XMFLOAT2 D3DManager::GetWindowPosition() const {
	return DirectX::XMFLOAT2(static_cast<float>(this->GetWindowPositionX()), 
							 static_cast<float>(this->GetWindowPositionY()));
}

inline const int D3DManager::GetWindowWidth() const {
	return this->m_windowWidth;
}

inline const int D3DManager::GetWindowHeight() const {
	return this->m_windowHeight;
}

inline const bool D3DManager::CheckFullScreen() const {
	return this->m_isFullScreen;
}

inline ID3D12Resource2* D3DManager::GetCurrentBackBuffer() const {
	return this->m_pSwapChainBuffer.at(this->m_currentBackBuffer).Get();
}

inline const D3D12_CPU_DESCRIPTOR_HANDLE& D3DManager::GetCurrentBackBufferView() const {
	return CD3DX12_CPU_DESCRIPTOR_HANDLE(
		this->m_pRTVHeap->GetCPUDescriptorHandleForHeapStart(),
		this->m_currentBackBuffer,
		this->m_rtvDescriptorSize
	);
}

inline const D3D12_CPU_DESCRIPTOR_HANDLE& D3DManager::GetDepthStencilView() const {
	return this->m_pDSVHeap->GetCPUDescriptorHandleForHeapStart();
}
