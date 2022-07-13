#pragma once

#include "WorldTransform.h"
#include "WinApp.h"
#include "Model.h"
#include "DebugText.h"
#include "Input.h"
#include "Affin.h"
#include "stdint.h"


class Floor {

public:

    //初期化
    void Initialize(Model* model_, uint32_t textureHandle);

    //描画
    void Draw(ViewProjection& viewProjection);

    //Vector3 GetTrans();

    //ワールド変換データ
    WorldTransform worldtransform;

private:

    //モデル
    Model* model_ = nullptr;
    //テクスチャハンドル
    uint32_t textureHandle_ = 0u;

    DebugText* debugText_ = nullptr;


    Matrix4 affinTrans = MathUtility::Matrix4Identity();
    Matrix4 affinRotate = MathUtility::Matrix4Identity();
    Matrix4 affinScale = MathUtility::Matrix4Identity();

};
