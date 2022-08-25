#pragma once
#include"Model.h"
#include"WorldTransform.h"
#include"ViewProjection.h"
#include <assert.h>
#include"Affin.h"

/// <summary>
/// 天球
/// </summary>
class Skydome
{
public:


	void Initialize(Model* model);

	void Update();

	void Draw(ViewProjection viewProjection);
private:
	//ワールド変換データ
	WorldTransform worldTransform_;
	//モデル
	Model* model_ = nullptr;

	Matrix4 affinTrans = MathUtility::Matrix4Identity();
	Matrix4 affinRotate = MathUtility::Matrix4Identity();
	Matrix4 affinScale = MathUtility::Matrix4Identity();


};


