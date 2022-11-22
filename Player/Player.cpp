#include "Player.h"

using namespace MathUtility;

void Player::Initialize(Model* model, uint32_t textureHandle) {
	//NULLポインタチェック
	assert(model);
	model_ = model;
	textureHandle_ = textureHandle;
	
	//シングルインスタンスを取得する
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	//3Dレティクル用ワールドトランスフォーム初期化
	worldTransform3DReticle_.Initialize();

	//ワールド変換の初期化
	worldTransform_.Initialize();
	worldTransform_.translation_ = {0, 0, 40};

	//レティクル用テクスチャ取得
	uint32_t textureReticle = TextureManager::Load("tage.png");
	//スプライト生成
	sprite2DReticle_.reset(
	  Sprite::Create(textureReticle, Vector2(500, 350), Vector4(1, 1, 1, 1), Vector2(0.5, 0.5)));


}

void Player::Update(ViewProjection viewProjection_) {
	
	//デスフラグの立った弾の削除
	bullets_.remove_if([](std::unique_ptr<PlayerBullet>& bullet) { 
		return bullet->IsDead();  
	});

	
	//キャラクターの移動ベクトル
	Vector3 move = {0, 0, 0};
	//キャラクターの移動の速さ
	const float kCharacterSpeed = 0.2f;

	//押した方向で移動ベクトルを変更
	if (input_->PushKey(DIK_W)) {
		move = {0, kCharacterSpeed, 0};
	} else if (input_->PushKey(DIK_S)) {
		move = {0, -kCharacterSpeed, 0};
	}
	if (input_->PushKey(DIK_A)) {
		move = {-kCharacterSpeed, 0, 0};
	} else if (input_->PushKey(DIK_D)) {
		move = {kCharacterSpeed, 0, 0};
	}

	worldTransform_.translation_ += move;

	//行列更新
	AffinTrans::affin(worldTransform_);

	worldTransform_.matWorld_ *= worldTransform_.parent_->matWorld_;

	worldTransform_.TransferMatrix();


	const float kMoveLimitX = 35;
	const float kMoveLimitY = 18;

	//範囲を超えない処理
	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kMoveLimitX);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, +kMoveLimitX);
	worldTransform_.translation_.y = max(worldTransform_.translation_.y, -kMoveLimitY);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y, +kMoveLimitY);

	const float kChestRotSpeed = 0.05f;

	//押した方向で移動ベクトルを変更
	if (input_->PushKey(DIK_J)) {
		worldTransform_.rotation_.y -= kChestRotSpeed;
	} else if (input_->PushKey(DIK_K)) {
		worldTransform_.rotation_.y += kChestRotSpeed;
	}

	//弾発射処理
	Attack();

	//弾更新
	//複数
	for (std::unique_ptr<PlayerBullet>& bullet : bullets_) {
		bullet->Update();
	}

	//単発
	/*if (bullet_) {
		bullet_->Update();
	}*/

	//---------自機のワールド座標から3Dレティクルのワールド座標を計算-----------//
	
	//自機から3Dレティクルへの距離
	const float kDistancePlayerTo3DReticle = 60.0f;
	//自機から3Dレティクルへのオフセット(Z+向き)
	Vector3 offset = {0, 0, 1.0f};
	//自機のワールド座標の回転を反映
	offset = AffinTrans::MatVector(offset,worldTransform_.matWorld_);
	//ベクトルの長さを整える
	offset = Vector3Normalize(offset) * kDistancePlayerTo3DReticle;
	//3Dレティクル座標設定
	worldTransform3DReticle_.translation_ =
	  offset + Vector3(
		  worldTransform_.matWorld_.m[3][0], 
		  worldTransform_.matWorld_.m[3][1],
	      worldTransform_.matWorld_.m[3][2]
	  );
	//行列更新
	AffinTrans::affin(worldTransform3DReticle_);
	worldTransform3DReticle_.TransferMatrix();
	
	/////////////////////////////////////////////////////////////////


	//----------3Dレティクルのワールド座標から2Dレティクルのスクリーン座標を計算-------//

	Vector3 positionReticle = AffinTrans::GetWorldtransform(worldTransform3DReticle_.matWorld_);

	Vector2 windowWH =
	  Vector2(WinApp::GetInstance()->kWindowWidth, WinApp::GetInstance()->kWindowHeight);

	//ビューポート行列
	Matrix4 Viewport = {
	   windowWH.x / 2,				  0,  0,  0, 
					0,	-windowWH.y / 2,  0,  0, 
				    0,				  0,  1,  0, 
	   windowWH.x / 2,	 windowWH.y / 2,  0,  1
	};
	  
	//ビュー行列とプロジェクション行列、ビューポート行列を合成する
	Matrix4 matViewProjectionViewport = viewProjection_.matView * viewProjection_.matProjection * Viewport;

	//ワールド→スクリーン座標変換(ここから3Dから2Dになる)
	positionReticle = AffinTrans::wDivision(positionReticle, matViewProjectionViewport);

	//スプライトのレティクルに座標設定
	sprite2DReticle_->SetPosition(Vector2(positionReticle.x, positionReticle.y));

	////////////////////////////////////////////////////////////////////////////////

	//----------スクリーン座標から2Dレティクルのワールド座標を計算-------//

	POINT mousePosition;
	//マウス座標(スクリーン座標)を取得する
	GetCursorPos(&mousePosition);

	//クライアントエリア座標に変換する
	HWND hwnd = WinApp::GetInstance()->GetHwnd();
	ScreenToClient(hwnd, &mousePosition);

	sprite2DReticle_->SetPosition(Vector2(mousePosition.x, mousePosition.y));

	//ビュープロジェクションビューポート合成
	Matrix4 matVPV = viewProjection_.matView * viewProjection_.matProjection * Viewport;

	//合成行列の逆行列を計算する
	Matrix4 matInverseVPV = MathUtility::Matrix4Inverse(matVPV);
	//スクリーン座標
	Vector3 posNear = Vector3(mousePosition.x, mousePosition.y, 0);
	Vector3 posFar = Vector3(mousePosition.x, mousePosition.y, 1);

	//スクリーン座標系からワールド座標系へ
	posNear = AffinTrans::wDivision(posNear, matInverseVPV);
	posFar = AffinTrans::wDivision(posFar, matInverseVPV);

	//マウスレイの方向
	Vector3 mouseDirection = posFar - posNear;
	mouseDirection = Vector3Normalize(mouseDirection);
	//カメラから照準オブジェクトの距離
	const float kDistanceTestObject = 222.0f;
	worldTransform3DReticle_.translation_ = AffinTrans::AddVector(posNear, mouseDirection * kDistanceTestObject);

	//行列更新
	AffinTrans::affin(worldTransform3DReticle_);
	worldTransform3DReticle_.TransferMatrix();

	/////////////////////////////////////////////////////////////

	debugText_->SetPos(50, 150);
	debugText_->Printf(
	  "translation : %f,%f,%f", worldTransform_.translation_.x,
	  worldTransform_.translation_.y,
	  worldTransform_.translation_.z);
	DebugText::GetInstance()->SetPos(20, 200);
	DebugText::GetInstance()->Printf("Mouse ScreenPos:(%d,%d)", mousePosition.x, mousePosition.y);
	DebugText::GetInstance()->SetPos(20, 220);
	DebugText::GetInstance()->Printf("Near:(%f,%f,%f)", posNear.x, posNear.y, posNear.z);
	DebugText::GetInstance()->SetPos(20, 240);
	DebugText::GetInstance()->Printf("Far:(%f,%f,%f)", posFar.x, posFar.y, posFar.z);
	DebugText::GetInstance()->SetPos(20, 260);
	DebugText::GetInstance()->Printf(
	  "MouseObject:(%f,%f,%f)", worldTransform3DReticle_.translation_.x,
	  worldTransform3DReticle_.translation_.y, worldTransform3DReticle_.translation_.z);


}

void Player::Draw(ViewProjection viewProjection_) { 
	model_->Draw(worldTransform_, viewProjection_, textureHandle_);
	
	//弾更新
	//複数
	for (std::unique_ptr<PlayerBullet>& bullet : bullets_) {
		bullet->Draw(viewProjection_);
	}

	//3Dレティクルを描画
	model_->Draw(worldTransform3DReticle_, viewProjection_,textureHandle_);


	//単発
	/*if (bullet_) {
		bullet_->Draw(viewProjection_);
	}*/
}

void Player::Attack() { 
	if (input_->IsTriggerMouse(0)) {
		//弾の速度
		const float kBulletSpeed = 1.0f;
		Vector3 velocity(0, 0, kBulletSpeed);

		//速度ベクトルを自機の向きに合わせて回転させる
		velocity = bVelocity(velocity, worldTransform_);

		//自機から標準オブジェクトへのベクトル
		velocity = AffinTrans::GetWorldtransform(worldTransform3DReticle_.matWorld_) - AffinTrans::GetWorldtransform(worldTransform_.matWorld_);
		velocity = Vector3Normalize(velocity) * kBulletSpeed;

		//弾を生成し初期化
		//複数
		std::unique_ptr<PlayerBullet> newBullet = std::make_unique<PlayerBullet>();

		//単発
		/*PlayerBullet* newBullet = new PlayerBullet();*/
		newBullet->Initialize(model_, AffinTrans::GetWorldtransform(worldTransform_.matWorld_), velocity);

		 //弾の登録
		//複数
		bullets_.push_back(std::move(newBullet));

		//単発
		/*bullet_.reset(newBullet);*/
	}

}

void Player::DrawUI() { 
	sprite2DReticle_->Draw();

}

Vector3 Player::bVelocity(Vector3& velocity, WorldTransform& worldTransform) { 
	
	Vector3 result = {0, 0, 0};

	
	result.x = velocity.x * worldTransform.matWorld_.m[0][0] +
	           velocity.y * worldTransform.matWorld_.m[1][0] +
	           velocity.z * worldTransform.matWorld_.m[2][0];

	result.y = velocity.x * worldTransform.matWorld_.m[0][1] +
	           velocity.y * worldTransform.matWorld_.m[1][1] +
	           velocity.z * worldTransform.matWorld_.m[2][1];

	result.z = velocity.x * worldTransform.matWorld_.m[0][2] +
	           velocity.y * worldTransform.matWorld_.m[1][2] +
	           velocity.z * worldTransform.matWorld_.m[2][2];


	return result;
}

Vector3 Player::GetWorldPosition2() {
	//ワールド座標を入れる変数
	Vector3 worldPos;
	//ワールド行列の平行移動成分
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos; 
}

void Player::OnCollision() {}

void Player::setparent(WorldTransform* worldTransform) { 
	worldTransform_.parent_ = worldTransform; 
}


