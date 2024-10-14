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
#include "InputManager.h"

class SystemManager final : public Singleton<SystemManager> {
private:
	Microsoft::WRL::ComPtr<ID3D12Device>				m_pD3DDevice;
	Microsoft::WRL::ComPtr<ID3D12CommandQueue>			m_pCommandQueue;
	Microsoft::WRL::ComPtr<IDXGISwapChain3>				m_pSwapChain;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>		m_pRtvHeap;
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>	m_pCommandList; 
	Microsoft::WRL::ComPtr<ID3D12CommandAllocator>		m_pCommandAllocator;

private:
	HWND m_windowHandle;	// ������ �ڵ�.

	const LPCTSTR m_windowClassName	= TEXT("Virus Striker Class");	// ������ Ŭ���� �̸�.
	const LPCTSTR m_windowName		= TEXT("Virus Striker");		// ���α׷� �̸�.

	INT m_windowWidth	= 1280;		// ������ ���� �ʺ�.
	INT m_windowHeight	= 720;		// ������ ���� ����.

	UINT m_bufferCount		= 2;	// ���۸� Ƚ��.
	UINT m_descriptorCount	= 2;	// ��ũ���� ����(���۸� Ƚ���� ���ƾ� ��.).

public:
	inline const LPCTSTR GetWindowClassName() const;
	inline const LPCTSTR GetWindowName() const;

	inline const INT GetWindowWidth() const;
	inline const INT GetWindowHeight() const;

public:
	HRESULT Initialize(const HWND& _windowHandle);
	VOID Update(const float _deltaTime);
	VOID FixedUpdate(const float _fixedDeltaTime);
	VOID Render();
	VOID Release();

};

inline const LPCTSTR SystemManager::GetWindowClassName() const {
	return this->m_windowClassName;
}

inline const LPCTSTR SystemManager::GetWindowName() const {
	return this->m_windowName;
}

inline const INT SystemManager::GetWindowWidth() const {
	return this->m_windowWidth;
}

inline const INT SystemManager::GetWindowHeight() const {
	return this->m_windowHeight;
}

