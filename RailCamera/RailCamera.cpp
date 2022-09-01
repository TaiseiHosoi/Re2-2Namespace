#include "RailCamera.h"


void RailCamera::Initialize(WorldTransform worldTransform)
{
	//���[���h���W������
	worldTransform_.translation_ = worldTransform.translation_;
	worldTransform_.rotation_ = worldTransform.rotation_;
	worldTransform_.Initialize();

	//�r���[�v���W�F�N�V����������
	viewprojection_.Initialize();
	viewprojection_.farZ = 2000.0f;
	viewprojection_.nearZ = 2.0f;
	viewprojection_.fovAngleY = 1.4f;


	//�C���X�^���X�擾
	input_ = Input::GetInstance();

}

void RailCamera::Update()
{

	//��]
	Vector3 rotate = { 0.0f,0.0f,0.0f };

	const float kRotateSpeed = 0.01f;
	if (input_->PushKey(DIK_S)) {
	rotate += {kRotateSpeed, 0, 0};
	}
	else if (input_->PushKey(DIK_W)) {
	rotate -= {kRotateSpeed, 0, 0};
	}
	if (input_->PushKey(DIK_A)) {
		rotate += {0, kRotateSpeed, 0};
	}
	else if (input_->PushKey(DIK_D)) {
		rotate -= {0, kRotateSpeed, 0};
	}

	if (worldTransform_.rotation_.x > 0.6f) {
		worldTransform_.rotation_.x = 0.6f;
	}
	else if (worldTransform_.rotation_.x < -0.6f) {
		worldTransform_.rotation_.x = -0.6f;
	}
	

	worldTransform_.rotation_ += rotate;


	//���_�̈ړ�����
	const float kEyeSpeed = 0.1f;
	//�ړ�
	Vector3 move = { 0.0f, 0.0f, kEyeSpeed };

	Affin::MatVec(move, worldTransform_);
	
	worldTransform_.translation_ += move;


	//�A�t�B���s��v�Z
	Affin::AffinUpdate(worldTransform_);

	//�A�t�B���s��]��
	worldTransform_.TransferMatrix();

	viewprojection_.eye = worldTransform_.translation_;
	//���[���h�O���x�N�g��
	Vector3 forward(0, 0, 1);
	//���[���J�����̉�]�𔽉f
	Affin::VectorUpdate(forward, worldTransform_);
	//���_����O���ɐi�񂾈ʒu�������X
	viewprojection_.target = viewprojection_.eye + forward;
	//world����x�N�g��
	Vector3 up(0, 1, 0);
	
	//���[���J�����̉�]�𔽉f
	Affin::VectorUpdate(up, worldTransform_);
	//�r���[�v���W�F�N�V�������X�V
	viewprojection_.UpdateMatrix();
	viewprojection_.TransferMatrix();

}

ViewProjection RailCamera::GetViewProjection()
{
	return viewprojection_;
}

WorldTransform* RailCamera::GetWorldTransform()
{
	return &worldTransform_;
}
