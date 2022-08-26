#include "RailCamera.h"


void RailCamera::Initialize(WorldTransform worldTransform)
{
	//ワールド座標初期化
	worldTransform_.translation_ = worldTransform.translation_;
	worldTransform_.rotation_ = worldTransform.rotation_;
	worldTransform_.Initialize();

	//ビュープロジェクション初期化
	viewprojection_.Initialize();



	//インスタンス取得
	input_ = Input::GetInstance();

}

void RailCamera::Update()
{

	//移動
	Vector3 move = { 0.0f, 0.0f, 0.0f };

	//視点の移動速さ
	const float kEyeSpeed = 0.2f;

	//押した方向で移動ベクトルを変更
	if (input_->PushKey(DIK_Y)) {
		move += {0, kEyeSpeed, 0};
	}
	else if (input_->PushKey(DIK_H)) {
		move -= {0, kEyeSpeed, 0};
	}

	if (input_->PushKey(DIK_G)) {
		move += {-kEyeSpeed, 0, 0};
	}
	else if (input_->PushKey(DIK_J)) {
		move += {kEyeSpeed, 0, 0};
	}

	worldTransform_.translation_ += move;

	//回転
	Vector3 rotateY = { 0.0f,0.0f,0.0f };

	const float kRotateSpeed = 0.01f;
	if (input_->PushKey(DIK_T)) {
		rotateY += {0, kRotateSpeed, 0};
	}
	else if (input_->PushKey(DIK_U)) {
		rotateY -= {0, kRotateSpeed, 0};
	}
	worldTransform_.rotation_ += rotateY;

	//アフィン行列計算
	Affin::UpdateRotateY(affinRotate, worldTransform_);
	Affin::UpdateTrans(affinTrans, worldTransform_);
	Affin::UpdateMatrixWorld(affinScale, affinTrans, affinRotate, worldTransform_);

	//アフィン行列転送
	worldTransform_.TransferMatrix();

	viewprojection_.eye = worldTransform_.translation_;
	//ワールド前方ベクトル
	Vector3 forward(0, 0, 1);
	//レールカメラの回転を反映
	Affin::VectorUpdate(forward, worldTransform_);
	//視点から前方に進んだ位置が注視店
	viewprojection_.target = viewprojection_.eye + forward;
	//world上方ベクトル
	Vector3 up(0, 1, 0);
	//レールカメラの回転を反映
	Affin::VectorUpdate(up, worldTransform_);
	//ビュープロジェクションを更新
	viewprojection_.UpdateMatrix();
	viewprojection_.TransferMatrix();

}

ViewProjection RailCamera::GetViewProjection()
{
	return viewprojection_;
}

WorldTransform* RailCamera::GetWorldTransform()
{
	return &worldTransform_;
}
