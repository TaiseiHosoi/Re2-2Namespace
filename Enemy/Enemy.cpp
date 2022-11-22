#include "Enemy.h"
#include "Player/Player.h"
#include <GameScene.h>
#include <cmath>

void Enemy::Initialize(Model* model, uint32_t textureHandle, Vector3 vector3) {
	// NULLポインタチェック
	assert(model);
	model_ = model;
	textureHandle_ = textureHandle;

	//シングルインスタンスを取得する
	input_ = Input::GetInstance();
	debugText_ = DebugText::GetInstance();

	//ワールド変換の初期化
	worldTransform_.Initialize();

	//初期座標をセット
	worldTransform_.translation_ = vector3;

	/*Fire();*/
	Approach();
}

void Enemy::Update() {

	//敵の移動の速さ
	const float kCharacterSpeed = 0.1f;

	//行列更新
	AffinTrans::affin(worldTransform_);

	worldTransform_.TransferMatrix();

	//移動(ベクトルを加算)
	worldTransform_.translation_ += {0, 0, -kCharacterSpeed};
	

	//発射タイマーカウントダウン
	shotTimer--;

	if (shotTimer == 0) {
		//弾発射
		Fire();
		//発射タイマー初期化
		shotTimer = kFireInterval;
	}

	


	debugText_->SetPos(50, 180);
	debugText_->Printf(
	  "Enemytranslation : %f,%f,%f", worldTransform_.translation_.x, worldTransform_.translation_.y,
	  worldTransform_.translation_.z);
}

void Enemy::Draw(ViewProjection viewProjection_) {
	model_->Draw(worldTransform_, viewProjection_, textureHandle_);
	
}

void Enemy::Fire() {

	/*assert(player_);*/

	//弾の速度
	const float kBulletSpeed = -2.0f;
	Vector3 velocity(0, 0, kBulletSpeed);

	//プレイヤーのワールド座標の取得
	Vector3 playerPosition;
	playerPosition = player_->GetWorldPosition2();
	//敵のワールド座標を取得
	Vector3 enemyPosition;
	enemyPosition = GetWorldPosition();
	//差分ベクトルを求める
	Vector3 A_BVec = Vector3(
	  playerPosition.x - enemyPosition.x, playerPosition.y - enemyPosition.y,
	  playerPosition.z - enemyPosition.z);
	//ベクトル正規化
	float nomalize = sqrt(A_BVec.x * A_BVec.x + A_BVec.y * A_BVec.y + A_BVec.z * A_BVec.z) * 10;
	//ベクトルの長さを速さに合わせる
	A_BVec = Vector3(A_BVec.x / nomalize, A_BVec.y / nomalize, A_BVec.z / nomalize);


	//弾を生成し初期化
	//複数
	std::unique_ptr<EnemyBullet> newBullet = std::make_unique<EnemyBullet>();
	//単発
	/*PlayerBullet* newBullet = new PlayerBullet();*/
	newBullet->Initialize(model_, worldTransform_.translation_, A_BVec);
	
	//弾を登録する
	gameScene_->AddEnemyBullet(newBullet);
	
}

void Enemy::Approach() { 
	//発射タイマーを初期化
	shotTimer = kFireInterval;
}

Vector3 Enemy::GetWorldPosition() { 
	//ワールド座標を入れる変数
	Vector3 worldPos;
	//ワールド行列の平行移動成分
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}

void Enemy::OnCollision() { 
	isDead_ = true; 
}
