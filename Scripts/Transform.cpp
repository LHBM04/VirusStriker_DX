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
    const DirectX::XMFLOAT2 _rotation) : Component(_owner) ,
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

DirectX::XMVECTOR Transform::WorldToLocalPosition(const Transform& _transform) {
    if (!_transform.m_parent) {
       return DirectX::
    }

    DirectX::XMMATRIX invParentMatrix = DirectX::XMMatrixInverse(
        nullptr,
        DirectX::XMMatrixTranslation(
            _transform.m_parent->m_position.x,
            _transform.m_parent->m_position.y,
            _transform.m_parent->m_position.z
        ) *
        DirectX::XMMatrixScaling(
            _transform.m_parent->m_scale.x,
            _transform.m_parent->m_scale.y,
            _transform.m_parent->m_scale.z
        ) *
        DirectX::XMMatrixRotationRollPitchYaw(
            _transform.m_parent->m_rotation.x,
            _transform.m_parent->m_rotation.y,
            _transform.m_parent->m_rotation.z));

    DirectX::XMVECTOR worldPosVec = DirectX::XMLoadFloat3(&_transform.m_position);
    DirectX::XMVECTOR localPosVec = DirectX::XMVector3TransformCoord(worldPosVec, invParentMatrix);
    DirectX::XMFLOAT3 localPosition;
    DirectX::XMStoreFloat3(&localPosition, localPosVec);
    return localPosition;
}

DirectX::XMFLOAT3 Transform::LocalToWorldPosition(const Transform& _transform) {
    if (!_transform.m_parent) {
        return _transform.m_position;
    }

    DirectX::XMMATRIX parentMatrix =
        DirectX::XMMatrixTranslation(
            _transform.m_parent->GetPosition().x,
            _transform.m_parent->GetPosition().y,
            _transform.m_parent->GetPosition().z
        ) *
        DirectX::XMMatrixScaling(
            _transform.m_parent->GetScale().x,
            _transform.m_parent->GetScale().y,
            _transform.m_parent->GetScale().z
        ) *
        DirectX::XMMatrixRotationRollPitchYaw(
            _transform.m_parent->GetRotation().x,
            _transform.m_parent->GetRotation().y,
            _transform.m_parent->GetRotation().z);

    DirectX::XMVECTOR localPosVec = DirectX::XMLoadFloat3(&_transform.m_position);
    DirectX::XMVECTOR worldPosVec = DirectX::XMVector3TransformCoord(localPosVec, parentMatrix);
    DirectX::XMFLOAT3 worldPosition;
    DirectX::XMStoreFloat3(&worldPosition, worldPosVec);
    return worldPosition;
}

DirectX::XMFLOAT3 Transform::WorldToLocalScale(const Transform& _transform) {
    if (!_transform.m_parent) {
        return _transform.m_scale;
    }

    DirectX::XMFLOAT3 localScale = {
        _transform.m_x / _transform.GetParent()->GetScale().x,
        _transform.m_x / _transform.GetParent()->GetScale().y,
        _transform.m_x / _transform.GetParent()->GetScale().z
    };

    return localScale;
}

DirectX::XMFLOAT3 Transform::LocalToWorldScale(const Transform& _transform) {
    if (!_transform.m_parent) {
        return _transform.m_scale;
    }

    DirectX::XMFLOAT3 worldScale = {
        _transform.GetScale().x * _transform.m_parent->GetScale().x,
        _transform.GetScale().y * _transform.m_parent->GetScale().y,
        _transform.GetScale().z * _transform.m_parent->GetScale().z
    };

    return worldScale;
}

DirectX::XMFLOAT3 Transform::WorldToLocalRotation(const Transform& _transform) {
    if (!_transform.GetParent()) {
        return _transform.m_rotation;
    }

    DirectX::XMMATRIX invParentRotationMatrix = DirectX::XMMatrixInverse(nullptr,
        DirectX::XMMatrixRotationRollPitchYaw(
            _transform.m_parent->GetRotation().x,
            _transform.m_parent->GetRotation().y,
            _transform.m_parent->GetRotation().z));

    DirectX::XMVECTOR worldRotVec = DirectX::XMLoadFloat3(&_transform.m_rotation);
    DirectX::XMVECTOR localRotVec = DirectX::XMVector3TransformNormal(worldRotVec, invParentRotationMatrix);
    DirectX::XMFLOAT3 localRotation;
    DirectX::XMStoreFloat3(&localRotation, localRotVec);
    return localRotation;
}

DirectX::XMFLOAT3 Transform::LocalToWorldRotation(const Transform& _transform) {
    if (!_transform.m_parent) {
        return _transform.m_rotation;
    }

    DirectX::XMMATRIX parentRotationMatrix =
        DirectX::XMMatrixRotationRollPitchYaw(
            _transform.m_parent->GetRotation().x,
            _transform.m_parent->GetRotation().y,
            _transform.m_parent->GetRotation().z);

    DirectX::XMVECTOR localRotVec = DirectX::XMLoadFloat3(&_transform.m_rotation);
    DirectX::XMVECTOR worldRotVec = DirectX::XMVector3TransformNormal(localRotVec, parentRotationMatrix);
    DirectX::XMFLOAT3 worldRotation;
    DirectX::XMStoreFloat3(&worldRotation, worldRotVec);
    return worldRotation;
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
