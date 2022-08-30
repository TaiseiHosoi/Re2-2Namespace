#include "DebugText.h"
#include "Input.h"
#include <Player.h>
#include <Affin.h>
#include "MathUtility.h"
using namespace MathUtility;

void Player::Initialize(Model* model, uint32_t& textureHandle) {

	// nullcheck
	assert(model);

	//引数をメンバ変数に記録
	model_ = model;
	textureHandle_ = textureHandle;

	//シングル豚インスタンスを取得する
	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();

	worldTransform_.Initialize();

	// X,Y,Z方向スケーリング設定
	worldTransform_.scale_ = { 1.0f, 1.0f, 1.0f };
	worldTransform_.translation_ = { 1.0f, 1.0f, 20.0f };

	//3Dレティクルの初期化
	worldTransform3DReticle_.Initialize();

	//レティクル用テクスチャ
	uint32_t textureReticle = TextureManager::Load("reticle3.png");

	//スプライト生成
	sprite2DReticle_.reset(Sprite::Create(textureReticle, Vector2(200,100), Vector4(1, 1, 1, 1), Vector2(0.5, 0.5)));

}

void Player::Update(const ViewProjection& viewProjection) {


	//bulletDelete
	bullets_.remove_if([](std::unique_ptr<PlayerBullet>& bullet) {
		return bullet->IsDead();
		});

	//移動
	Vector3 move = { 0.0f, 0.0f, 0.0f };

	//視点の移動速さ
	const float kEyeSpeed = 0.2f;

	//押した方向で移動ベクトルを変更
	if (input_->PushKey(DIK_W)) {
		move += {0, kEyeSpeed, 0};
	}
	else if (input_->PushKey(DIK_S)) {
		move -= {0, kEyeSpeed, 0};
	}

	if (input_->PushKey(DIK_A)) {
		move += {-kEyeSpeed, 0, 0};
	}
	else if (input_->PushKey(DIK_D)) {
		move += {kEyeSpeed, 0, 0};
	}

	worldTransform_.translation_ += move;

	//回転
	Vector3 rotateY = { 0.0f,0.0f,0.0f };

	const float kRotateSpeed = 0.01f;
	if (input_->PushKey(DIK_E)) {
		rotateY += {0, kRotateSpeed, 0};
	}
	else if (input_->PushKey(DIK_Q)) {
		rotateY -= {0, kRotateSpeed, 0};
	}
	worldTransform_.rotation_ += rotateY;

	//アフィン行列計算
	Affin::AffinUpdate(worldTransform_);

	//ペアレント先更新
	worldTransform_.matWorld_ *= worldTransform_.parent_->matWorld_;

	//アフィン行列転送
	worldTransform_.TransferMatrix();

	//弾更新
	for (std::unique_ptr<PlayerBullet>& bullet : bullets_) {
		bullet->Update();
	}


#pragma region レティクル更新
	//自機から3Dレティクルの距離
	const float kDistancePlayerTo3DReticle = 50.0f;
	//自機から3Dレティクルへのオフセットz向き
	Vector3 offset = { 0,0,1.0f };
	//自機のworld行列の回転を反映
	Affin::MatVec(offset, worldTransform_);
	//offset長さ調節
	offset = MathUtility::Vector3Normalize(offset) *  kDistancePlayerTo3DReticle;
	//3Dレティクル座標設定
	Vector3 getWorldMat = Affin::GetVecTrans(worldTransform_.matWorld_);
	worldTransform3DReticle_.translation_ = Affin::AddVector(Affin::GetVecTrans(worldTransform_.matWorld_), offset);

	//行列更新
	Affin::AffinUpdate(worldTransform3DReticle_);
	worldTransform3DReticle_.TransferMatrix();


#pragma endregion

#pragma region 3Dレティクルへの座標変換
	Vector3 positionReticle = Affin::GetVecTrans(worldTransform3DReticle_.matWorld_);

	Vector2 windowWH = Vector2(WinApp::GetInstance()->kWindowWidth, WinApp::GetInstance()->kWindowHeight);

	//ビューポート行列
	Matrix4 Viewport =
	{ windowWH.x / 2,0,0,0,
	0,-windowWH.y / 2,0,0,
	0,0,1,0,
	windowWH.x / 2, windowWH.y / 2,0,1 };

	//ビュー行列とプロジェクション行列,ビューポート行列を合成する
	Matrix4 matViewProjectionViewport = viewProjection.matView * viewProjection.matProjection * Viewport;

	//ワールド→スクリーン座標変換(ここで3D～2Dになる)
	positionReticle = Affin::WDiv(positionReticle, matViewProjectionViewport);

	//スプライトのレティクルに座業設定
	sprite2DReticle_->SetPosition(Vector2(positionReticle.x, positionReticle.y));


#pragma endregion

#pragma region マウスカーソルのスクリーン座標からワールド座標を取得して3Dレティクル配置
	POINT mousePosition;
	//マウス座標(スクリーン座標)を取得
	GetCursorPos(&mousePosition);

	//クライアントエリア座標に変換する
	HWND hwnd = WinApp::GetInstance()->GetHwnd();
	ScreenToClient(hwnd, &mousePosition);

	sprite2DReticle_->SetPosition(Vector2(mousePosition.x, mousePosition.y));

	Matrix4 matVPV = viewProjection.matView * viewProjection.matProjection * Viewport;

	Matrix4 matInverceVPV = MathUtility::Matrix4Inverse(matVPV);

	//スクリーン座標
	Vector3 posNear = Vector3(mousePosition.x, mousePosition.y, 0);
	Vector3 posFar = Vector3(mousePosition.x, mousePosition.y, 1);

	//マウスのレイ方向
	Vector3 mouseDirection = posFar - posNear;
	mouseDirection = MathUtility::Vector3Normalize(mouseDirection);

	//カメラから標準オブジェク距離
	const float kDistanceTestObject = 50.0f;
	worldTransform3DReticle_.translation_ = Affin::AddVector(posNear, mouseDirection * kDistanceTestObject);

	//行列更新
	Affin::AffinUpdate(worldTransform3DReticle_);
	worldTransform3DReticle_.TransferMatrix();

	DebugText::GetInstance()->SetPos(20, 200);
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
		worldTransform3DReticle_.translation_.z);
	
#pragma endregion
}

void Player::Draw(ViewProjection viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);

	//弾描画
	for (std::unique_ptr<PlayerBullet>& bullet : bullets_) {
		bullet->Draw(viewProjection);
	}

	//model_->Draw(worldTransform3DReticle_, viewProjection, textureHandle_);


	debugText_->SetPos(50, 110);
	debugText_->Printf(
		"reticle:(%f,%f,%f)", worldTransform3DReticle_.matWorld_.m[3][0], worldTransform3DReticle_.matWorld_.m[3][1], worldTransform3DReticle_.matWorld_.m[3][2]);

}

void Player::Attack()
{
	if (input_->TriggerKey(DIK_V)) {

		const float kBulletSpeed = 1.0f;
		Vector3 velocity(0, 0, kBulletSpeed);
		/*velocity = Affin::GetVecTrans(worldTransform3DReticle_.matWorld_)  - Affin::GetVecTrans(worldTransform_.matWorld_);
		MathUtility::Vector3Normalize(velocity);
		velocity *= kBulletSpeed;*/


		//速度ベクトルを自機の向きに回転させる
		Affin::MatVec(velocity, worldTransform_);
		
		

		Vector3 worldPos =	//初期値用ワールド座標取得
		{	worldTransform_.matWorld_.m[3][0],
			worldTransform_.matWorld_.m[3][1],
			worldTransform_.matWorld_.m[3][2] 
		};

		//弾を生成し初期化
		std::unique_ptr<PlayerBullet> newBullet = std::make_unique<PlayerBullet>();
		newBullet->Initialize(model_,worldPos, velocity);

		//弾を登録
		bullets_.push_back(std::move(newBullet));
	}

}

Vector3 Player::GetWorldPosition()
{
	//ワールド座標を入れる変数
	Vector3 worldPos;

	//ワールド行列移動成分を取得(ワールド座標)
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

