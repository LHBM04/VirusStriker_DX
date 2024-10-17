#pragma once

#include <Windows.h>
#include <string>

class Object abstract {
private:
	/// <summary>
	/// ������Ʈ�� �̸�.
	/// </summary>
	std::wstring	m_name;

	/// <summary>
	/// ������Ʈ�� �ı� ����.
	/// </summary>
	bool			m_isDetsroy;

public:
	Object();
	Object(const std::wstring);
	~Object();

	void operator = (const Object&) = delete;

public:
	/// <summary>
	/// Object�� �̸��� �����ɴϴ�.
	/// </summary>
	/// <returns>Object�� �̸�.</returns>
	virtual inline const std::wstring& GetName() const;

	/// <summary>
	/// Object�� �ı� ���θ� �����ɴϴ�.
	/// </summary>
	/// <returns>Object�� �ı� ����.</returns>
	virtual inline const bool GetDestroy() const;

	/// <summary>
	/// Object�� �̸��� �����մϴ�.
	/// </summary>
	/// <param name="_value">���ο� �̸�.</param>
	virtual inline void SetName(const std::wstring);

	/// <summary>
	/// Object�� �ı� ���θ� �����մϴ�.
	/// </summary>
	/// <param name="_value"�ı� ����.</param>
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
