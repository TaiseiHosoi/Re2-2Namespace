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
	worldTransform_.translation_ = { 2.0f, 1.0f, 6.0f };

}

void Enemy::Update()
{
	

	//移動
	Vector3 move = { 0.0f, 0.0f, 0.0f };

	//視点の移動速さ
	const float kEyeSpeed = 0.2f;

	//押した方向で移動ベクトルを変更
	/*if (input_->PushKey(DIK_I)) {
		move += {0, kEyeSpeed, 0};
	}
	else if (input_->PushKey(DIK_K)) {
		move -= {0, kEyeSpeed, 0};
	}

	if (input_->PushKey(DIK_J)) {
		move += {-kEyeSpeed, 0, 0};
	}
	else if (input_->PushKey(DIK_L)) {
		move += {kEyeSpeed, 0, 0};
	}*/

	move += {0, 0, -kEyeSpeed};

	worldTransform_.translation_ += move;

	//回転
	Vector3 rotateY = { 0.0f,0.0f,0.0f };

	const float kRotateSpeed = 0.1f;

	/*if (input_->PushKey(DIK_E)) {
		rotateY += {0, kRotateSpeed, 0};
	}
	else if (input_->PushKey(DIK_Q)) {
		rotateY -= {0, kRotateSpeed, 0};
	}
	worldTransform_.rotation_ += rotateY;*/

	//アフィン行列計算
	Affin::UpdateRotateY(affinRotate, worldTransform_);
	Affin::UpdateTrans(affinTrans, worldTransform_);
	Affin::UpdateMatrixWorld(affinTrans, affinRotate, worldTransform_);

	//アフィン行列転送
	worldTransform_.TransferMatrix();
}

void Enemy::Draw(ViewProjection viewProjection)
{
	model_->Draw(worldTransform_, viewProjection, textureHandle_);

}
