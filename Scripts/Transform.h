#pragma once

#include <DirectXMath.h>
#include <Windows.h>
#include <wrl.h>
#include <vector>
#include <memory>
#include <iostream>
#include <optional>
#include <stdexcept>

#include "Component.h"
#include "GameObject.h"

#pragma comment (lib, "winmm.lib")

class GameObject;

/// <summary>
/// 
/// </summary>
class Transform : public Component {
private:
	DirectX::XMFLOAT3 m_position;
	DirectX::XMFLOAT3 m_scale;
	DirectX::XMFLOAT3 m_rotation;

	std::shared_ptr<Transform>				m_parent;
	std::vector<std::shared_ptr<Transform>>	m_children;

public:
	Transform(GameObject* _owner);
	Transform(GameObject* _owner, 
		const DirectX::XMFLOAT3& _position, 
		const DirectX::XMFLOAT3& _scale, 
		const DirectX::XMFLOAT3& _rotation);
	~Transform();

public:
	void Start() override;
	void OnDestroy() override;

public:
	/// <summary>
	/// 월드 좌표 -> 로컬 좌표로 변환합니다.
	/// </summary>
	/// <param name="_transform">변환할 Transform.</param>
	/// <returns></returns>
	static DirectX::XMFLOAT3 WorldToLocalPosition(const Transform& _transform);
	static DirectX::XMFLOAT3 LocalToWorldPosition(const Transform& _transform);

	static DirectX::XMFLOAT3 WorldToLocalScale(const Transform& _transform);
	static DirectX::XMFLOAT3 LocalToWorldScale(const Transform& _transform);

	static DirectX::XMFLOAT3 WorldToLocalRotation(const Transform& _transform);
	static DirectX::XMFLOAT3 LocalToWorldRotation(const Transform& _transform);

public:
	inline const DirectX::XMFLOAT3 GetPosition() const;
	inline const DirectX::XMFLOAT3 GetScale() const;
	inline const DirectX::XMFLOAT3 GetRotation() const;

	inline void SetPosition(const DirectX::XMFLOAT3& _position);
	inline void SetScale(const DirectX::XMFLOAT3& _scale);
	inline void SetRotation(const DirectX::XMFLOAT3& _rotation);

	inline const DirectX::XMFLOAT3 GetLocalPosition() const;
	inline const DirectX::XMFLOAT3 GetLocalScale() const;
	inline const DirectX::XMFLOAT3 GetLocalRotation() const;

	inline void SetLocalPosition(const DirectX::XMFLOAT3& _position);
	inline void SetLocalScale(const DirectX::XMFLOAT3& _scale);
	inline void SetLocalRotation(const DirectX::XMFLOAT3& _rotation);

public:
	inline const std::shared_ptr<Transform>& GetParent() const;
	inline const std::shared_ptr<Transform>& GetChild(const size_t _index) const;
	inline const std::vector<std::shared_ptr<Transform>>& GetChildren() const;

};

inline const DirectX::XMFLOAT3 Transform::GetPosition() const {
	return this->m_position;
}

inline const DirectX::XMFLOAT3 Transform::GetScale() const {
	return this->m_scale;
}

inline const DirectX::XMFLOAT3 Transform::GetRotation() const {
	return this->m_rotation;
}

inline void Transform::SetPosition(const DirectX::XMFLOAT3& _position) {
	this->m_position = _position;
}

inline void Transform::SetScale(const DirectX::XMFLOAT3& _scale) {
	this->m_scale = _scale;
}

inline void Transform::SetRotation(const DirectX::XMFLOAT3& _rotation) {
	this->m_rotation = _rotation;
}

inline const DirectX::XMFLOAT3 Transform::GetLocalPosition() const {
	throw std::runtime_error("");
}

inline const DirectX::XMFLOAT3 Transform::GetLocalScale() const {
	throw std::runtime_error("");
}

inline const DirectX::XMFLOAT3 Transform::GetLocalRotation() const {
	throw std::runtime_error("");
}

inline void Transform::SetLocalPosition(const DirectX::XMFLOAT3& _position) {
	throw std::runtime_error("");
}

inline void Transform::SetLocalScale(const DirectX::XMFLOAT3& _scale) {
	throw std::runtime_error("");
}

inline void Transform::SetLocalRotation(const DirectX::XMFLOAT3& _rotation) {
	throw std::runtime_error("");
}

inline const std::shared_ptr<Transform>& Transform::GetParent() const {
	return this->m_parent;
}

inline const std::shared_ptr<Transform>& Transform::GetChild(const size_t _index) const {
	return this->m_children[_index];
}

inline const std::vector<std::shared_ptr<Transform>>& Transform::GetChildren() const {
	return this->m_children;
}