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
	Transform(GameObject* const);
	Transform(
		GameObject* const,
		const DirectX::XMFLOAT2,
		const DirectX::XMFLOAT2,
		const DirectX::XMFLOAT2
	);
	Transform(
		GameObject* const, 
		const DirectX::XMFLOAT3,
		const DirectX::XMFLOAT3, 
		const DirectX::XMFLOAT3);
	Transform(
		GameObject* const,
		const DirectX::XMVECTOR,
		const DirectX::XMVECTOR,
		const DirectX::XMVECTOR);
	~Transform();

public:
	void OnStart() override;
	void OnDestroy() override;

public:
	/// <summary>
	/// 월드 좌표 -> 로컬 좌표로 변환합니다.
	/// </summary>
	/// <param name="_transform">변환할 Transform.</param>
	/// <returns></returns>
	static const DirectX::XMVECTOR WorldToLocalPosition(const Transform&);
	static const DirectX::XMVECTOR LocalToWorldPosition(const Transform&);

	static const DirectX::XMVECTOR WorldToLocalScale(const Transform&);
	static const DirectX::XMVECTOR LocalToWorldScale(const Transform&);

	static const DirectX::XMVECTOR WorldToLocalRotation(const Transform&);
	static const DirectX::XMVECTOR LocalToWorldRotation(const Transform&);

public:
#pragma region Getters
#pragma region Position
	inline const DirectX::XMVECTOR	GetPosition()	const;
	inline const FLOAT				GetPositionX()	const;
	inline const FLOAT				GetPositionY()	const;
	inline const FLOAT				GetPositionZ()	const;

	inline const DirectX::XMVECTOR	GetLocalPosition()	const;
	inline const FLOAT				GetLocalPositionX() const;
	inline const FLOAT				GetLocalPositionY() const;
	inline const FLOAT				GetLocalPositionZ() const;
#pragma endregion
#pragma region Scale
	inline const DirectX::XMVECTOR	GetScale()	const;
	inline const FLOAT				GetScaleX() const;
	inline const FLOAT				GetScaleY() const;
	inline const FLOAT				GetScaleZ() const;

	inline const DirectX::XMVECTOR	GetLocalScale()		const;
	inline const FLOAT				GetLocalScaleX()	const;
	inline const FLOAT				GetLocalScaleY()	const;
	inline const FLOAT				GetLocalScaleZ()	const;
#pragma endregion
#pragma region Rotation
	inline const DirectX::XMVECTOR	GetRotation()	const;
	inline const FLOAT				GetRotationX()	const;
	inline const FLOAT				GetRotationY()	const;
	inline const FLOAT				GetRotationZ()	const;

	inline const DirectX::XMVECTOR	GetLocalRotation()	const;
	inline const FLOAT				GetLocalRotationX() const;
	inline const FLOAT				GetLocalRotationY() const;
	inline const FLOAT				GetLocalRotationZ() const;
#pragma endregion
	inline const std::shared_ptr<Transform>& GetParent() const;
	inline const std::shared_ptr<Transform>& GetChild(const size_t) const;
	inline const std::vector<std::shared_ptr<Transform>>& GetChildren() const;
#pragma endregion
#pragma region Setters
	inline void SetPosition(const DirectX::XMFLOAT2);
	inline void SetPosition(const DirectX::XMFLOAT3);
	inline void SetPosition(const DirectX::XMVECTOR);

	inline void SetLocalPosition(const DirectX::XMFLOAT2);
	inline void SetLocalPosition(const DirectX::XMFLOAT3);
	inline void SetLocalPosition(const DirectX::XMVECTOR);
	
	inline void SetScale(const DirectX::XMFLOAT2);
	inline void SetScale(const DirectX::XMFLOAT3);
	inline void SetScale(const DirectX::XMVECTOR);
	
	inline void SetLocalScale(const DirectX::XMFLOAT2);
	inline void SetLocalScale(const DirectX::XMFLOAT3);
	inline void SetLocalScale(const DirectX::XMVECTOR);

	inline void SetRotation(const DirectX::XMFLOAT2);
	inline void SetRotation(const DirectX::XMFLOAT3);
	inline void SetRotation(const DirectX::XMVECTOR);

	inline void SetLocalRotation(const DirectX::XMFLOAT2);
	inline void SetLocalRotation(const DirectX::XMFLOAT3);
	inline void SetLocalRotation(const DirectX::XMVECTOR);
#pragma endregion
public:
};