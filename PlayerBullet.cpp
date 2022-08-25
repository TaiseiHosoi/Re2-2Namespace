#include "PlayerBullet.h"


void PlayerBullet::Initialize(Model* model, const Vector3& position, const Vector3& velocity)
{

	assert(model);

	model_ = model;

	//テクスチャ読み込み
	textureHandle_ = TextureManager::Load("mario.jpg");


	worldTransform_.Initialize();
	//引数で受け取った初期座標をセット
	worldTransform_.translation_ = position;

	worldTransform_.rotation_ = { 0.0f,0.0f,0.0f };

	worldTransform_.scale_ = { 1.0f,1.0f,1.0f };

	Affin::UpdateTrans(matTrans, worldTransform_);
	Affin::UpdateMatrixWorld(matScale, matTrans, matRotate, worldTransform_);

	worldTransform_.TransferMatrix();

	//引数で受け取った速度をメンバ変数に代入
	velocity_ = velocity;
}

void PlayerBullet::Update()
{
	//TimerDeath
	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}



	//座標を移動させる
	worldTransform_.translation_ += velocity_;

	//アフィン行列計算
	Affin::UpdateRotateY(matRotate, worldTransform_);
	Affin::UpdateTrans(matTrans, worldTransform_);
	Affin::UpdateMatrixWorld(matScale, matTrans, matRotate, worldTransform_);

	worldTransform_.TransferMatrix();

}

void PlayerBullet::Draw(const ViewProjection& viewProjection)
{
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}

void PlayerBullet::OnCollision()
{
	isDead_ = true;
}

Vector3 PlayerBullet::GetWorldPosition()
{

	//ワールド座標を入れる変数
	Vector3 worldPos;

	//ワールド行列移動成分を取得(ワールド座標)
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}

Matrix4 PlayerBullet::GetMatrix()
{
	return worldTransform_.matWorld_;
}
