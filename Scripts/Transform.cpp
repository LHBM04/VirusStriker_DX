#include "Transform.h"

Transform::Transform(GameObject* _owner) : Component(_owner),
    m_position(0.0f, 0.0f, 0.0f), 
    m_scale(1.0f, 1.0f, 1.0f), 
    m_rotation(0.0f, 0.0f, 0.0f),
    m_parent(nullptr) {

}

Transform::Transform(GameObject* _owner, 
    const DirectX::XMFLOAT3& _position, 
    const DirectX::XMFLOAT3& _scale, 
    const DirectX::XMFLOAT3& _rotation) : Component(_owner),
    m_position(_position), 
    m_scale(_scale),
    m_rotation(_rotation),
    m_parent(nullptr) {

}

Transform::~Transform() {
}

void Transform::Start() {
    return;
}

void Transform::OnDestroy() {
    return;
}

DirectX::XMFLOAT3 Transform::WorldToLocalPosition(const Transform& _transform) {
    if (!_transform.m_parent) {
        return _transform.m_position;
    }

    DirectX::XMMATRIX invParentMatrix = DirectX::XMMatrixInverse(
        nullptr,
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
            _transform.m_parent->GetRotation().z));

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
        _transform.GetScale().x / _transform.GetParent()->GetScale().x,
        _transform.GetScale().y / _transform.GetParent()->GetScale().y,
        _transform.GetScale().z / _transform.GetParent()->GetScale().z
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
