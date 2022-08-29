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
#include"PlayerBullet.h"
#include"memory"
#include "list"


class Player {

public:
	void Initialize(Model* model, uint32_t& textureHandle);

	void Update(const ViewProjection& viewProjection);

	void Draw(ViewProjection viewProjection);

	void Attack();

	Vector3 GetWorldPosition();

	void OnCollision();

	//弾リストを取得
	const std::list<std::unique_ptr<PlayerBullet>>& GetBulletd() {
		return bullets_;
	}

	Matrix4 GetMatrix();	//行列取得用

	void SetWorldTransformPair(WorldTransform* worldTransform);

	void DrawUI();


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

	std::list<std::unique_ptr<PlayerBullet>> bullets_;
	Matrix4 matVelocity = MathUtility::Matrix4Identity();	//velocity専用行列	

	Vector3 transPos = { 0, 0, 0 };

	//3Dレティクル用worldトランスフォーム
	WorldTransform worldTransform3DReticle_;

	
	//自機用
	Matrix4 affinTrans = MathUtility::Matrix4Identity();
	Matrix4 affinRotate = MathUtility::Matrix4Identity();
	Matrix4 affinScale = MathUtility::Matrix4Identity();

	//2Dレティクルスプライト用
	std::unique_ptr<Sprite> sprite2DReticle_;


};
