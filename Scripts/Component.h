#pragma once

#include <string>

#include "Object.h"
#include "GameObject.h"

class GameObject;
class Transform;

/// <summary>
/// Game Object�� ���� �� Ư¡�� �����ϰ� �����մϴ�.
/// </summary>
class Component abstract : public Object {
private:
	/// <summary>
	/// Component�� ����.
	/// </summary>
	GameObject* m_owner;

public:
	Component() = delete;
	Component(GameObject* const);
	~Component();

public:
	const GameObject* GetOwner() const;
	const std::wstring& GetName() const override;
	const Transform& GetTransform() const;

public:
	virtual void OnStart() = 0;
	virtual void OnDestroy() = 0;

};

