#include "RailCamera.h"


void RailCamera::Initialize(WorldTransform worldTransform)
{
	//ワールド座標初期化
	worldTransform_.translation_ = worldTransform.translation_;
	worldTransform_.rotation_ = worldTransform.rotation_;
	worldTransform_.Initialize();

	//ビュープロジェクション初期化
	viewprojection_.Initialize();
	viewprojection_.farZ = 2000.0f;
	viewprojection_.nearZ = 2.0f;
	viewprojection_.fovAngleY = 1.4f;


	//インスタンス取得
	input_ = Input::GetInstance();

}

void RailCamera::Update()
{

	//回転
	Vector3 rotate = { 0.0f,0.0f,0.0f };

	const float kRotateSpeed = 0.01f;
	if (input_->PushKey(DIK_S)) {
	rotate += {kRotateSpeed, 0, 0};
	}
	else if (input_->PushKey(DIK_W)) {
	rotate -= {kRotateSpeed, 0, 0};
	}
	if (input_->PushKey(DIK_A)) {
		rotate += {0, kRotateSpeed, 0};
	}
	else if (input_->PushKey(DIK_D)) {
		rotate -= {0, kRotateSpeed, 0};
	}

	if (worldTransform_.rotation_.x > 0.6f) {
		worldTransform_.rotation_.x = 0.6f;
	}
	else if (worldTransform_.rotation_.x < -0.6f) {
		worldTransform_.rotation_.x = -0.6f;
	}
	

	worldTransform_.rotation_ += rotate;


	//視点の移動速さ
	const float kEyeSpeed = 0.1f;
	//移動
	Vector3 move = { 0.0f, 0.0f, kEyeSpeed };

	Affin::MatVec(move, worldTransform_);
	
	worldTransform_.translation_ += move;


	//アフィン行列計算
	Affin::AffinUpdate(worldTransform_);

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
