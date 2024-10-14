#pragma once

#include <memory>
#include <Windows.h>
#include <DirectXMath.h>
#include <directxtk/Keyboard.h>
#include <directxtk/Mouse.h>

#include "Core.h"
#include "Singleton.h"

/// <summary>
/// Ű �Է��� �����ϰ� �̸� ó���մϴ�.
/// </summary>
class InputManager final : public Singleton<InputManager>{
private:
	/// <summary>
	/// Ű���� �̺�Ʈ �ڵ鷯.
	/// </summary>
	std::unique_ptr<DirectX::Keyboard>	m_keyboardHandle;

	/// <summary>
	/// ���콺 �̺�Ʈ �ڵ鷯.
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
	void Initialize();
	
	/// <summary>
	/// Ű/��ư �Է� �޽����� ���Źް� �̸� ó���մϴ�.
	/// </summary>
	bool ProceedInput(const UINT, const WPARAM, const LPARAM);
	
	/// <summary>
	/// ����.
	/// </summary>
	void Reset();

	/// <summary>
	/// ������Ʈ.
	/// </summary>
	void Update();

public:
	inline const bool GetKey(const DirectX::Keyboard::Keys) const;
	inline const bool GetKeyDown(const DirectX::Keyboard::Keys) const;
	inline const bool GetKeyUp(const DirectX::Keyboard::Keys) const;

	inline const bool GetButton(const BYTE) const;
	inline const bool GetButtonDown(const BYTE) const;
	inline const bool GetButtonUp(const BYTE) const;

	inline DirectX::XMFLOAT2 GetCursorPosition() const;
};

inline const bool InputManager::GetKey(const DirectX::Keyboard::Keys _keyCode) const {
	return this->m_keyboardHandle->GetState().IsKeyDown(_keyCode);
}

inline const bool InputManager::GetKeyDown(const DirectX::Keyboard::Keys _keyCode) const {
	return this->m_keyboardStateTracker.IsKeyPressed(_keyCode);
}

inline const bool InputManager::GetKeyUp(const DirectX::Keyboard::Keys _keyCode) const {
	return this->m_keyboardStateTracker.IsKeyReleased(_keyCode);
}

inline const bool InputManager::GetButton(const BYTE _buttonCode) const {
	switch (_buttonCode) {
		case LEFT_BUTTON: { return this->m_buttonStateTracker.leftButton; }
		case RIGHT_BUTTON: { return this->m_buttonStateTracker.rightButton; }
		case MIDDLE_BUTTON: { return this->m_buttonStateTracker.middleButton; }
		default: { return false; }
	}
}

inline const bool InputManager::GetButtonDown(const BYTE _buttonCode) const {
	switch (_buttonCode) {
		case LEFT_BUTTON: { return this->m_buttonStateTracker.leftButton == DirectX::Mouse::ButtonStateTracker::PRESSED; }
		case RIGHT_BUTTON: { return this->m_buttonStateTracker.rightButton == DirectX::Mouse::ButtonStateTracker::PRESSED; }
		case MIDDLE_BUTTON: { return this->m_buttonStateTracker.middleButton == DirectX::Mouse::ButtonStateTracker::PRESSED; }
		default: { return false; }
	}
}

inline const bool InputManager::GetButtonUp(const BYTE _buttonCode) const {
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
