#include "InputManagement.h"

InputManager::InputManager() : 
	m_isPressedKey(false), 
	m_mousePosition(DirectX::XMFLOAT2(0.0f, 0.0f)) {
}

InputManager::~InputManager() {
}

void InputManager::Update() {
	for (auto iter = m_keyState.begin(); iter != m_keyState.end(); ++iter) {
		if (iter->second == EInputState::DOWN) {
			iter->second = EInputState::PRESS;
		} else if (iter->second == EInputState::UP) {
			iter->second = EInputState::NONE;
		}
	}
}