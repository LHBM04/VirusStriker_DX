#pragma once

#include "Object.h"

class GameObject : public Object {
public:
	// Object��(��) ���� ��ӵ�
	void OnStart() override;
	void OnDestroy() override;
};

