#pragma once

#include <iostream>
#include <memory>
#include <Windows.h>
#include <DirectXMath.h>
#include <directxtk/Keyboard.h>
#include <directxtk/Mouse.h>

#include "Singleton.h"

/// <summary>
/// Ű �Է��� �����ϰ� �̸� ó���մϴ�.
/// </summary>
class InputManager final : public Singleton<InputManager>{
private:
	/// <summary>
	/// Ű���� �ڵ鷯.
	/// </summary>
	std::unique_ptr<DirectX::Keyboard>	m_keyboardHandle;

	/// <summary>
	/// ���콺 �ڵ鷯.
	/// </summary>
	std::unique_ptr<DirectX::Mouse>		m_mouseHandle;

	/// <summary>
	/// Ű���� ���¸� �ǽð����� ���� �� ó���ϴ� �ν��Ͻ�.
	/// </summary>
	DirectX::Keyboard::KeyboardStateTracker m_keyboardStateTracker;

	/// <summary>
	/// ���콺 ��ư ���¸� �ǽð����� ���� �� ó���ϴ� �ν��Ͻ�.
	/// </summary>
	DirectX::Mouse::ButtonStateTracker		m_buttonStateTracker;

public:
	// ���콺 ��ư �ڵ� ����
	static const BYTE LEFT_BUTTON	= 0;
	static const BYTE RIGHT_BUTTON	= 1;
	static const BYTE MIDDLE_BUTTON = 2;

	InputManager();
	~InputManager();

public:
	/// <summary>
	/// �ʱ�ȭ.
	/// </summary>
	VOID Initialize(HWND);
	
	/// <summary>
	/// Ű/��ư �Է� �޽����� ���Źް� �̸� ó���մϴ�.
	/// </summary>
	BOOL InputProc(UINT, WPARAM, LPARAM);
	
	/// <summary>
	/// ����.
	/// </summary>
	VOID Reset();

	/// <summary>
	/// ������Ʈ.
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
		case LEFT_BUTTON: { return this->m_buttonStateTracker.leftButton; }		// ���콺 ���� ��ư.
		case RIGHT_BUTTON: { return this->m_buttonStateTracker.rightButton; }		// ���콺 ������ ��ư.
		case MIDDLE_BUTTON: { return this->m_buttonStateTracker.middleButton; }		// ���콺 ��� ��ư(��).
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
