#pragma once

#include "Object.h"

class GameObject : public Object {
public:
	// Object을(를) 통해 상속됨
	void Start() override;
	void OnDestroy() override;
};

