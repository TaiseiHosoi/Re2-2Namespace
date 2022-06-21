#pragma once
#include "Audio.h"
#include "DebugText.h"
#include "Input.h"
#include "Math.h"
#include "Model.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "Windows.h"
#include "WorldTransform.h"
#include <DirectXMath.h>
#include"DirectXCommon.h"
#include"Affin.h"


class PlayerBullet
{
public:
	

	void Initialize(Model* model, const Vector3& position);
	void Update();
	void Draw(const ViewProjection& viewProjection);

private:
	WorldTransform worldTransform_;

	Model* model_ = nullptr;

	uint32_t textureHandle_ = 0u;

	Input* input_ = nullptr;
	DebugText* debugText_ = nullptr;

	Matrix4 matTrans = MathUtility::Matrix4Identity();
	Matrix4 matScale = MathUtility::Matrix4Identity();
	Matrix4 matRotate = MathUtility::Matrix4Identity();



};


