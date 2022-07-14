#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include <random>
#include <cmath>

using namespace DirectX;

GameScene::GameScene() {}

GameScene::~GameScene() {
	// delete sprite_;
	delete model_;

	delete player_;

	delete enemy_;

	delete floor_;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();



	textureHandle_ = TextureManager::Load("mario.jpg");
	textureHandle2_ = TextureManager::Load("block.png");
	textureHandle3_ = TextureManager::Load("sibafu.jpg");

	// sprite_ = Sprite::Create(textureHandle_, { 100,50 });
	model_ = Model::Create();


	//カメラ視点座標を設定
	viewProjection_.eye = { 0,0,0 };
	viewProjection_.nearZ = 10;

	//ビュープロジェクションの初期化
	viewProjection_.Initialize();

	//インスタンス生成
	player_ = new Player();
	enemy_ = new Enemy();
	floor_ = new Floor();

	//初期化
	player_->Initialize(model_, textureHandle_);
	enemy_->Initialize(model_, textureHandle2_);
	floor_->Initialize(model_, textureHandle3_);



	//音声再生
	// audio_->PlayWave(soundDataHandle_);
}

void GameScene::Update() {
	
	//自キャラの更新
	player_->Update();

	//敵キャラの更新
	enemy_->Update();


	//転送用の座標
	Vector3 position = worldTransform_.translation_;

	player_->Attack();

	//ビュー変換
	viewProjection_.target = enemy_->worldTransform_.translation_;	//注視

	//敵から自機にかけてのベクトル計算
	Vector3 eyePos = { player_->worldTransform_.translation_.x - enemy_->worldTransform_.translation_.x,
	player_->worldTransform_.translation_.y - enemy_->worldTransform_.translation_.y,
	player_->worldTransform_.translation_.z - enemy_->worldTransform_.translation_.z };
	
	float mag = 1.0f;
	float eyeLen = std::sqrt(eyePos.x * eyePos.x + eyePos.y * eyePos.y + eyePos.z * eyePos.z);	//ベクトルの長さ
	if (eyeLen > 1.0f) {
		mag = 1.0f / eyeLen; //ベクトルの長さを1にする
	};

	eyePos.x *= mag;	
	eyePos.y *= mag;
	eyePos.z *= mag;

	for (int i = 0; i<40; i++) {
		if (floor_->wallTransform.translation_.x - floor_->wallTransform.scale_.x <	//矩形の当たり判定
			player_->worldTransform_.translation_.x + eyePos.x * i &&
			floor_->wallTransform.translation_.x + floor_->wallTransform.scale_.x >
			player_->worldTransform_.translation_.x + eyePos.x * i
			) {
			if (floor_->wallTransform.translation_.z - floor_->wallTransform.scale_.z <
				player_->worldTransform_.translation_.z + eyePos.x * i &&
				floor_->wallTransform.translation_.z + floor_->wallTransform.scale_.z >
				player_->worldTransform_.translation_.z + eyePos.x * i) {

				cameraDistance = i;
				break;

			}
		}else if (i >= 39) {
			cameraDistance = i;
		}
		
	}
	
	

	viewProjection_.eye =	//ビュープロジェクションに代入
	{ player_->worldTransform_.translation_.x + eyePos.x * cameraDistance/*長さが40*/,
	20, 
	player_->worldTransform_.translation_.z + eyePos.z * cameraDistance };

	

	//行列の再計算
	viewProjection_.UpdateMatrix();

	//デバッグ用表示
#pragma region debugText
	debugText_->SetPos(50, 70);
	debugText_->Printf(
		"target:(%f,%f,%f)", viewProjection_.target.x, viewProjection_.target.y,
		viewProjection_.target.z);

	debugText_->SetPos(50, 90);
	debugText_->Printf(
		"up:(%f,%f,%f)", viewProjection_.up.x, viewProjection_.up.y, viewProjection_.up.z);
#pragma endregion 

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

	player_->Draw(viewProjection_);
	enemy_->Draw(viewProjection_);
	floor_->Draw(viewProjection_);

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
}
