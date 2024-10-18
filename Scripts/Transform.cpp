#include "Transform.h"

Transform::Transform(GameObject* _owner) : Component(_owner),
m_position(0.0f, 0.0f, 0.0f),
m_scale(1.0f, 1.0f, 1.0f),
m_rotation(0.0f, 0.0f, 0.0f),
m_parent(nullptr) {
}

Transform::Transform(
    GameObject* const _owner,
    const DirectX::XMFLOAT2 _position,
    const DirectX::XMFLOAT2 _scale,
    const DirectX::XMFLOAT2 _rotation) : Component(_owner),
    m_position(DirectX::XMFLOAT3(_position.x, _position.y, 0.0f)),
    m_scale(DirectX::XMFLOAT3(_scale.x, _scale.y, 0.0f)),
    m_rotation(DirectX::XMFLOAT3(_rotation.x, _rotation.y, 0.0f)),
    m_parent(nullptr) {
}

Transform::Transform(
    GameObject* const _owner,
    const DirectX::XMFLOAT3 _position,
    const DirectX::XMFLOAT3 _scale,
    const DirectX::XMFLOAT3 _rotation) : Component(_owner),
    m_position(_position),
    m_scale(_scale),
    m_rotation(_rotation),
    m_parent(nullptr) {
}

Transform::Transform(
    GameObject* const _owner,
    const DirectX::XMVECTOR _position,
    const DirectX::XMVECTOR _scale,
    const DirectX::XMVECTOR _rotation) : Component(_owner),
    m_parent(nullptr) {
    DirectX::XMStoreFloat3(&this->m_position, _position);
    DirectX::XMStoreFloat3(&this->m_scale, _scale);
    DirectX::XMStoreFloat3(&this->m_rotation, _rotation);
}

Transform::~Transform() {
}

void Transform::OnStart() {
    return;
}

void Transform::OnDestroy() {
    return;
}

const DirectX::XMVECTOR Transform::LocalToWorldScale(const Transform&)
{
    return DirectX::XMVECTOR();
}

const DirectX::XMVECTOR Transform::WorldToLocalScale(const Transform&)
{
    return DirectX::XMVECTOR();
}

const DirectX::XMVECTOR Transform::LocalToWorldPosition(const Transform&)
{
    return DirectX::XMVECTOR();
}

const DirectX::XMVECTOR Transform::WorldToLocalPosition(const Transform&)
{
    return DirectX::XMVECTOR();
}

const DirectX::XMVECTOR Transform::WorldToLocalRotation(const Transform&)
{
    return DirectX::XMVECTOR();
}

const DirectX::XMVECTOR Transform::LocalToWorldRotation(const Transform&)
{
    return DirectX::XMVECTOR();
}

inline const DirectX::XMVECTOR Transform::GetPosition() const
{
    return DirectX::XMVECTOR();
}

inline const FLOAT Transform::GetPositionX() const
{
    return FLOAT();
}

inline const FLOAT Transform::GetPositionY() const
{
    return FLOAT();
}

inline const FLOAT Transform::GetPositionZ() const
{
    return FLOAT();
}

inline const DirectX::XMVECTOR Transform::GetLocalPosition() const
{
    return DirectX::XMVECTOR();
}

inline const FLOAT Transform::GetLocalPositionX() const
{
    return FLOAT();
}

inline const FLOAT Transform::GetLocalPositionY() const
{
    return FLOAT();
}

inline const FLOAT Transform::GetLocalPositionZ() const
{
    return FLOAT();
}

inline const DirectX::XMVECTOR Transform::GetScale() const
{
    return DirectX::XMVECTOR();
}

inline const FLOAT Transform::GetScaleX() const
{
    return FLOAT();
}

inline const FLOAT Transform::GetScaleY() const
{
    return FLOAT();
}

inline const FLOAT Transform::GetScaleZ() const
{
    return FLOAT();
}

inline const DirectX::XMVECTOR Transform::GetLocalScale() const
{
    return DirectX::XMVECTOR();
}

inline const FLOAT Transform::GetLocalScaleX() const
{
    return FLOAT();
}

inline const FLOAT Transform::GetLocalScaleY() const
{
    return FLOAT();
}

inline const FLOAT Transform::GetLocalScaleZ() const
{
    return FLOAT();
}

inline const DirectX::XMVECTOR Transform::GetRotation() const
{
    return DirectX::XMVECTOR();
}

inline const FLOAT Transform::GetRotationX() const
{
    return FLOAT();
}

inline const FLOAT Transform::GetRotationY() const
{
    return FLOAT();
}

inline const FLOAT Transform::GetRotationZ() const
{
    return FLOAT();
}

inline const DirectX::XMVECTOR Transform::GetLocalRotation() const
{
    return DirectX::XMVECTOR();
}

inline const FLOAT Transform::GetLocalRotationX() const
{
    return FLOAT();
}

inline const FLOAT Transform::GetLocalRotationY() const
{
    return FLOAT();
}

inline const FLOAT Transform::GetLocalRotationZ() const
{
    return FLOAT();
}

inline void Transform::SetPosition(const DirectX::XMFLOAT2)
{
}

inline void Transform::SetPosition(const DirectX::XMFLOAT3)
{
}

inline void Transform::SetPosition(const DirectX::XMVECTOR)
{
}

inline void Transform::SetLocalPosition(const DirectX::XMFLOAT2)
{
}

inline void Transform::SetLocalPosition(const DirectX::XMFLOAT3)
{
}

inline void Transform::SetLocalPosition(const DirectX::XMVECTOR)
{
}

inline void Transform::SetScale(const DirectX::XMFLOAT2)
{
}

inline void Transform::SetScale(const DirectX::XMFLOAT3)
{
}

inline void Transform::SetScale(const DirectX::XMVECTOR)
{
}

inline void Transform::SetLocalScale(const DirectX::XMFLOAT2)
{
}

inline void Transform::SetLocalScale(const DirectX::XMFLOAT3)
{
}

inline void Transform::SetLocalScale(const DirectX::XMVECTOR)
{
}

inline void Transform::SetRotation(const DirectX::XMFLOAT2)
{
}

inline void Transform::SetRotation(const DirectX::XMFLOAT3)
{
}

inline void Transform::SetRotation(const DirectX::XMVECTOR)
{
}

inline void Transform::SetLocalRotation(const DirectX::XMFLOAT2)
{
}

inline void Transform::SetLocalRotation(const DirectX::XMFLOAT3)
{
}

inline void Transform::SetLocalRotation(const DirectX::XMVECTOR)
{
}
