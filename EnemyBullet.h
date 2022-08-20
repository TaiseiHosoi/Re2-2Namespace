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


class EnemyBullet
{

public:


	void Initialize(Model* model, const Vector3& position, const Vector3& velocity);
	void Update();
	void Draw(const ViewProjection& viewProjection);

	bool IsDead() const { return isDead_; }

private:
	WorldTransform worldTransform_;

	Model* model_ = nullptr;

	uint32_t textureHandle_ = 0u;

	Input* input_ = nullptr;
	DebugText* debugText_ = nullptr;

	Matrix4 matTrans = MathUtility::Matrix4Identity();
	Matrix4 matScale = MathUtility::Matrix4Identity();
	Matrix4 matRotate = MathUtility::Matrix4Identity();

	//JUMYO-
	static const int32_t kLifeTime = 60 * 5;

	//deathTimer
	int32_t deathTimer_ = kLifeTime;

	//deathFlag
	bool isDead_ = false;


	Vector3 velocity_;



};