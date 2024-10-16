#include "Component.h"

Component::Component(GameObject* const _owner) {
	this->m_owner = _owner;
}

Component::~Component() {
}

const GameObject* Component::GetOwner() const {
	return this->m_owner;
}

const Transform& Component::GetTransform() const {
	throw std::exception("");
}

const std::wstring& Component::GetName() const {
	return this->m_owner->GetName();
}

void Component::OnStart() {
	return;
}

void Component::OnDestroy(){
	return;
}
