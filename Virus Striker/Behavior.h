#pragma once

#include "Component.h"
#include "GameObject.h"

class GameObject;
class Behavior abstract : public Component{
private:
	bool m_isEnabled;

public:
	Behavior(GameObject* _actor);
	Behavior(GameObject* _actor, const bool _isEnable);
	~Behavior();

public:
	virtual void Start() = 0;
	virtual void OnEnable() = 0;
	virtual void Update(const float _deltaTime) = 0;
	virtual void FixedUpdate(const float _fixedDeltaTime) = 0;
	virtual void LateUpdate(const float _deltaTime) = 0;
	virtual void OnDisable() = 0;
	virtual void OnDestroy() = 0;

};

