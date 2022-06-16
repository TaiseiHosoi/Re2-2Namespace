#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "DebugText.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "DebugCamera.h"
#include"Affin.h"

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
	/// 
	
	enum PartId {
		Root,	//大元
		Spine,	//脊椎
		Chest,	//胸
		Head,	//頭
		ArmL,	//左上
		ArmR,	//右腕
		Hip,	//尻
		LegL,	//左足
		LegR,	//右足
	};

	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;
	DebugText* debugText_ = nullptr;

	//テクスチャハンドル
	uint32_t textureHandle_ = 0;
	//3Dモデル
	Model* model_ = nullptr;

	//ワールドトランスフォーム
	WorldTransform worldTransforms_[10];

	//ビュープロジェクション
	ViewProjection viewProjection_;

	//カメラ上方向の角度
	float viewAngle = 0.0f;

	//変化させる量の箱
	Matrix4 matScale;
	Matrix4 matTrans[10];
	Matrix4 matRotate[10];


	//デバッグカメラ
	DebugCamera* debugCamera_ = nullptr;

	Vector3 lineStart(int x, int y, int z);
	Vector3 lineFinish(int x, int y, int z);
	Vector4 lineColor(int x, int y, int z, int w);

	/// <summary>
	/// ゲームシーン用
	/// </summary>
};