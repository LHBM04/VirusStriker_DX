#pragma once


#include <iostream>
#include <Windows.h>
#include <string>
#include <d3d12.h>
#include <dxgi1_4.h>
#include <d3dx12.h>
#include <wrl.h>
#include <DirectXMath.h>

#pragma comment (lib, "d3d12.lib")
#pragma comment (lib, "winmm.lib")

#include "Singleton.h"
#include "InputManagement.h"

class SystemManager final : public Singleton<SystemManager> {
private:
	Microsoft::WRL::ComPtr<ID3D12Device>				m_pD3DDevice;
	Microsoft::WRL::ComPtr<ID3D12CommandQueue>			m_pCommandQueue;
	Microsoft::WRL::ComPtr<IDXGISwapChain3>				m_pSwapChain;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>		m_pRtvHeap;
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>	m_pCommandList; 
	Microsoft::WRL::ComPtr<ID3D12CommandAllocator>		m_pCommandAllocator;

private:
	HWND m_windowHandle;

	LPCTSTR m_windowClassName	= TEXT("Virus Striker Class");
	LPCTSTR m_windowName		= TEXT("Virus Striker");

	INT m_windowWidth	= 1280;
	INT m_windowHeight	= 720;

	UINT m_bufferCount		= 2;	// 이중 버퍼링
	UINT m_descriptorCount	= 2;	// 몰라 이건 왜 쓰는지

public:
	const LPCTSTR GetWindowClassName() const;
	const LPCTSTR GetWindowName() const;

	const INT GetWindowWidth() const;
	const INT GetWindowHeight() const;

	inline void SetWindowWidth(const int _width) {
		this->m_windowWidth = _width;
	}

	inline void SetWindowHeight(const int _height) {
		this->m_windowHeight = _height;
	}

	inline void SetWindowResolution(const int _width, const int _height) {
		this->SetWindowWidth(_width);
		this->SetWindowWidth(_height);
	}

public:
	HRESULT Initialize(const HWND& _windowHandle);
	VOID Update(const float _deltaTime);
	VOID FixedUpdate(const float _fixedDeltaTime);
	VOID Render();
	VOID Release();

};

