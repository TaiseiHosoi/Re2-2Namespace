#include "GameClear.h"

void GameClear::Initialize(Model* model, uint32_t textureHandle) {
	// NULLポインタチェック
	assert(model);
	model_ = model;
	textureHandle_ = textureHandle;

	//シングルインスタンスを取得する
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	//ワールド変換の初期化
	worldTransform_.Initialize();
	worldTransform_.translation_ = {0.9, 1.5, -20};
	worldTransform_.scale_ = {15, 5, 1};
}

void GameClear::Update() {
	//行列更新
	AffinTrans::affin(worldTransform_);

	worldTransform_.TransferMatrix();
}

void GameClear::Draw(ViewProjection viewProjection_) {
	model_->Draw(worldTransform_, viewProjection_, textureHandle_);
}
