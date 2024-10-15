#pragma once

#include <string>

#include "Object.h"
#include "GameObject.h"

class GameObject;
class Transform;

class Component abstract : public Object {
private:
	GameObject* m_owner;

public:
	Component() = delete;
	Component(GameObject* _owner);
	~Component();

	

public:
	const GameObject* GetOwner() const;
	const Transform& GetTransform() const;
	const std::wstring& GetName() const override;

public:
	virtual void Start() = 0;
	virtual void OnDestroy() = 0;

};

