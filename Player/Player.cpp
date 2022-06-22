#include "DebugText.h"
#include "Input.h"
#include <Player.h>

void Player::Initialize(Model* model, uint32_t &textureHandle) {

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
	worldTransform_.scale_ = {1.0f, 1.0f, 1.0f};
	worldTransform_.translation_ = {1.0f, 1.0f, 1.0f};

}

void Player::Update() {


		//移動
		Vector3 move = {0.0f, 0.0f, 0.0f};

		//視点の移動速さ
		const float kEyeSpeed = 0.2f;

		//押した方向で移動ベクトルを変更
		if (input_->PushKey(DIK_W)) {
			move += {0, kEyeSpeed, 0};
		} else if (input_->PushKey(DIK_S)) {
			move -= {0, kEyeSpeed, 0};
		}

		if (input_->PushKey(DIK_A)) {
			move += {-kEyeSpeed, 0, 0};
		} else if (input_->PushKey(DIK_D)) {
			move += {kEyeSpeed, 0, 0};
		}

		worldTransform_.translation_ += move;

		//回転
		Vector3 rotateY = { 0.0f,0.0f,0.0f };

		const float kRotateSpeed = 0.1f;
		if (input_->PushKey(DIK_E)) {
			rotateY += {0, kRotateSpeed, 0};
		}
		else if (input_->PushKey(DIK_Q)) {
			rotateY -= {0, kRotateSpeed, 0};
		}
		worldTransform_.rotation_ += rotateY;

		//アフィン行列計算
		Affin::UpdateRotateY(affinRotate, worldTransform_);
		Affin::UpdateTrans(affinTrans, worldTransform_);
		Affin::UpdateMatrixWorld(affinTrans, affinRotate, worldTransform_);

		//アフィン行列転送
		worldTransform_.TransferMatrix();

	
}

void Player::Draw(ViewProjection viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);

	//弾描画
	for(std::unique_ptr<PlayerBullet>& bullet:bullets_) {
		bullet->Draw(viewProjection);
	}
}

void Player::Attack()
{
	if (input_->TriggerKey(DIK_V)) {

		//弾を生成し初期化
		std::unique_ptr<PlayerBullet> newBullet = std::make_unique<PlayerBullet>();
		newBullet->Initialize(model_,worldTransform_.translation_);

		//弾を登録
		bullets_.push_back(std::move(newBullet));
	}

}
