#pragma once

#include <Windows.h>

#include "D3DManager.h"

class Core {
private:
	static const LPCTSTR m_windowClassName;
	static const LPCTSTR m_windowName;

	static HINSTANCE	m_instanceHandle;
	static WNDCLASSEXW	m_windowClass;
	static HWND			m_windowHandle;

public:
	static HRESULT	WINAPI		Initialize(const HINSTANCE& _instanceHandle);
	static int		WINAPI		Run(const INT _commandShow);
	static void		WINAPI		Quit();
	static LRESULT	CALLBACK	ProceedMessage(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

public:
	inline static const LPCTSTR& GetWindowClassName();
	inline static const LPCTSTR& GetWindowName();

	inline static const HINSTANCE& GetInstanceHandle();
	inline static const WNDCLASSEXW& GetWindowClass();
	inline static const HWND& GetWindowHandle();

};

inline const LPCTSTR& Core::GetWindowClassName() {
	return Core::m_windowClassName;
}

inline const LPCTSTR& Core::GetWindowName() {
	return Core::m_windowName;
}

inline const HINSTANCE& Core::GetInstanceHandle() {
	return Core::m_instanceHandle;
}

inline const WNDCLASSEXW& Core::GetWindowClass() {
	return Core::m_windowClass;
}

inline const HWND& Core::GetWindowHandle() {
	return Core::m_windowHandle;
}
