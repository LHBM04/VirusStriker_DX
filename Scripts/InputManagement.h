#pragma once

#include <iostream>
#include <memory>
#include <Windows.h>
#include <DirectXMath.h>
#include <directxtk/Keyboard.h>
#include <directxtk/Mouse.h>

#include "Singleton.h"

/// <summary>
/// 키 입력을 감지하고 이를 처리합니다.
/// </summary>
class InputManager final : public Singleton<InputManager>{
private:
	/// <summary>
	/// 키보드 핸들러.
	/// </summary>
	std::unique_ptr<DirectX::Keyboard>	m_keyboardHandle;

	/// <summary>
	/// 마우스 핸들러.
	/// </summary>
	std::unique_ptr<DirectX::Mouse>		m_mouseHandle;

	/// <summary>
	/// 키보드 상태를 실시간으로 감지 및 처리하는 인스턴스.
	/// </summary>
	DirectX::Keyboard::KeyboardStateTracker m_keyboardStateTracker;

	/// <summary>
	/// 마우스 버튼 상태를 실시간으로 감지 및 처리하는 인스턴스.
	/// </summary>
	DirectX::Mouse::ButtonStateTracker		m_buttonStateTracker;

public:
	// 마우스 버튼 코드 정의
	static const BYTE LEFT_BUTTON	= 0;
	static const BYTE RIGHT_BUTTON	= 1;
	static const BYTE MIDDLE_BUTTON = 2;

	InputManager();
	~InputManager();

public:
	/// <summary>
	/// 초기화.
	/// </summary>
	VOID Initialize(HWND);
	
	/// <summary>
	/// 키/버튼 입력 메시지를 수신받고 이를 처리합니다.
	/// </summary>
	BOOL InputProc(UINT, WPARAM, LPARAM);
	
	/// <summary>
	/// 리셋.
	/// </summary>
	VOID Reset();

	/// <summary>
	/// 업데이트.
	/// </summary>
	VOID Update();

public:
	inline const bool GetKey(DirectX::Keyboard::Keys) const;
	inline const bool GetKeyDown(DirectX::Keyboard::Keys) const;
	inline const bool GetKeyUp(DirectX::Keyboard::Keys) const;

	inline const bool GetButton(BYTE) const;
	inline const bool GetButtonDown(BYTE) const;
	inline const bool GetButtonUp(BYTE) const;

	inline DirectX::XMFLOAT2 GetCursorPosition() const;
};

inline const bool InputManager::GetKey(DirectX::Keyboard::Keys _keyCode) const {
	return this->m_keyboardHandle->GetState().IsKeyDown(_keyCode);
}

inline const bool InputManager::GetKeyDown(DirectX::Keyboard::Keys _keyCode) const {
	return this->m_keyboardStateTracker.IsKeyPressed(_keyCode);
}

inline const bool InputManager::GetKeyUp(DirectX::Keyboard::Keys _keyCode) const {
	return this->m_keyboardStateTracker.IsKeyReleased(_keyCode);
}

inline const bool InputManager::GetButton(BYTE _buttonCode) const {
	switch (_buttonCode) {
		case LEFT_BUTTON: { return this->m_buttonStateTracker.leftButton; }		// 마우스 왼쪽 버튼.
		case RIGHT_BUTTON: { return this->m_buttonStateTracker.rightButton; }		// 마우스 오른쪽 버튼.
		case MIDDLE_BUTTON: { return this->m_buttonStateTracker.middleButton; }		// 마우스 가운데 버튼(휠).
		default: { return false; }
	}
}

inline const bool InputManager::GetButtonDown(BYTE _buttonCode) const {
	switch (_buttonCode) {
		case LEFT_BUTTON: { return this->m_buttonStateTracker.leftButton == DirectX::Mouse::ButtonStateTracker::PRESSED; }
		case RIGHT_BUTTON: { return this->m_buttonStateTracker.rightButton == DirectX::Mouse::ButtonStateTracker::PRESSED; }
		case MIDDLE_BUTTON: { return this->m_buttonStateTracker.middleButton == DirectX::Mouse::ButtonStateTracker::PRESSED; }
		default: { return false; }
	}
}

inline const bool InputManager::GetButtonUp(BYTE _buttonCode) const {
	switch (_buttonCode) {
		case LEFT_BUTTON: { return this->m_buttonStateTracker.leftButton == DirectX::Mouse::ButtonStateTracker::RELEASED; }
		case RIGHT_BUTTON: { return this->m_buttonStateTracker.rightButton == DirectX::Mouse::ButtonStateTracker::RELEASED; }
		case MIDDLE_BUTTON: { return this->m_buttonStateTracker.middleButton == DirectX::Mouse::ButtonStateTracker::RELEASED; }
		default: { return false; }
	}
}

inline DirectX::XMFLOAT2 InputManager::GetCursorPosition() const {
	return DirectX::XMFLOAT2(static_cast<float>(this->m_mouseHandle->GetState().x), 
							 static_cast<float>(this->m_mouseHandle->GetState().y));
}
