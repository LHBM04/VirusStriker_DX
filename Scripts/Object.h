#pragma once

#include <Windows.h>
#include <string>

class Object abstract {
private:
	/// <summary>
	/// 오브젝트의 이름.
	/// </summary>
	std::wstring	m_name;

	/// <summary>
	/// 오브젝트의 파괴 여부.
	/// </summary>
	bool			m_isDetsroy;

public:
	Object();
	Object(const std::wstring);
	~Object();

	void operator = (const Object&) = delete;

public:
	/// <summary>
	/// Object의 이름을 가져옵니다.
	/// </summary>
	/// <returns>Object의 이름.</returns>
	virtual inline const std::wstring& GetName() const;

	/// <summary>
	/// Object의 파괴 여부를 가져옵니다.
	/// </summary>
	/// <returns>Object의 파괴 여부.</returns>
	virtual inline const bool GetDestroy() const;

	/// <summary>
	/// Object의 이름을 설정합니다.
	/// </summary>
	/// <param name="_value">새로운 이름.</param>
	virtual inline void SetName(const std::wstring);

	/// <summary>
	/// Object를 파괴 여부를 설정합니다.
	/// </summary>
	/// <param name="_value"파괴 여부.</param>
	virtual inline void SetDestroy(const bool);

public:
	virtual void OnStart() = 0;
	virtual void OnDestroy() = 0;

};

inline const std::wstring& Object::GetName() const {
	return this->m_name;
}

inline const bool Object::GetDestroy() const {
	return this->m_isDetsroy;
}

inline void Object::SetName(const std::wstring _value) {
	this->m_name = _value;
}

inline void Object::SetDestroy(const bool _value) {
	this->m_isDetsroy = _value;
}
