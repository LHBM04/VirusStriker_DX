#include "Behavior.h"

Behavior::Behavior(GameObject* _actor) : Component(_actor),
	m_isEnabled(true) {
}

Behavior::Behavior(GameObject* _actor, const bool _isEnable) : Component(_actor),
	m_isEnabled(_isEnable) {
}

Behavior::~Behavior()
{
}
