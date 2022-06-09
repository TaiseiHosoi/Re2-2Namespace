#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include "AxisIndicator.h"
#include "PrimitiveDrawer.h"
#include<random>



GameScene::GameScene() {

}

GameScene::~GameScene() {
	delete model_;
	delete debugCamera_;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugText_ = DebugText::GetInstance();

	//乱数シード生成器
	std::random_device seed_gen;
	//メルセンヌツイスター
	std::mt19937_64 engine(seed_gen());
	//乱数範囲(回転角度)
	std::uniform_real_distribution<float>rotDist(0.0f, 3.14f);
	//乱数範囲(座標用)
	std::uniform_real_distribution<float>posDist(-10.0f, 10.0f);

	//ファイル名を指定してテクスチャを読み込む
	textureHandle_ = TextureManager::Load("mario.jpg");
	//3Dモデルの生成
	model_ = Model::Create();


	Matrix4 matScale = MathUtility::Matrix4Identity();
	Matrix4 matTrans = MathUtility::Matrix4Identity();
	Matrix4 matRotate = MathUtility::Matrix4Identity();

	//100個生成
	for (size_t i = 0; i < _countof(worldTransforms_); i++) {
		
		//ワールドトランスフォーム初期化
		worldTransforms_[i].Initialize();
		//X,Y,Z方向のスケーリング
		worldTransforms_[i].scale_ = { 1.0f,1.0f,1.0f };
		//回転角設定
		worldTransforms_[i].rotation_ = { rotDist(engine),rotDist(engine),rotDist(engine) };
		//平行移動
		worldTransforms_[i].translation_ = { posDist(engine),posDist(engine),posDist(engine) };

		Affin::UpdateTrans(matTrans, worldTransforms_[i]);
		Affin::UpdateScale(matScale, worldTransforms_[i]);
		Affin::UpdateRotate(matRotate, worldTransforms_[i]);
		worldTransforms_[i].TransferMatrix();

	}

	
	
#pragma region Re2-2
	//// X,Y,Z方向トランスレーション設定
	//worldTransform_.scale_ = { 2.0f,1.0f,1.0f };

	//// X,Y,Z方向スケーリング設定
	//worldTransform_.translation_ = { 5.0f,1.0f,1.0f };

	//// X,Y,Z方向回転角設定
	//worldTransform_.rotation_ = { 0.0f,0.0f,0.5f }; 

	/*
	Matrix4 matScale = MathUtility::Matrix4Identity();
	Matrix4 matTrans = MathUtility::Matrix4Identity();
	Matrix4 matRotate = MathUtility::Matrix4Identity();*/

	//worldTransform_.matWorld_ = {	//Matrixワールド座標初期化
	//	1.0f,0.0f,0.0f,0.0f,
	//	0.0f,1.0f,0.0f,0.0f,
	//	0.0f,0.0f,1.0f,0.0f,
	//	0.0f,0.0f,0.0f,1.0f
	//};

	//worldTransform_.matWorld_ *= matScale;

	//Affin::UpdateTrans(matTrans,worldTransform_);
	//Affin::UpdateScale(matScale,worldTransform_);
	//Affin::UpdateRotate(matRotate,worldTransform_);


	//worldTransform_.TransferMatrix();

#pragma endregion 


	//カメラ注視座業設定
	viewProjection_.target = { 10.0f,0.0f,0.0f };

	//カメラ上方向ベクトルを設定
	viewProjection_.up = { cosf(3.14f / 4.0f),sinf(3.14f / 4.0f),0.0f };
	
	//ビュープロジェクションの初期化
	viewProjection_.Initialize();
	//デバッグカメラの生成
	debugCamera_ = new DebugCamera(1280, 720);
	//軸方向表示の表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);
	//軸方向表示が参照するビュープロジェクションを指定する(アドレス渡し)
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);
	//ライン描画が参照するビュープロジェクションを指定する(アドレス渡し)
	PrimitiveDrawer::GetInstance()->SetViewProjection(&viewProjection_);
}

void GameScene::Update() {
	debugCamera_->Update();
	//視点移動処理
	{
		//視点の移動ベクトル
		Vector3 move = { 0.0f,0.0f,0.0f };
		//視点の移動の速さ
		const float kEyeSpeed = 0.2f;

		//押した方向で移動ベクトルを変更
		if (input_->PushKey(DIK_W)) {
			move.z += kEyeSpeed;
		}
		else if (input_->PushKey(DIK_S)) {
			move.z -= kEyeSpeed;
		}

		//視点移動
		viewProjection_.eye += move;

		//行列の再計算
		viewProjection_.UpdateMatrix();

		//デバッグ用表示
		debugText_->SetPos(50, 50);
		debugText_->Printf(
			"eye:(%f,%f,%f)", viewProjection_.eye.x, viewProjection_.eye.y, viewProjection_.eye.z);


	}
	//注視店移動処理
	{

		//中止・の移動ベクトル
		Vector3 move = { 0.0f,0.0f,0.0f };

		const float kTargetSpeed = 0.2f;

		//キー処理
		if (input_->PushKey(DIK_LEFT)) {
			move.x += kTargetSpeed;
		}
		else if (input_->PushKey(DIK_RIGHT)) {
			move.x -= kTargetSpeed;
		}

		//注視・移動
		viewProjection_.target += move;

		//行列の再計算
		viewProjection_.UpdateMatrix();

		debugText_->SetPos(50, 70);
		debugText_->Printf(
			"target:(%f,%f,%f)", viewProjection_.target.x, viewProjection_.target.y, viewProjection_.target.z);
	}

	//上方向回転処理
	{
		//上方向の回転速さ
		const float kUpRotSpeed = 0.05f;

		//押した方向で移動ベクトル変更
		if (input_->PushKey(DIK_SPACE)) {
			viewAngle += kUpRotSpeed;
			//2π超えたら0に戻す
			viewAngle = fmodf(viewAngle, 3.14f * 2.0f);

		}

		//上方向ベクトルを計算
		viewProjection_.up = { cosf(viewAngle),sinf(viewAngle),0.0f };

		viewProjection_.UpdateMatrix();
		debugText_->SetPos(50, 90);
		debugText_->Printf(
			"up:(%f,%f,%f)", viewProjection_.up.x, viewProjection_.up.y, viewProjection_.up.z);
	}
}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// 
	

	/// </summary>
	//model_->Draw(worldTransforms_, debugCamera_->GetViewProjection(), textureHandle_);

	for (size_t i = 0; i < _countof(worldTransforms_); i++) {
		model_->Draw(worldTransforms_[i],viewProjection_, textureHandle_);
	}

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// デバッグテキストの描画
	debugText_->DrawAll(commandList);
	//
	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
	//ライン描画が参照するビュープロジェクションを指定する(アドレス渡し)
	PrimitiveDrawer::GetInstance()->DrawLine3d(lineStart(-100, 0, -100), lineFinish(100, 0, 100), lineColor(255, 255, 255, 100));
}

Vector3 GameScene::lineStart(int x, int y, int z)
{
	return Vector3(x, y, z);
}

Vector3 GameScene::lineFinish(int x, int y, int z)
{
	return Vector3(x, y, z);
}

Vector4 GameScene::lineColor(int x, int y, int z, int w)
{
	return Vector4(x, y, z, w);
}