#pragma once

#include <string>

class Object abstract {
private:
	static const std::wstring DEFAULT_OBJECT_NAME;

	std::wstring	m_name{ DEFAULT_OBJECT_NAME };
	bool			m_isDetsroy{ false };

public:
	Object();
	~Object();

public:
	virtual const std::wstring& GetName() const;
	virtual void SetName(const std::wstring _name);
	
	const bool IsDestroy() const;

	static void Destroy(Object& _object);

	void operator = (const Object&) = delete;

public:
	virtual void Start() = 0;
	virtual void OnDestroy() = 0;

};