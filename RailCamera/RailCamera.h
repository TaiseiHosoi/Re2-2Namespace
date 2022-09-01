#pragma once
#include"WorldTransform.h"
#include"ViewProjection.h"
#include <assert.h>
#include"Affin.h"
#include "Input.h"
#include "DebugText.h"
#include"Vector3.h"

class RailCamera {
public:
	//初期化
	void Initialize(WorldTransform worldTransform);

	//更新
	void Update();

	//ビュープロジェクション取得
	ViewProjection GetViewProjection();

	//ワールドトランスフォーム取得用
	WorldTransform* GetWorldTransform();

private:
	//ワールド変換データ
	WorldTransform worldTransform_;

	//ビュープロジェクション
	ViewProjection viewprojection_;

	//インプット
	Input* input_ = nullptr;

	//debugtext
	DebugText* debugText_ = nullptr;

	Matrix4 affinTrans = MathUtility::Matrix4Identity();
	Matrix4 affinRotate = MathUtility::Matrix4Identity();
	Matrix4 affinScale = MathUtility::Matrix4Identity();

};

