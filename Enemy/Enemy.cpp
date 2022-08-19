#include "Enemy.h"
#include "DebugText.h"
#include "Input.h"

void Enemy::Initialize(Model* model, uint32_t& textureHandle)
{

	// nullcheck
	assert(model);

	//引数をメンバ変数に記録
	model_ = model;
	textureHandle_ = textureHandle;

	//シングル豚インスタンスを取得する
	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();

	worldTransform_.Initialize();

	// X,Y,Z方向スケーリング設定
	worldTransform_.scale_ = { 1.0f, 1.0f, 1.0f };
	worldTransform_.translation_ = { 2.0f, 1.0f, -20.0f };

}

void Enemy::Update()
{



	//アフィン行列計算
	Affin::UpdateRotateY(affinRotate, worldTransform_);
	Affin::UpdateTrans(affinTrans, worldTransform_);
	Affin::UpdateMatrixWorld(affinTrans, affinRotate, worldTransform_);

	//アフィン行列転送
	worldTransform_.TransferMatrix();

	//敵の動きをswitchで切り替え
	switch (phase_) {
	case Phase::Approach:

	default:
		Approach();
		break;

	case Phase::Leave:
		Leave();
		break;
	}

}

void Enemy::Draw(ViewProjection viewProjection)
{
	model_->Draw(worldTransform_, viewProjection, textureHandle_);

}


//敵の挙動something
void Enemy::Approach() {

	//移動ベクトル
	worldTransform_.translation_ += Vector3(0, 0, +kEnemyPhaseCharacterSpeed);

	//既定の位置に到達したら離脱
	if (worldTransform_.translation_.z > 0.0f) {
		phase_ = Phase::Leave;
	}
}

void Enemy::Leave() {

	//移動(ベクトルを加算)
	worldTransform_.translation_ += Vector3(0, 0, -kEnemyPhaseCharacterSpeed);
}