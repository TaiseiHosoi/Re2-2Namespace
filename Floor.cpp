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

    //ワールド変換初期化
    worldtransform.Initialize();

    worldtransform.translation_ = { -10,-10,0 };
    worldtransform.scale_ = { 100,1,100 };
    //アフィン行列計算
    Affin::UpdateRotateY(affinRotate, worldtransform);
    Affin::UpdateTrans(affinTrans, worldtransform);
    Affin::UpdateScale(affinScale, worldtransform);
    Affin::UpdateMatrixWorld(affinScale,affinTrans, affinRotate, worldtransform);

    worldtransform.TransferMatrix();

    

};


void Floor::Draw(ViewProjection& viewProjection_) {

    model_->Draw(worldtransform, viewProjection_, textureHandle_);

};

