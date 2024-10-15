#include "Object.h"

Object::Object()
{
}

Object::~Object()
{
}

const std::wstring& Object::GetName() const {
	return this->m_name;
}

void Object::SetName(const std::wstring _name) {
	this->m_name = _name;
}

const bool Object::IsDestroy() const {
	return this->m_isDetsroy;
}

void Object::Destroy(Object& _object) {
	_object.m_isDetsroy = false;
}
