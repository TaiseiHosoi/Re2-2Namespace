#include "Skydome.h"


void Skydome::Initialize(Model* model)
{
	//NULLポインタチェック
	assert(model);
	model_ = model;

	//ワールド変換の初期化
	worldTransform_.Initialize();

	// X,Y,Z方向スケーリング設定
	worldTransform_.rotation_ = { 1.0f, 1.0f, 1.0f };
	worldTransform_.translation_ = { 1.0f, 1.0f, 1.0f };
	worldTransform_.scale_ = { 100.0f, 100.0f, 100.0f };

	Affin::UpdateScale(affinScale, worldTransform_);
	Affin::UpdateMatrixWorld(affinScale, affinTrans, affinRotate, worldTransform_);

	//行列の転送
	worldTransform_.TransferMatrix();
}

void Skydome::Update()
{
}

void Skydome::Draw(ViewProjection viewProjection)
{
	model_->Draw(worldTransform_, viewProjection);
}

