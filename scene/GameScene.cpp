#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include <random>
#include"Vector3.h"

using namespace DirectX;

GameScene::GameScene() {}

GameScene::~GameScene() {
	// delete sprite_;
	delete model_;

	delete player_;

	delete enemy_;

	delete skydome_;

	delete modelSkydome_;

	delete railCamera_;

}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();
	debugCamera_ = new DebugCamera(1280,780);

	textureHandle_ = TextureManager::Load("mario.jpg");
	textureHandle2_ = TextureManager::Load("block.png");
	// sprite_ = Sprite::Create(textureHandle_, { 100,50 });
	model_ = Model::Create();


	//ビュープロジェクションの初期化
	viewProjection_.Initialize();

	//自キャラの生成
	player_ = new Player();

	enemy_ = new Enemy();
	//自キャラの初期化
	player_->Initialize(model_, textureHandle_);

	enemy_->Initialize(model_, textureHandle2_);

	enemy_->SetPlayer(player_);

	//天球生成
	skydome_ = new Skydome();

	modelSkydome_ = Model::CreateFromOBJ("skydome", true);

	skydome_->Initialize(modelSkydome_);

	//レールカメラ
	railCamera_ = new RailCamera();
	cameraTransform.Initialize();
	cameraTransform.rotation_ = { 0,0,0 };
	cameraTransform.translation_ = { 0,0,-50 };


	railCamera_->Initialize(cameraTransform);

	//音声再生
	// audio_->PlayWave(soundDataHandle_);
}

void GameScene::Update() {

	

	player_->SetWorldTransformPair(railCamera_->GetWorldTransform());

	//自キャラの更新
	player_->Update();
	
	//レールカメラ更新
	railCamera_->Update();

	//敵キャラの更新
	enemy_->Update();

	//転送用の座標
	Vector3 position = worldTransform_.translation_;

	player_->Attack();

	CheckAllCollisions();

	//自キャラの更新
	skydome_->Update();

	
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
	skydome_->Draw(railCamera_->GetViewProjection());

	player_->Draw(railCamera_->GetViewProjection());

	enemy_->Draw(railCamera_->GetViewProjection());

	skydome_->Draw(railCamera_->GetViewProjection());

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

void GameScene::CheckAllCollisions()
{
	//判定対象aとbの座標
	Vector3 posA, posB;

	//自機弾リストの取得
	const std::list<std::unique_ptr<PlayerBullet>>& playerBullet = player_->GetBulletd();
	//敵の弾リストの取得
	const std::list<std::unique_ptr<EnemyBullet>>& enemyBullet = enemy_->GetBulletd();

#pragma region 自キャラと敵弾の当たり判定
	//自キャラ座標
	posA = player_->GetWorldPosition();

	//自キャラと敵弾
	for (const std::unique_ptr<EnemyBullet>& bullet : enemyBullet) {
		posB = bullet.get()->GetWorldPosition();

		float x = posA.x - posB.x;
		float y = posA.y - posB.y;
		float z = posA.z - posB.z;

		float distance = sqrt(x * x + y * y + z * z);

		Matrix4 matA = player_->GetMatrix();
		Matrix4 matB = bullet->GetMatrix();

		//弾と弾の交差判定
		if (distance < matA.m[0][0] + matB.m[0][0]) {	//スケールxを半径として使用
			player_->OnCollision();
			bullet->OnCollision();
		}

	}
#pragma endregion


#pragma region 自弾と敵キャラの当たり判定
	//自キャラ座標
	posA = enemy_->GetWorldPosition();

	//自キャラと敵弾
	for (const std::unique_ptr<PlayerBullet>& bullet : playerBullet) {
		posB = bullet.get()->GetWorldPosition();

		float x = posA.x - posB.x;
		float y = posA.y - posB.y;
		float z = posA.z - posB.z;

		float distance = sqrt(x * x + y * y + z * z);

		Matrix4 matA = enemy_->GetMatrix();
		Matrix4 matB = bullet->GetMatrix();

		//弾と弾の交差判定
		if (distance <= matA.m[0][0] + matB.m[0][0]) {	//スケールxを半径として使用
			enemy_->OnCollision();
			bullet->OnCollision();
		}

	}
#pragma endregion
}
