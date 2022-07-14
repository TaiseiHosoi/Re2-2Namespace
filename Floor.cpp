#include "Floor.h"
#include "Affin.h"
#include "GameScene.h"


void Floor::Initialize(Model* model, uint32_t textureHandle) {

    //NULLポインタチェック
    assert(model);

    //引数としてうけとったデータを記録
    model_ = model;
    textureHandle_ = textureHandle;

    debugText_ = DebugText::GetInstance();

    //床の初期化
    floorTransform.Initialize();
    floorTransform.translation_ = { -10,-1,0 };
    floorTransform.scale_ = { 100,1,100 };
    

    //壁の初期化
    wallTransform.Initialize();
    wallTransform.translation_ = { 60,0,40 };
    wallTransform.scale_ = { 50,50,50 };

    //アフィン行列計算
    //床
    Affin::UpdateRotateY(affinRotate, floorTransform);
    Affin::UpdateTrans(affinTrans, floorTransform);
    Affin::UpdateScale(affinScale, floorTransform);
    Affin::UpdateMatrixWorld(affinScale, affinTrans, affinRotate, floorTransform);
    floorTransform.TransferMatrix();

    //壁
    Affin::UpdateRotateY(affinRotate, wallTransform);
    Affin::UpdateTrans(affinTrans, wallTransform);
    Affin::UpdateScale(affinScale, wallTransform);
    Affin::UpdateMatrixWorld(affinScale, affinTrans, affinRotate, wallTransform);
    wallTransform.TransferMatrix();



};


void Floor::Draw(ViewProjection& viewProjection_) {

    model_->Draw(floorTransform, viewProjection_, textureHandle_);
    model_->Draw(wallTransform, viewProjection_, textureHandle_);

};
