#pragma once

#include <map>
#include <Windows.h>
#include <stdexcept>
#include <DirectXMath.h>

#include "Singleton.h"

/// <summary>
/// Ű �Է� ���¸� �����ϴ� ������.
/// </summary>
enum EInputState : unsigned int {
	NONE,
	DOWN,
	PRESS,
	UP
};

/// <summary>
/// Ű �Է��� �����ϰ� �̸� ó���մϴ�.
/// </summary>
class InputManager final : public Singleton<InputManager>{
private:
	std::map<const int, EInputState>	m_keyState;			
	std::map<const int, EInputState>	m_buttonState;
	bool								m_isPressedKey;
	DirectX::XMFLOAT2					m_mousePosition;

public:
	InputManager();
	~InputManager();

public:
	inline const EInputState GetKeyState(const int _keyCode) const {
		const auto& iter = this->m_keyState.find(_keyCode);
		if (iter == this->m_keyState.end()) {
			throw std::invalid_argument("[Oops!] �ش� Ű�� ã�� ���߽��ϴ�.");
		}

		return iter->second;
	}

	inline const EInputState GetButtonState(const int _buttonCode) const {
		const auto& iter = this->m_buttonState.find(_buttonCode);
		if (iter == this->m_buttonState.end()) {
			throw std::invalid_argument("[Oops!] �ش� ��ư�� ã�� ���߽��ϴ�.");
		}

		return iter->second;
	}

	inline void SetKeyState(const int _keyCode, const EInputState _keyState) {
		this->m_isPressedKey = (_keyState == EInputState::DOWN);
		this->m_keyState[_keyCode] = _keyState;
	}

	inline void SetButtonState(const int _buttonCode, const EInputState _buttonState) {
		this->m_isPressedKey = (_buttonCode == EInputState::DOWN);
		this->m_buttonState[_buttonCode] = _buttonState;
	}

	inline void SetCursorPosition(POINT _cursorPosition) {
		this->m_mousePosition.x = static_cast<float>(_cursorPosition.x);
		this->m_mousePosition.y = static_cast<float>(_cursorPosition.y);
	}

	inline void SetCursorPosition(const int _x, const int _y) {
		this->SetCursorPosition(POINT{ _x, _y });
	}

	void Update();
};

