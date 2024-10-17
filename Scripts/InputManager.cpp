#include "InputManager.h"

InputManager::InputManager() : 
	m_keyboardHandle(std::make_unique<DirectX::Keyboard>()), 
	m_mouseHandle(std::make_unique<DirectX::Mouse>()) {
}

InputManager::~InputManager() {
	
}

void InputManager::Initialize() {
	this->m_mouseHandle->SetWindow(Core::GetWindowHandle());
}

bool InputManager::ProceedInput(const UINT message, const WPARAM wParam, const LPARAM lParam) {
	switch (message) {
		case WM_ACTIVATE:
		case WM_ACTIVATEAPP: {
			this->m_keyboardHandle->ProcessMessage(message, wParam, lParam);
			this->m_mouseHandle->ProcessMessage(message, wParam, lParam);
		} break;
		case WM_MOUSEMOVE:
		case WM_LBUTTONDOWN:
		case WM_LBUTTONUP:
		case WM_RBUTTONDOWN:
		case WM_RBUTTONUP:
		case WM_MBUTTONDOWN:
		case WM_MBUTTONUP:
		case WM_MOUSEWHEEL:
		case WM_MOUSEHOVER: {
			this->m_mouseHandle->ProcessMessage(message, wParam, lParam);
		} break;
		case WM_KEYDOWN:
		case WM_KEYUP:
		case WM_SYSKEYDOWN:
		case WM_SYSKEYUP: {
			this->m_keyboardHandle->ProcessMessage(message, wParam, lParam);
		} break;
		default: {
		} return false;
	}
	return true;
}

void InputManager::Update() {
	this->m_keyboardStateTracker.Update(this->m_keyboardHandle->GetState());
	this->m_buttonStateTracker.Update(this->m_mouseHandle->GetState());
}

void InputManager::Reset() {
	this->m_mouseHandle->ResetScrollWheelValue();
	this->m_keyboardHandle->Reset();

	this->m_keyboardStateTracker.Reset();
	this->m_buttonStateTracker.Reset();
}
