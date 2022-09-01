#include "DebugText.h"
#include "Input.h"
#include <Player.h>
#include <Affin.h>
#include "MathUtility.h"

using namespace MathUtility;

void Player::Initialize(Model* model, uint32_t& textureHandle) {

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
	worldTransform_.scale_ = { 1.0f, 1.0f, 1.0f };
	worldTransform_.translation_ = { 0.0f, 0.0f, 0.0f };

	//3D���e�B�N���̏�����
	worldTransform3DReticle_.Initialize();

	//���e�B�N���p�e�N�X�`��
	uint32_t textureReticle = TextureManager::Load("reticle3.png");

	//�X�v���C�g����
	sprite2DReticle_.reset(Sprite::Create(textureReticle, Vector2(200,100), Vector4(1, 1, 1, 1), Vector2(0.5, 0.5)));

}

void Player::Update(const ViewProjection& viewProjection) {
	//�O�t���[��
	oldQuadrant = Quadrant;


	//bulletDelete
	bullets_.remove_if([](std::unique_ptr<PlayerBullet>& bullet) {
		return bullet->IsDead();
		});




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
	}
	else if (input_->PushKey(DIK_D)) {
	}

	worldTransform_.rotation_ += rotate;

	//�ړ�
	Vector3 move = { 0.0f, 0.0f, 0.0f };

	//���_�̈ړ�����
	const float kEyeSpeed = 0.2f;

	if (unitAngle > 360.0f) {
		unitAngle -= 360.0f;
	}
	else if (unitAngle < 0.0f) {
		unitAngle += 360.0f;
	}

	if (worldTransform_.rotation_.x > 0.3f) {
		worldTransform_.rotation_.x = 0.3f;
	}
	else if (worldTransform_.rotation_.x < -0.3f) {
		worldTransform_.rotation_.x = -0.3f;
	}

	//�����������ňړ��x�N�g����ύX

	if (input_->PushKey(DIK_W)) {
		/*if (unitLen < 10) {
			unitLen+= 1.0f;
		}
		if (isPush == false) {
			isPush = true;
			unitAngle = 0.0f;
		}*/

		/*if(unitAngle < 350.0f && unitAngle > 10)
		if (Quadrant == 1) {
			unitAngle -= 2.0f;
		}
		else if (Quadrant == 2) {
			unitAngle -= 2.0f;
		}
		else if (Quadrant == 3) {
			unitAngle += 2.0f;
		}
		else if (Quadrant == 4) {
			unitAngle += 2.0f;
		}*/
		

	}
	else if (input_->PushKey(DIK_S)) {

		/*if (unitLen < 10) {
			unitLen+=1.0f;
		}
		if (isPush == false) {
			isPush = true;
			unitAngle = 180.0f;
		}*/

		//if (Quadrant == 1) {
		//	unitAngle += 2.0f;
		//}
		//else if (Quadrant == 2) {
		//	unitAngle += 2.0f;
		//}
		//else if (Quadrant == 3) {
		//	unitAngle -= 2.0f;
		//}
		//else if (Quadrant == 4) {
		//	unitAngle -= 2.0f;
		//}

		

	}else 
		if (input_->PushKey(DIK_D)) {


		/*if (unitLen < 10) {
			unitLen += 1.0f;
		}
		if (isPush == false) {
			isPush = true;
			unitAngle = 270.0f;
		}*/

		if (Quadrant == 1) {
			unitAngle -= 2.0f;
		}
		else if (Quadrant == 2) {
			unitAngle += 2.0f;
		}
		else if (Quadrant == 3) {
			unitAngle += 2.0f;
		}
		else if (Quadrant == 4) {
			unitAngle -= 2.0f;
		}

		
	}
	else if (input_->PushKey(DIK_A)) {

		//if (unitLen < 10) {
		//	unitLen += 1.0f;
		//}
		//if (isPush == false) {
		//	isPush = true;
		//	unitAngle = 90.0f;
		//}

		if (Quadrant == 1) {
			unitAngle += 2.0f;
		}
		else if (Quadrant == 2) {
			unitAngle -= 2.0f;
		}
		else if (Quadrant == 3) {
			unitAngle -= 2.0f;
		}
		else if (Quadrant == 4) {
			unitAngle += 2.0f;
		}

		
	}
	else {
		if (Quadrant == 1) {
			unitAngle += 2.0f;
		}
		else if (Quadrant == 2) {
			unitAngle += 2.0f;
		}
		else if (Quadrant == 3) {
			unitAngle -= 2.0f;
		}
		else if (Quadrant == 4) {
			unitAngle -= 2.0f;
		}
	}



	if (unitAngle >= 1.0f && unitAngle < 90.0f) {
		Quadrant = 1;
	}
	else if (unitAngle >= 90.0f && unitAngle < 180.0f) {
		Quadrant = 2;
	}
	else if (unitAngle >= 180.0f && unitAngle < 270.0f) {
		Quadrant = 3;
	}
	else if (unitAngle >= 270.0f && unitAngle < 360.0f) {
		Quadrant = 4;
	}

	//�u��
	if (input_->PushKey(DIK_W)) {
		
		//�u���}��
		if (oldQuadrant == 1 && Quadrant == 4) {
			unitAngle += 2.0f;
		}
		else if (oldQuadrant == 4 && Quadrant == 1) {
			unitAngle -= 2.0f;
		}

	}
	else if (input_->PushKey(DIK_S)) {

		
		//�u���}��
		if (oldQuadrant == 2 && Quadrant == 3) {
			unitAngle -= 2.0f;
		}
		else if (oldQuadrant == 3 && Quadrant == 2) {
			unitAngle += 2.0f;
		}

	}
	if (input_->PushKey(DIK_D)) {


		

		//�u���}��
		if (oldQuadrant == 3 && Quadrant == 4) {
			unitAngle -= 2.0f;
		}
		else if (oldQuadrant == 4 && Quadrant == 3) {
			unitAngle += 2.0f;
		}
	}
	else if (input_->PushKey(DIK_A)) {

		

		//�u���}��
		if (oldQuadrant == 1 && Quadrant == 2) {
			unitAngle -= 2.0f;
		}
		else if (oldQuadrant == 2 && Quadrant == 1) {
			unitAngle += 2.0f;
		}
	}

	

	if (input_->PushKey(DIK_W) == false &&
		input_->PushKey(DIK_S) == false &&
		input_->PushKey(DIK_A) == false &&
		input_->PushKey(DIK_D) == false) {
		
		/*if (unitLen > 0) {
			unitLen-= 1.0f;
		}*/
		isPush = false;
		//�u���}��
		if (oldQuadrant == 2 && Quadrant == 3) {
			unitAngle -= 2.0f;
		}
		else if (oldQuadrant == 3 && Quadrant == 2) {
			unitAngle += 2.0f;
		}

	}

	if (triggerPointAngle * PI / 180.0f <360.0f) {
		triggerPointAngle = 0.0f;
	}
	
	//unitAngle += angleSpeed;

	/*if (unitAngle > 2.00f) {
		unitAngle = 0.0f;
	}*/

	move = {sin(unitAngle*PI / 180.0f) * unitLen,0.0f,20.0f};

	worldTransform_.translation_ = move;

	//�A�t�B���s��v�Z
	Affin::AffinUpdate(worldTransform_);

	//�y�A�����g��X�V
	worldTransform_.matWorld_ *= worldTransform_.parent_->matWorld_;

	//�A�t�B���s��]��
	worldTransform_.TransferMatrix();

	//�e�X�V
	for (std::unique_ptr<PlayerBullet>& bullet : bullets_) {
		bullet->Update();
	}


#pragma region ���e�B�N���X�V
	//���@����3D���e�B�N���̋���
	const float kDistancePlayerTo3DReticle = 50.0f;
	//���@����3D���e�B�N���ւ̃I�t�Z�b�gz����
	Vector3 offset = { 0,0,1.0f };
	//���@��world�s��̉�]�𔽉f
	Affin::MatVec(offset, worldTransform_);
	//offset��������
	offset = MathUtility::Vector3Normalize(offset) *  kDistancePlayerTo3DReticle;

	
	//3D���e�B�N�����W�ݒ�
	Vector3 getWorldMat = Affin::GetVecTrans(worldTransform_.matWorld_);
	worldTransform3DReticle_.translation_ = Affin::AddVector(Affin::GetVecTrans(worldTransform_.matWorld_), offset);

	//�s��X�V
	Affin::AffinUpdate(worldTransform3DReticle_);
	worldTransform3DReticle_.TransferMatrix();


#pragma endregion

#pragma region 3D���e�B�N���ւ̍��W�ϊ�
	Vector3 positionReticle = Affin::GetVecTrans(worldTransform3DReticle_.matWorld_);
	
	Vector2 windowWH = Vector2(WinApp::GetInstance()->kWindowWidth, WinApp::GetInstance()->kWindowHeight);

	//�r���[�|�[�g�s��
	Matrix4 Viewport =
	{ windowWH.x / 2,0,0,0,
	0,-windowWH.y / 2,0,0,
	0,0,1,0,
	windowWH.x / 2, windowWH.y / 2,0,1 };

	//�r���[�s��ƃv���W�F�N�V�����s��,�r���[�|�[�g�s�����������
	matViewProjectionViewport = viewProjection.matView * viewProjection.matProjection * Viewport;

	//���[���h���X�N���[�����W�ϊ�(������3D�`2D�ɂȂ�)
	positionReticle = Affin::WDiv(positionReticle, matViewProjectionViewport);

	//�X�v���C�g�̃��e�B�N���ɍ��Ɛݒ�
	sprite2DReticle_->SetPosition(Vector2(positionReticle.x, positionReticle.y));


#pragma endregion

#pragma region �}�E�X�J�[�\���̃X�N���[�����W���烏�[���h���W���擾����3D���e�B�N���z�u
	//POINT mousePosition;
	////�}�E�X���W(�X�N���[�����W)���擾
	//GetCursorPos(&mousePosition);

	////�N���C�A���g�G���A���W�ɕϊ�����
	//HWND hwnd = WinApp::GetInstance()->GetHwnd();
	//ScreenToClient(hwnd, &mousePosition);

	//sprite2DReticle_->SetPosition(Vector2(mousePosition.x, mousePosition.y));

	//Matrix4 matVPV = viewProjection.matView * viewProjection.matProjection * Viewport;

	//Matrix4 matInverceVPV = MathUtility::Matrix4Inverse(matVPV);

	////�X�N���[�����W
	//Vector3 posNear = Vector3(mousePosition.x, mousePosition.y, 0);
	//Vector3 posFar = Vector3(mousePosition.x, mousePosition.y, 1);

	////�}�E�X�̃��C����
	//Vector3 mouseDirection = posFar - posNear;
	//mouseDirection = MathUtility::Vector3Normalize(mouseDirection);

	////�J��������W���I�u�W�F�N����
	//const float kDistanceTestObject = 50.0f;
	//worldTransform3DReticle_.translation_ = Affin::AddVector(posNear, mouseDirection * kDistanceTestObject);

	//�s��X�V
	Affin::AffinUpdate(worldTransform3DReticle_);
	worldTransform3DReticle_.TransferMatrix();

	/*DebugText::GetInstance()->SetPos(20, 200);
	DebugText::GetInstance()->Printf(
		"Mouse ScreenPos:(%d,%d)", mousePosition.x, mousePosition.y);
	DebugText::GetInstance()->SetPos(20, 220);
	DebugText::GetInstance()->Printf("Near:(%f,%f,%f)",posNear.x,posNear.y,posNear.z);
	DebugText::GetInstance()->SetPos(20, 240);
	DebugText::GetInstance()->Printf("Far:(%f,%f,%f)",posFar.x,posFar.y,posFar.z);
	DebugText::GetInstance()->SetPos(20, 260);
	DebugText::GetInstance()->Printf(
	"MouseObject:(%f,%f,%f)",
		worldTransform3DReticle_.translation_.x, 
		worldTransform3DReticle_.translation_.y,
		worldTransform3DReticle_.translation_.z);*/
	
#pragma endregion
}

void Player::Draw(ViewProjection viewProjection) {
	model_->Draw(worldTransform_, viewProjection);

	//�e�`��
	for (std::unique_ptr<PlayerBullet>& bullet : bullets_) {
		bullet->Draw(viewProjection);
	}


	//debugText_->SetPos(50, 110);
	//debugText_->Printf(
	//	"reticlePos:(%f,%f,%f)", worldCameraPos.x, worldCameraPos.y, worldCameraPos.z);

}

void Player::Attack()
{
	if (input_->IsTriggerMouse(0)) {

		const float kBulletSpeed = 5.0f;
		Vector3 velocity(0, 0, kBulletSpeed);
		velocity = Affin::GetVecTrans(worldTransform3DReticle_.matWorld_)  - Affin::GetVecTrans(worldTransform_.matWorld_);
		MathUtility::Vector3Normalize(velocity);
		velocity *= kBulletSpeed;


		//���x�x�N�g�������@�̌����ɉ�]������
		
	/*	velocity = { worldTransform3DReticle_.matWorld_.m[3][0] - worldTransform_.matWorld_.m[3][0],
			worldTransform3DReticle_.matWorld_.m[3][1] - worldTransform_.matWorld_.m[3][1],
			worldTransform3DReticle_.matWorld_.m[3][2] - worldTransform_.matWorld_.m[3][2] };*/

		/*velocity = MathUtility::Vector3Normalize(velocity);
		Affin::MatVec(velocity, worldTransform_);	*/


		Vector3 worldPos =	//�����l�p���[���h���W�擾
		{	worldTransform_.matWorld_.m[3][0],
			worldTransform_.matWorld_.m[3][1],
			worldTransform_.matWorld_.m[3][2] 
		};

		//�e�𐶐���������
		std::unique_ptr<PlayerBullet> newBullet = std::make_unique<PlayerBullet>();
		newBullet->Initialize(model_,worldPos, velocity);

		//�e��o�^
		bullets_.push_back(std::move(newBullet));
	}

}

Vector3 Player::GetWorldPosition()
{
	//���[���h���W������ϐ�
	Vector3 worldPos;

	//���[���h�s��ړ��������擾(���[���h���W)
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}

void Player::OnCollision()
{
}

Matrix4 Player::GetMatrix()
{
	return worldTransform_.matWorld_;
}

void Player::SetWorldTransformPair(WorldTransform* worldTransform)
{
	worldTransform_.parent_ = worldTransform;
}

void Player::DrawUI()
{
	sprite2DReticle_->Draw();
}

