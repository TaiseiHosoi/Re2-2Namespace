#include "DebugText.h"
#include "Input.h"
#include <Player.h>

void Player::Initialize(Model* model, uint32_t &textureHandle) {

	affinTrans = {
		1.0f, 0.0f, 0.0f, 0.0f, 
		0.0f, 1.0f, 0.0f, 0.0f,
	    0.0f, 0.0f, 1.0f, 0.0f, 
		0.0f, 0.0f, 0.0f, 1.0f
	};


	// nullcheck
	assert(model);

	//�����������o�ϐ��ɋL�^
	model_ = model;
	textureHandle_ = textureHandle;

	//�V���O���؃C���X�^���X���擾����
	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();

	worldTransform_.Initialize();
	
	// X,Y,Z�����X�P�[�����O�ݒ�
	worldTransform_.scale_ = {1.0f, 1.0f, 1.0f};
	worldTransform_.translation_ = {1.0f, 1.0f, 1.0f};

}

void Player::Update() {

	//���_�̈ړ��x�N�g��
	{
		Vector3 move = {0.0f, 0.0f, 0.0f};

		//���_�̈ړ�����
		const float kEyeSpeed = 0.2f;

		//�����������ňړ��x�N�g����ύX
		if (input_->PushKey(DIK_W)) {
			move += {0, kEyeSpeed, 0};
		} else if (input_->PushKey(DIK_S)) {
			move -= {0, kEyeSpeed, 0};
		}

		if (input_->PushKey(DIK_A)) {
			move += {-kEyeSpeed, 0, 0};
		} else if (input_->PushKey(DIK_D)) {
			move += {kEyeSpeed, 0, 0};
		}

		affinTrans = {
			1.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
		    0.0f, 0.0f, 1.0f, 0.0f, 
            move.x, move.y, move.z, 1.0f
		};

		worldTransform_.matWorld_ *= affinTrans;

		worldTransform_.translation_.x = worldTransform_.matWorld_.m[3][0];

		worldTransform_.translation_.y = worldTransform_.matWorld_.m[3][1];


		worldTransform_.translation_.z = worldTransform_.matWorld_.m[3][2];

		worldTransform_.TransferMatrix();

		

	}
}

void Player::Draw(ViewProjection viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);

	//�e�`��
	if (bullet_) {
		bullet_->Draw(viewProjection);
	}
}

void Player::Attack()
{
	if (input_->PushKey(DIK_V)) {

		//�e�𐶐���������
		PlayerBullet* newBullet = new PlayerBullet();
		newBullet->Initialize(model_,worldTransform_.translation_);

		//�e��o�^
		bullet_ = newBullet;
	}

}
