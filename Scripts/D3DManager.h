#pragma once

#include <iostream>
#include <Windows.h>
#include <d3d12.h>
#include <dxgi1_4.h>
#include <d3dx12.h>
#include <wrl.h>
#include <DirectXMath.h>

#pragma comment (lib, "d3d12.lib")
#pragma comment (lib, "winmm.lib")

#include "Core.h"
#include "Singleton.h"
#include "InputManager.h"

class D3DManager final : public Singleton<D3DManager> {
private:
	Microsoft::WRL::ComPtr<ID3D12Device>				m_pD3DDevice;
	Microsoft::WRL::ComPtr<ID3D12CommandQueue>			m_pCommandQueue;
	Microsoft::WRL::ComPtr<IDXGISwapChain3>				m_pSwapChain;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>		m_pRtvHeap;
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>	m_pCommandList; 
	Microsoft::WRL::ComPtr<ID3D12CommandAllocator>		m_pCommandAllocator;

private:
	int m_windowPositionX = 100;	// ������ ���� ��ġ(X).
	int m_windowPositionY = 100;	// ������ ���� ��ġ(Y).
	
	int m_windowWidth	= 1280;		// ������ ���� �ʺ�.
	int m_windowHeight	= 720;		// ������ ���� ����.

	bool m_isFullScreen = false;	// Ǯ��ũ�� ����.

	unsigned int m_bufferCount		= 2;	// ���۸� Ƚ��.
	unsigned int m_descriptorCount	= 2;	// ��ũ���� ����(���۸� Ƚ���� ���ƾ� ��.).

public:
	HRESULT Initialize();
	void Update(const float _deltaTime);
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
