#pragma once

#include <string>

#include "Object.h"
#include "GameObject.h"

class GameObject;
class Transform;

/// <summary>
/// Game Object의 정보 및 특징을 저장하고 관리합니다.
/// </summary>
class Component abstract : public Object {
private:
	/// <summary>
	/// Component의 주인.
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

