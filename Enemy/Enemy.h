#pragma once
#include "Audio.h"
#include "DebugText.h"
#include <DirectXCommon.h>
#include "Input.h"
#include "Math.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "Windows.h"
#include "WorldTransform.h"
#include <DirectXMath.h>
#include <d3d12.h>
#include <wrl.h>
#include"EnemyBullet.h"
#include"memory"
#include "list"

class Enemy {
public:
	void Initialize(Model* model, uint32_t& textureHandle);
	void Update();
	void Draw(ViewProjection viewProjection);

	void Approach();	//接近
	void InitApproach();

	void Leave();	//離脱
	void InitLeave();

	void Fire();	//弾発射

	void BulletClean();
	
	enum class Phase {
		Approach,	//接近
		Leave,	//離脱
	};

	//玉のインターバル
	static const int kFireInterval = 30;

private:
	WorldTransform worldTransform_;

	Model* model_ = nullptr;

	Matrix4 matTrans;
	Matrix4 matScale;
	Matrix4 matRotate;

	uint32_t textureHandle_ = 0u;
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;
	DebugText* debugText_ = nullptr;

	std::list<std::unique_ptr<EnemyBullet>> bullets_;
	Matrix4 matVelocity = MathUtility::Matrix4Identity();	//velocity専用行列	

	Vector3 transPos = { 0, 0, 0 };

	Matrix4 affinTrans = MathUtility::Matrix4Identity();
	Matrix4 affinRotate = MathUtility::Matrix4Identity();
	Matrix4 affinScale = MathUtility::Matrix4Identity();

	//移動の速さ
	const float kEnemyCharacterSpeed = 0.2f;

	//x方向に移動
	Vector3 move = { kEnemyCharacterSpeed, 0, 0 };

	//フェーズ
	Phase phase_ = Phase::Approach;

	//フェーズの速さ
	const float kEnemyPhaseCharacterSpeed = 0.1f;

	//発射タイマー
	int32_t bFireTimer = 0;

};