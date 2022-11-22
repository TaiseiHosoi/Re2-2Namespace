#pragma once

#include "Audio.h"
#include "DebugCamera.h"
#include "DebugText.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "affin.h"
#include "Player.h"
#include "Enemy.h"
#include "Skydome/Skydome.h"
#include "RailCamera/RailCamera.h"
#include "EnemyBullet.h"
#include <sstream>
#include "Title.h"
#include "push.h"
#include "GameClear.h"
#include "GameOver.h"

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {

  public: 
	// メンバ関数
	/// <summary>
	/// コンストクラタ
	GameScene();
	

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// 衝突判定と応答
	/// </summary>
	void CheckAllCollisions();

	/// <summary>
	/// 敵弾を追加する
	/// </summary>
	void AddEnemyBullet(std::unique_ptr<EnemyBullet>& enemyBullet);

	/// <summary>
	/// 敵発生データの読み込み
	/// </summary>
	void LoadEnemyPopData();

	/// <summary>
	/// 敵発生コマンドの更新
	/// </summary>
	void UpdataEnemyPopCommands();

	/// <summary>
	/// 敵の発生
	/// </summary>
	void GenerEnemy(Vector3 EnemyPos);

	//弾リストを取得
	const std::list<std::unique_ptr<EnemyBullet>>& GetBullets() { return enemybullets_; }

	//シーン切り替え
	enum class SceneNo {
		Title, //タイトル
		Game,  //射撃
		Clear, //ゲームクリア
		Over   //ゲメオーバー
	};


  private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;
	DebugText* debugText_ = nullptr;

	//テクスチャハンドル
	uint32_t textureHandle_ = 0;
	uint32_t textureHandle2_ = 0;
	//ボタンを押せ!!
	uint32_t textureHandle0_ = 0;
	push* push_ = nullptr;
	//タイトル
	uint32_t textureHandle3_ = 0;
	Title* title_ = nullptr;
	//ゲームクリア
	uint32_t textureHandle4_ = 0;
	GameClear* gameClear_ = 0;
	//ゲームオーバー
	uint32_t textureHandle5_ = 0;
	GameOver* gameOver_ = 0;

	// 3Dモデル
	Model* model_ = nullptr;

	
	//ビュープロジェクション
	ViewProjection viewProjection_;
	//デバッグカメラ
	DebugCamera* debugCamera_ = nullptr;
	//カメラ上方向
	float viewAngle = 0.0f;

	//自キャラ
	Player* player_ = nullptr;
	int playerRadius = 1;
	int playerBulletRadius = 1;
	//自機のの撃破カウント
	int playerTimer = 1000;
	//敵キャラ
	std::list<std::unique_ptr<Enemy>> enemys_;
	int enemyRadius = 1;
	int enemyBulletRadius = 1;
	//敵の撃破カウント
	int enemyDefeat = 0;
	//弾 複数
	std::list<std::unique_ptr<EnemyBullet>> enemybullets_;

	//スカイドーム
	Skydome* skydome_ = nullptr;
	//3Dモデル
	Model* modelSkydome_ = nullptr;

	//レールカメラ
	RailCamera* railCamera_ = nullptr;

	


	// 敵発生コマンド
	std::stringstream enemyPopCommands;

	Vector3 vector3(float x, float y, float z);
	Vector4 vector4(int x, int y, int z, int w);

	//待機中フラグ
	bool isStand_ = false;

	//待機タイマー
	int standTime_ = 0;

	SceneNo sceneNo_ = SceneNo::Title;


};