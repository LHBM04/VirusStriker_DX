#pragma once

#include "Object.h"

class GameObject : public Object {
public:
	// Object��(��) ���� ��ӵ�
	void Start() override;
	void OnDestroy() override;
};

