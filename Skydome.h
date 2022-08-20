#pragma once
#include"Model.h"
#include"WorldTransform.h"
#include"ViewProjection.h"
/// <summary>
/// 天球
/// </summary>
class Skydome
{
public
	

	void Initialize();

	void Update();

	void Draw(ViewProjection viewProjection);
private:
	//ワールド変換データ
	WorldTransform worldTransform_;
	//モデル
	Model* model_ = nullptr;

	
};

