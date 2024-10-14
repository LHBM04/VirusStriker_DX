#pragma once

#include <cassert>
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

#define SAFE_RELEASE(x) if (x) x->Release()

class D3DManager final : public Singleton<D3DManager> {
private:
	Microsoft::WRL::ComPtr<ID3D12Device>				m_pD3DDevice;
	Microsoft::WRL::ComPtr<ID3D12CommandQueue>			m_pCommandQueue;
	Microsoft::WRL::ComPtr<IDXGISwapChain3>				m_pSwapChain;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap>		m_pRtvHeap;
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList>	m_pCommandList; 
	Microsoft::WRL::ComPtr<ID3D12CommandAllocator>		m_pCommandAllocator;

	D3D12_VIEWPORT                                      m_viewport;            // 뷰포트 설정
	D3D12_RECT                                          m_scissorRect;         // 스키서(Rectangle) 설정

	Microsoft::WRL::ComPtr<ID3D12Fence>                 m_pFence;              // 동기화를 위한 펜스
	HANDLE                                              m_fenceEvent;          // 펜스 이벤트 핸들
	UINT64                                              m_fenceValue;          // 펜스 값

	UINT                                                m_rtvDescriptorSize;   // RTV 디스크립터 크기
	Microsoft::WRL::ComPtr<ID3D12Resource>              m_renderTargets[2];    // 렌더 타겟 백 버퍼

	UINT m_bufferCount = 2;	// 버퍼링 횟수.
	UINT m_descriptorCount = 2;	// 디스크립터 개수(버퍼링 횟수와 같아야 함.).

	// 카메라 관련
	DirectX::XMMATRIX                                   m_matView;             // 뷰 매트릭스
	DirectX::XMMATRIX                                   m_matProj;             // 프로젝션 매트릭스
	DirectX::XMVECTOR                                   m_cameraPos;             // 카메라 위치
	DirectX::XMVECTOR                                   m_cameraScale;           // 카메라 스케일

private:
	int m_windowPositionX = 100;	// 윈도우 생성 위치(X).
	int m_windowPositionY = 100;	// 윈도우 생성 위치(Y).
	
	int m_windowWidth	= 1280;		// 윈도우 가로 너비.
	int m_windowHeight	= 720;		// 윈도우 세로 높이.

	bool m_isFullScreen = false;	// 풀스크린 여부.



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
