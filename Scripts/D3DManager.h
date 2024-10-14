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
	HWND m_windowHandle;	// 윈도우 핸들.

	const LPCTSTR m_windowClassName	= TEXT("Virus Striker Class");	// 윈도우 클래스 이름.
	const LPCTSTR m_windowName		= TEXT("Virus Striker");		// 프로그램 이름.

	INT m_windowWidth	= 1280;		// 윈도우 가로 너비.
	INT m_windowHeight	= 720;		// 윈도우 세로 높이.

	UINT m_bufferCount		= 2;	// 버퍼링 횟수.
	UINT m_descriptorCount	= 2;	// 디스크립터 개수(버퍼링 횟수와 같아야 함.).

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

