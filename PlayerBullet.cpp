#include "PlayerBullet.h"


void PlayerBullet::Initialize(Model* model, const Vector3& position)
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
	Affin::UpdateMatrixWorld(matTrans, matRotate, worldTransform_);

	worldTransform_.TransferMatrix();
}

void PlayerBullet::Update()
{

}

void PlayerBullet::Draw(const ViewProjection& viewProjection)
{
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}