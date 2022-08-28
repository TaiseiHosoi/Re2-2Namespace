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
	debugCamera_ = new DebugCamera(1280, 780);

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

	enemy_->Initialize(model_, textureHandle2_, Vector3(2.0f, 1.0f, -20.0f));

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

	//ポップデータcsv読み込み
	LoadEnemyPopData();

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
	UpdateEnemyPopCommands();

	for (std::unique_ptr<Enemy>& enemy_ : enemys_) {
		enemy_->SetGameScene(this);
		enemy_->Update();
	}

	//弾更新
	for (std::unique_ptr<EnemyBullet>& bullet : enemyBullets_) {
		bullet->Update();
	}





	//転送用の座標
	Vector3 position = worldTransform_.translation_;

	player_->Attack();

	CheckAllCollisions();

	BulletClean();

	enemys_.remove_if([](std::unique_ptr<Enemy>& enemy) {	//デスフラグの立った敵リムーブ
		return enemy->Dead();
		});

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


	//弾描画
	/*for (std::unique_ptr<PlayerBullet>& pBullet : playerBullets_) {
		pBullet->Draw(railCamera_->GetViewProjection());
	}*/

	//弾描画
	for (std::unique_ptr<EnemyBullet>& bullet : enemyBullets_) {
		bullet->Draw(railCamera_->GetViewProjection());

	}

	player_->Draw(railCamera_->GetViewProjection());


	for (std::unique_ptr<Enemy>& enemy_ : enemys_) {
		enemy_->Draw(railCamera_->GetViewProjection());

	}

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
	const std::list<std::unique_ptr<EnemyBullet>>& enemyBullet = GetEnemyBullet();
	

	
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
	//キャラ座標
	for (std::unique_ptr<Enemy>& enemy_ : enemys_) {
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
	}
#pragma endregion
}

void GameScene::AddEnemyBullet(std::unique_ptr<EnemyBullet> enemyBullet)
{
	//リストに登録する
	//弾を登録
	enemyBullets_.push_back(std::move(enemyBullet));

}



void GameScene::BulletClean()
{

	//デスフラグが立った弾を削除
	enemyBullets_.remove_if([](std::unique_ptr<EnemyBullet>& bullet) {
		return bullet->IsDead();
		});
	playerBullets_.remove_if([](std::unique_ptr<PlayerBullet>& bullet) {
		return bullet->IsDead();
		});

}

void GameScene::LoadEnemyPopData()
{
	//ファイr開く
	std::ifstream file;
	file.open("Resources/enemyPop.csv");
	assert(file.is_open());

	//ファイルの内容を文字列ストリームにコピー
	enemyPopCommands << file.rdbuf();

	//ふぁいるを閉じる
	file.close();


}

void GameScene::UpdateEnemyPopCommands()
{

	//待機時間
	if (isWait_ == true) {
		waitTimer_--;
		if (waitTimer_ <= 0) {
			//待機終了
			isWait_ = false;

		}
		return;
	}

	//1行分の文字列を入れる変数
	std::string line;

	//コマンド実行ループ
	while (getline(enemyPopCommands, line)) {
		//1行分の文字数をストリームに変換して解折しやすくなる
		std::istringstream line_stream(line);

		std::string word;
		//,区切りで行の先頭文字を取得
		getline(line_stream, word, ',');

		//"//"から始まる行はコメント
		if (word.find("//") == 0) {
			//コメント行を飛ばす
			continue;
		}
		// POPコマンド
		if (word.find("POP") == 0) {
			// X座標
			std::getline(line_stream, word, ',');
			float x = static_cast<float>(std::atof(word.c_str()));

			// Y座標
			std::getline(line_stream, word, ',');
			float y = static_cast<float>(std::atof(word.c_str()));

			// Z座標
			std::getline(line_stream, word, ',');
			float z = static_cast<float>(std::atof(word.c_str()));

			SponeEnemy(Vector3(x, y, z));
		}
		// WAITコマンド
		else if (word.find("WAIT") == 0) {
			std::getline(line_stream, word, ',');

			//待ち時間
			int32_t waitTime = std::atoi(word.c_str());

			//待機開始
			isWait_ = true;
			waitTimer_ = waitTime;

			//ループを抜ける
			break;
		}

	}
}

void GameScene::SponeEnemy(Vector3 EnemyPos)
{
	//敵キャラの生成
	std::unique_ptr<Enemy> newEnemy = std::make_unique<Enemy>();

	//敵キャラの初期化
	newEnemy->Initialize(model_, textureHandle2_, EnemyPos);

	//プレイヤーの位置をセット
	newEnemy->SetPlayer(player_);

	//敵リスト
	enemys_.push_back(std::move(newEnemy));





}
