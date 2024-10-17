#include "Object.h"

Object::Object() : 
	m_name(TEXT("???")),
	m_isDetsroy(false) {
}

Object::Object(const std::wstring _name) :
	m_name(_name),
	m_isDetsroy(false) {
}

Object::~Object() {
}
