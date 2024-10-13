#pragma once

#include <string>

class Object abstract {
private:
	std::wstring	m_name;
	bool			m_isDetsroy;

public:
	virtual const std::wstring& GetName() const;
	virtual void SetName(const std::wstring _name);
	
	const bool IsDestroy() const;

	static void Destroy(Object& _object);

public:
	virtual void Start() = 0;
	virtual void OnDestroy() = 0;

};