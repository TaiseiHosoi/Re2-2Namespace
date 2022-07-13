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

    //������
    void Initialize(Model* model_, uint32_t textureHandle);

    //�`��
    void Draw(ViewProjection& viewProjection);

    //Vector3 GetTrans();

    //���[���h�ϊ��f�[�^
    WorldTransform worldtransform;

private:

    //���f��
    Model* model_ = nullptr;
    //�e�N�X�`���n���h��
    uint32_t textureHandle_ = 0u;

    DebugText* debugText_ = nullptr;


    Matrix4 affinTrans = MathUtility::Matrix4Identity();
    Matrix4 affinRotate = MathUtility::Matrix4Identity();
    Matrix4 affinScale = MathUtility::Matrix4Identity();

};
