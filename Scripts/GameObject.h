#pragma once

#include "Object.h"

class GameObject : public Object {
public:
	// Object을(를) 통해 상속됨
	void OnStart() override;
	void OnDestroy() override;
};

