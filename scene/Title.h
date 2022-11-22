#pragma once
#include "DebugText.h"
#include "Input.h"
#include "ViewProjection.h"
#include <Model.h>
#include <WorldTransform.h>
#include <affin.h>
#include <cassert>
#include <list>

class Title {
  public:
	///< summary>
	///初期化
	///</summary>
	void Initialize(Model* model, uint32_t textureHandle);

	///< summary>
	///更新処理
	///</summary>
	void Update();

	///< summary>
	///描画処理
	///</summary>
	void Draw(ViewProjection viewProjection_);

	
  private:
	//ワールド変換データ
	WorldTransform worldTransform_;
	//モデル
	Model* model_ = nullptr;
	//テクスチャハンドル
	uint32_t textureHandle_ = 0u;

	Input* input_ = nullptr;
	DebugText* debugText_ = nullptr;

};
