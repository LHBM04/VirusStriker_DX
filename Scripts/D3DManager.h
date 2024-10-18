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

class D3DManager final : public Singleton<D3DManager> {
private:
	Microsoft::WRL::ComPtr<IDXGIFactory4>				m_pFactory;
	Microsoft::WRL::ComPtr<ID3D12Device>				m_pD3DDevice;

	static const UINT BUFFERING_COUNT		{ 2 };	// 이중 버퍼링.
	UINT m_currentBackBuffer				{ 0 };	// 현재 백버퍼 인덱스.
	Microsoft::WRL::ComPtr<IDXGISwapChain3>										m_pSwapChain;
	Microsoft::WRL::ComPtr<ID3D12Resource2>										m_pSwapChainBuffer[BUFFERING_COUNT]{};
	Microsoft::WRL::ComPtr<ID3D12Resource2>										m_pDepthStencilBuffer;

	Microsoft::WRL::ComPtr<ID3D12CommandQueue>			m_pCommandQueue;
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>	m_pCommandList; 
	Microsoft::WRL::ComPtr<ID3D12CommandAllocator>		m_pCommandAllocator;

	Microsoft::WRL::ComPtr<ID3D12Fence>                 m_pFence;              // 동기화를 위한 펜스
	UINT m_currentFence{ 0 };

	UINT m_rtvDescriptorSize{ 0 };
	UINT m_dsvDescriptorSize{ 0 };
	UINT m_cbvSrvUavDescriptorSize{ 0 };

	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_pRTVHeap;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> m_pDSVHeap;

	// Set true to use 4X MSAA (?.1.8).  The default is false.
	BOOL      m_isEnable4xMsaa{ false };    // 4X MSAA enabled
	UINT      m_4xMsaaQuality{ 0 };      // quality level of 4X MSAA

	D3D12_VIEWPORT	m_screenViewport;
	D3D12_RECT		m_scissorRect;

private:
	FLOAT m_windowPositionX{ 100 };	// 윈도우 생성 위치(X).
	FLOAT m_windowPositionY{ 100 };	// 윈도우 생성 위치(Y).
	
	FLOAT m_windowWidth{ 1280 };	// 윈도우 가로 너비.
	FLOAT m_windowHeight{ 720 };	// 윈도우 세로 높이.

	BOOL m_isFullScreen{ false };	// 풀스크린 여부.

public:
	HRESULT Initialize();
	HRESULT InitializeD3D();
	HRESULT CreateCommandObjects();
	HRESULT CreateSwapChain();
	HRESULT CreateDescriptorHeaps();
	HRESULT CreateFence();
	VOID FixedUpdate(const float _fixedDeltaTime);
	VOID Render();


	HRESULT Resize();
	VOID FlushCommandQueue();
public:
	const INT				GetWindowPositionX()	const;
	const INT				GetWindowPositionY()	const;
	const DirectX::XMVECTOR	GetWindowPosition()		const;

	const INT GetWindowWidth()	const;
	const INT GetWindowHeight()	const;

	const BOOL GetFullScreen() const;
	const BOOL GetEnabl4XMSAA() const;

	ID3D12Resource2* GetCurrentBackBuffer() const;
	D3D12_CPU_DESCRIPTOR_HANDLE GetCurrentBackBufferView() const;
	D3D12_CPU_DESCRIPTOR_HANDLE GetDepthStencilView() const;

}; 
