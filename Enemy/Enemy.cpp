#include "Enemy.h"
#include "DebugText.h"
#include "Input.h"

void Enemy::Initialize(Model* model, uint32_t& textureHandle)
{

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
	worldTransform_.translation_ = { 2.0f, 1.0f, -20.0f };

	InitApproach();



}

void Enemy::Update()
{



	//アフィン行列計算
	Affin::UpdateRotateY(affinRotate, worldTransform_);
	Affin::UpdateTrans(affinTrans, worldTransform_);
	Affin::UpdateMatrixWorld(affinScale, affinTrans, affinRotate, worldTransform_);

	//アフィン行列転送
	worldTransform_.TransferMatrix();

	//敵の動きをswitchで切り替え
	switch (phase_) {
	case Phase::Approach:

	default:
		Approach();
		break;

	case Phase::Leave:
		Leave();
		break;
	}

	//弾更新
	for (std::unique_ptr<EnemyBullet>& bullet : bullets_) {
		bullet->Update();
	}

}

void Enemy::Draw(ViewProjection viewProjection)
{
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
	//弾描画
	for (std::unique_ptr<EnemyBullet>& bullet : bullets_) {
		bullet->Draw(viewProjection);
	}
}

//敵の挙動something
void Enemy::Approach() {

	BulletClean();

	//発射タイマーのカウントダウン
	bFireTimer--;
	//指定時間に達した
	if (bFireTimer <= 0) {
		//弾を発射
		Fire();
		//発射タイマー初期化
		bFireTimer = kFireInterval;
	}

	/////////////////↑弾の発射↑/////////////////

	//移動ベクトル
	worldTransform_.translation_ += Vector3(0, 0, +kEnemyPhaseCharacterSpeed);

	//既定の位置に到達したら離脱
	if (worldTransform_.translation_.z > 30.0f) {
		phase_ = Phase::Leave;
	}
}

void Enemy::InitApproach()
{
	bFireTimer = kFireInterval;
}

void Enemy::Leave() {
	BulletClean();

	//移動(ベクトルを加算)
	worldTransform_.translation_ += Vector3(0, 0, -kEnemyPhaseCharacterSpeed);

	//既定の位置に到達したら離脱
	if (worldTransform_.translation_.z < -30.0f) {
		phase_ = Phase::Approach;
	}
}

void Enemy::Fire() {

	const float kBulletSpeed = 1.0f;
	//Vector3 velocity(0, 0, -kBulletSpeed);

	//速度ベクトルを自機の向きに回転させる
	//Affin::VectorUpdate(velocity, worldTransform_);

	Vector3 PlayerVec = player_->GetWorldPosition();
	Vector3 EnemyVec = GetWorldPosition();

	Vector3 Vec = Vector3(PlayerVec.x - EnemyVec.x, PlayerVec.y - EnemyVec.y, PlayerVec.z - EnemyVec.z);	//ヴェクトルの引き算
	Vector3 normalizeVec = MathUtility::Vector3Normalize(Vec);	//正規化
	//スピードは正規化した値のまま(1.0f)



	//弾を生成し初期化
	std::unique_ptr<EnemyBullet> newBullet = std::make_unique<EnemyBullet>();
	newBullet->Initialize(model_, worldTransform_.translation_, normalizeVec);

	//弾を登録
	bullets_.push_back(std::move(newBullet));

}

void Enemy::BulletClean()
{
	//デスフラグが立った弾を削除
	bullets_.remove_if([](std::unique_ptr<EnemyBullet>& bullet) {
		return bullet->IsDead();
		});
}

Vector3 Enemy::GetWorldPosition()
{
	//ワールド座標を入れる変数
	Vector3 worldPos;

	//ワールド行列移動成分を取得(ワールド座標)
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;

}

Matrix4 Enemy::GetMatrix()
{
	return worldTransform_.matWorld_;
}

void Enemy::OnCollision()
{
}
