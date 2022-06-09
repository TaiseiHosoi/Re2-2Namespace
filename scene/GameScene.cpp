#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include "AxisIndicator.h"
#include "PrimitiveDrawer.h"



GameScene::GameScene() {

}

GameScene::~GameScene() {
	delete model_;
	delete debugCamera_;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();

	//ファイル名を指定してテクスチャを読み込む
	textureHandle_ = TextureManager::Load("mario.jpg");
	//3Dモデルの生成
	model_ = Model::Create();

	//ワールドトランスフォームの初期化
	worldTransform_.Initialize();
	// X,Y,Z方向トランスレーション設定
	worldTransform_.scale_ = { 2.0f,1.0f,1.0f };

	// X,Y,Z方向スケーリング設定
	worldTransform_.translation_ = { 5.0f,1.0f,1.0f };

	// X,Y,Z方向回転角設定
	worldTransform_.rotation_ = { 0.0f,0.0f,0.5f }; 

	

	Matrix4 matScale = MathUtility::Matrix4Identity();
	Matrix4 matTrans = MathUtility::Matrix4Identity();
	Matrix4 matRotate = MathUtility::Matrix4Identity();

	//worldTransform_.matWorld_ = {	//Matrixワールド座標初期化
	//	1.0f,0.0f,0.0f,0.0f,
	//	0.0f,1.0f,0.0f,0.0f,
	//	0.0f,0.0f,1.0f,0.0f,
	//	0.0f,0.0f,0.0f,1.0f
	//};

	//worldTransform_.matWorld_ *= matScale;

	Affin::UpdateTrans(matTrans,worldTransform_);
	Affin::UpdateScale(matScale,worldTransform_);
	Affin::UpdateRotate(matRotate,worldTransform_);


	worldTransform_.TransferMatrix();





	
	//ビュープロジェクションの初期化
	viewProjection_.Initialize();
	//デバッグカメラの生成
	debugCamera_ = new DebugCamera(1280, 720);
	//軸方向表示の表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);
	//軸方向表示が参照するビュープロジェクションを指定する(アドレス渡し)
	AxisIndicator::GetInstance()->SetTargetViewProjection(&debugCamera_->GetViewProjection());
	//ライン描画が参照するビュープロジェクションを指定する(アドレス渡し)
	PrimitiveDrawer::GetInstance()->SetViewProjection(&debugCamera_->GetViewProjection());
}

void GameScene::Update() {
	debugCamera_->Update();
}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>
	model_->Draw(worldTransform_, debugCamera_->GetViewProjection(), textureHandle_);

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// デバッグテキストの描画
	debugText_->DrawAll(commandList);
	//
	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
	//ライン描画が参照するビュープロジェクションを指定する(アドレス渡し)
	PrimitiveDrawer::GetInstance()->DrawLine3d(lineStart(-100, 0, -100), lineFinish(100, 0, 100), lineColor(255, 255, 255, 100));
}

Vector3 GameScene::lineStart(int x, int y, int z)
{
	return Vector3(x, y, z);
}

Vector3 GameScene::lineFinish(int x, int y, int z)
{
	return Vector3(x, y, z);
}

Vector4 GameScene::lineColor(int x, int y, int z, int w)
{
	return Vector4(x, y, z, w);
}