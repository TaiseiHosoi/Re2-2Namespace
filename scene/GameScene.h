#pragma once

#include "Audio.h"
#include "DebugText.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "Player.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include <DirectXMath.h>
#include "Enemy.h"
#include "Skydome.h"
#include "RailCamera.h"
#include "DebugCamera.h"
#include "EnemyBullet.h"
#include "PlayerBullet.h"
#include <sstream>
#include <fstream>
#include <ostream>
#include <stdio.h>
#include <iostream>




/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {

public: // メンバ関数
  /// <summary>
  /// コンストクラタ
  /// </summary>
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
	void AddEnemyBullet(std::unique_ptr<EnemyBullet>enemyBullet);

	void InitEnemy();
	//タマリストを取得
	const std::list<std::unique_ptr<EnemyBullet>>& GetEnemyBullet() {
		return enemyBullets_;
	}
	const std::list<std::unique_ptr<PlayerBullet>>& GetPlayerBullet() {
		return playerBullets_;
	}

	//リムーブ
	void BulletClean();

	//敵発生コマンド
	void LoadEnemyPopData();
	void UpdateEnemyPopCommands();


	//敵発生コマンド
	std::stringstream enemyPopCommands;

	//敵生成
	void SponeEnemy(Vector3 EnemyPos);

	

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;
	DebugText* debugText_ = nullptr;
	Skydome* skydome_ = nullptr;	//天球
	RailCamera* railCamera_ = nullptr;	//レールカメラ
	WorldTransform cameraTransform;	//カメラ用のトランスフォーム
	DebugCamera* debugCamera_ = nullptr;	//デバッグカメラ

	uint32_t textureHandle_ = 0; //テクスチャハンドル
	uint32_t textureHandle2_ = 0;
	uint32_t textureHandle3_ = 0;
	uint32_t soundDataHandle_ = 0;
	uint32_t voiceHandle_ = 0;
	uint32_t title = 0;
	std::unique_ptr<Sprite> startSc_;
	//キャラ
	Player* player_ = nullptr;
	Enemy* enemy_ = nullptr;
	std::list<std::unique_ptr<Enemy>> enemys_;	//敵のリスト作成

	//弾
	std::list<std::unique_ptr<EnemyBullet>> enemyBullets_;
	std::list<std::unique_ptr<PlayerBullet>> playerBullets_;

	//カメラ上方向の角度
	float viewAngle = 0.0f;

	//model
	Sprite* sprite_ = nullptr;
	Model* model_ = nullptr;
	Model* enemyModel_ = nullptr;
	Model* modelSkydome_ = nullptr;
	Model* modelPlane_ = nullptr;

	//ワールドトランスフォーム初期化
	WorldTransform worldTransform_;
	WorldTransform enemyWorldTransform_;

	//ビュープロジェクション初期化
	ViewProjection viewProjection_;

	//待ち時間フラグ用変数
	bool isWait_;
	int waitTimer_;

	//シーン
	int scene = 0;;
	std::unique_ptr<Sprite> opening;
	/// <summary>
	/// ゲームシーン用
	/// </summary>
	///
};
