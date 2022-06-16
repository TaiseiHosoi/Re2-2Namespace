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
	//std::random_device seed_gen;
	////メルセンヌツイスター
	//std::mt19937_64 engine(seed_gen());
	////乱数範囲(回転角度)
	//std::uniform_real_distribution<float>rotDist(0.0f, 3.14f);
	////乱数範囲(座標用)
	//std::uniform_real_distribution<float>posDist(-10.0f, 10.0f);



	//ファイル名を指定してテクスチャを読み込む
	textureHandle_ = TextureManager::Load("mario.jpg");
	//3Dモデルの生成
	model_ = Model::Create();
	
	
	matScale = MathUtility::Matrix4Identity();
	for (int i = 0; i < 10; i++) {
		
		matTrans[i] = MathUtility::Matrix4Identity();
		matRotate[i] = MathUtility::Matrix4Identity();
	}
	//大元から脊髄
	worldTransforms_[PartId::Root].Initialize();
	//子
	worldTransforms_[PartId::Spine].translation_ = { 0, 4, 0 };
	worldTransforms_[PartId::Spine].parent_ = &worldTransforms_[PartId::Root];
	worldTransforms_[PartId::Spine].Initialize();

	//上半身
	worldTransforms_[PartId::Chest].translation_ = { 0, -4, 0 };
	worldTransforms_[PartId::Chest].parent_ = &worldTransforms_[PartId::Spine];
	worldTransforms_[PartId::Chest].Initialize();

	worldTransforms_[PartId::Head].translation_ = { 0, -4, 0 };
	worldTransforms_[PartId::Head].parent_ = &worldTransforms_[PartId::Chest];
	worldTransforms_[PartId::Head].Initialize();

	worldTransforms_[PartId::ArmL].translation_ = { -5, -4, 0 };
	worldTransforms_[PartId::ArmL].parent_ = &worldTransforms_[PartId::Chest];
	worldTransforms_[PartId::ArmL].Initialize();

	worldTransforms_[PartId::ArmR].translation_ = { 5, -4, 0 };
	worldTransforms_[PartId::ArmR].parent_ = &worldTransforms_[PartId::Chest];
	worldTransforms_[PartId::ArmR].Initialize();

	//下半身
	worldTransforms_[PartId::Hip].translation_ = { 0, -14, 0 };
	worldTransforms_[PartId::Hip].parent_ = &worldTransforms_[PartId::Spine];
	worldTransforms_[PartId::Hip].Initialize();

	worldTransforms_[PartId::LegL].translation_ = { -5, -4, 0 };
	worldTransforms_[PartId::LegL].parent_ = &worldTransforms_[PartId::Hip];
	worldTransforms_[PartId::LegL].Initialize();

	worldTransforms_[PartId::LegR].translation_ = { 5, -4, 0 };
	worldTransforms_[PartId::LegR].parent_ = &worldTransforms_[PartId::Hip];
	worldTransforms_[PartId::LegR].Initialize();

	for (int i = 0; i < 10; i++) {

		
		worldTransforms_[i].matWorld_ = {	//Matrixワールド座標初期化
			1.0f,0.0f,0.0f,0.0f,
			0.0f,1.0f,0.0f,0.0f,
			0.0f,0.0f,1.0f,0.0f,
			0.0f,0.0f,0.0f,1.0f
		};



	}
	




	////100個生成
	//for (size_t i = 0; i < _countof(worldTransforms_); i++) {
	//	
	//	//ワールドトランスフォーム初期化
	//	worldTransforms_[i].Initialize();
	//	//X,Y,Z方向のスケーリング
	//	worldTransforms_[i].scale_ = { 1.0f,1.0f,1.0f };
	//	//回転角設定
	//	worldTransforms_[i].rotation_ = { rotDist(engine),rotDist(engine),rotDist(engine) };
	//	//平行移動
	//	worldTransforms_[i].translation_ = { posDist(engine),posDist(engine),posDist(engine) };

	//	Affin::UpdateTrans(matTrans, worldTransforms_[i]);
	//	Affin::UpdateScale(matScale, worldTransforms_[i]);
	//	Affin::UpdateRotate(matRotate, worldTransforms_[i]);
	//	worldTransforms_[i].TransferMatrix();
	//}



#pragma region Re2-2
	//// X,Y,Z方向トランスレーション設定
	//worldTransform_.scale_ = { 2.0f,1.0f,1.0f };

	//// X,Y,Z方向スケーリング設定
	//worldTransform_.translation_ = { 5.0f,1.0f,1.0f };

	//// X,Y,Z方向回転角設定
	//worldTransform_.rotation_ = { 0.0f,0.0f,0.5f }; 






	/*Affin::UpdateTrans(matTrans,worldTransform_);
	Affin::UpdateScale(matScale,worldTransform_);
	Affin::UpdateRotate(matRotate,worldTransform_);


	worldTransform_.TransferMatrix();*/

#pragma endregion 


	////カメラ注視座業設定
	//viewProjection_.target = { 10.0f,0.0f,0.0f };

	//カメラ上方向ベクトルを設定
	viewProjection_.up = { cosf(3.14f / 4.0f),sinf(3.14f / 4.0f),0.0f };

	////アスペ
	//viewProjection_.aspectRatio = 1.0f;

	////ニアクリップ距離を設定	
	//viewProjection_.nearZ = 52.0f;

	////ファークリップの距離を設定
	//viewProjection_.farZ = 58.0f;

	//カメラ垂直方向視野角を設定
	viewProjection_.fovAngleY = 3.14f / 180.0f * 50.0f;

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

	
#pragma region 視点移動処理
	////視点移動処理
	//{
	//	//視点の移動ベクトル
	//	Vector3 move = { 0.0f,0.0f,0.0f };
	//	//視点の移動の速さ
	//	const float kEyeSpeed = 0.2f;

	//	//押した方向で移動ベクトルを変更
	//	if (input_->PushKey(DIK_W)) {
	//		move.z += kEyeSpeed;
	//	}
	//	else if (input_->PushKey(DIK_S)) {
	//		move.z -= kEyeSpeed;
	//	}

	//	//クリップ距離変更処理
	//	{
	//		//上下キーでニアクリップ距離を増減
	//		if (input_->PushKey(DIK_UP)) {
	//			viewProjection_.nearZ += 0.1f;
	//		}
	//		else if (input_->PushKey(DIK_DOWN)) {
	//			viewProjection_.nearZ -= 0.1f;
	//		}

	//		//行列の再計算
	//		viewProjection_.UpdateMatrix();

	//		//デバッグ用表示
	//		debugText_->SetPos(50, 130);
	//		debugText_->Printf("nearZ:%f", viewProjection_.nearZ);
	//	}

	//	//視点移動
	//	viewProjection_.eye += move;

	//	//行列の再計算
	//	viewProjection_.UpdateMatrix();

	//	//デバッグ用表示
	//	debugText_->SetPos(50, 50);
	//	debugText_->Printf(
	//		"eye:(%f,%f,%f)", viewProjection_.eye.x, viewProjection_.eye.y, viewProjection_.eye.z);


	//}
	////注視店移動処理
	//{

	//	//中止・の移動ベクトル
	//	Vector3 move = { 0.0f,0.0f,0.0f };

	//	const float kTargetSpeed = 0.2f;

	//	//キー処理
	//	if (input_->PushKey(DIK_LEFT)) {
	//		move.x += kTargetSpeed;
	//	}
	//	else if (input_->PushKey(DIK_RIGHT)) {
	//		move.x -= kTargetSpeed;
	//	}

	//	//注視・移動
	//	viewProjection_.target += move;

	//	//行列の再計算
	//	viewProjection_.UpdateMatrix();

	//	debugText_->SetPos(50, 70);
	//	debugText_->Printf(
	//		"target:(%f,%f,%f)", viewProjection_.target.x, viewProjection_.target.y, viewProjection_.target.z);
	//}
#pragma endregion 視点移動処理

		//上方向の回転速さ
		const float kUpRotSpeed = 0.05f;

		//押した方向で移動ベクトル変更
		if (input_->PushKey(DIK_SPACE)) {
			viewAngle += kUpRotSpeed;
			//2π超えたら0に戻す
			viewAngle = fmodf(viewAngle, 3.14f * 2.0f);

		}

		//上方向ベクトルを計算
		viewProjection_.up = { 0.0f,1.0f,0.0f };

		viewProjection_.UpdateMatrix();
		debugText_->SetPos(50, 90);
		debugText_->Printf(
			"up:(%f,%f,%f)", viewProjection_.up.x, viewProjection_.up.y, viewProjection_.up.z);
	
	//関数用行列初期化
	for (int i = 0; i < 9; i++) {
		matTrans[i] = MathUtility::Matrix4Identity();
		matRotate[i] = MathUtility::Matrix4Identity();
	}

	//キャラクター移動処理
	Vector3 move = { 0, 0, 0 };
	//キャラクターの移動の速さ
	const float kCharacterSpeed = 0.2f;

	//押した方向で移動ベクトルを変更
	if (input_->PushKey(DIK_LEFT)) {
		move = { -kCharacterSpeed, 0, 0 };
	}
	else if (input_->PushKey(DIK_RIGHT)) {
		move = { kCharacterSpeed, 0, 0 };
	}

	//注視点(ベクトルの加算)
	worldTransforms_[PartId::Root].translation_.x += move.x;
	worldTransforms_[PartId::Root].translation_.y += move.y;

		

		//上半身の回転処理
		//上半身の回転の速さ[ラジアン/frame]
		const float kChestRotSpeed = 0.05f;

		//押した方向で移動ベクトルを変更
		if (input_->PushKey(DIK_U)) {
			worldTransforms_[PartId::Chest].rotation_.y -= kChestRotSpeed;
		}
		else if (input_->PushKey(DIK_I)) {
			worldTransforms_[PartId::Chest].rotation_.y += kChestRotSpeed;
		}


			//速さ
			const float kHipRotSpeed = 0.05f;

			//
			if (input_->PushKey(DIK_J)) {
				worldTransforms_[PartId::Hip].rotation_.y -= kHipRotSpeed;

			}
			else if (input_->PushKey(DIK_K)) {
				worldTransforms_[PartId::Hip].rotation_.y += kHipRotSpeed;

			}
			


		

		//行列の更新
		
			for (int i = 0; i < 9; i++) {
				Affin::UpdateTrans(matTrans[i], worldTransforms_[i]);
				Affin::UpdateRotateY(matRotate[i], worldTransforms_[i]);
				Affin::UpdateMatrixWorld(matTrans[i], matRotate[i], worldTransforms_[i]);
				if (i != 0) {
					worldTransforms_[i].matWorld_ *= worldTransforms_[i].parent_->matWorld_;

				}
				

			}
			
		
		////注視点移動(ベクトルの加算)
		//worldTransforms_[PartId::Root].translation_.x += move.x;
		//worldTransforms_[PartId::Root].translation_.y += move.y;
		//worldTransforms_[PartId::Root].translation_.z += move.z;




		


		worldTransforms_[PartId::Root].TransferMatrix();
		worldTransforms_[PartId::Spine].TransferMatrix();
		worldTransforms_[PartId::Chest].TransferMatrix();
		worldTransforms_[PartId::Head].TransferMatrix();
		worldTransforms_[PartId::ArmL].TransferMatrix();
		worldTransforms_[PartId::ArmR].TransferMatrix();
		worldTransforms_[PartId::Hip].TransferMatrix();
		worldTransforms_[PartId::LegL].TransferMatrix();
		worldTransforms_[PartId::LegR].TransferMatrix();




		//デバッグ用表示
		debugText_->SetPos(50, 150);
		debugText_->Printf(
			"Root:(%f,%f,%f)", worldTransforms_[PartId::Root].translation_.x,
			worldTransforms_[PartId::Root].translation_.y,
			worldTransforms_[PartId::Root].translation_.z);

		

	

	//Fov変換処理
	
		////上キーの視野角が広がる
		//if (input_->PushKey(DIK_UP)) {
		//	viewProjection_.fovAngleY += 0.02f;
		//	viewProjection_.fovAngleY = min(viewProjection_.fovAngleY, 3.14f);
		//}
		//else if (input_->PushKey(DIK_DOWN)) {
		//	viewProjection_.fovAngleY -= 0.02f;
		//	viewProjection_.fovAngleY = max(viewProjection_.fovAngleY, 0.01f);
		//}

		//デバッグ用表示
		debugText_->SetPos(50, 110);
		debugText_->Printf("fovAngle(Deg):%f", 180.0f / 3.14f * viewProjection_.fovAngleY);


		//デバッグ用表示
		debugText_->SetPos(50, 200);
		debugText_->Printf("posChestX:%f",worldTransforms_[Chest].translation_.x);

		//デバッグ用表示
		debugText_->SetPos(50, 240);
		debugText_->Printf("hoge:%f", matTrans[Root].m[1][3] );

	
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

	/*for (size_t i = 0; i < _countof(worldTransforms_); i++) {
		model_->Draw(worldTransforms_[i],viewProjection_, textureHandle_);
	}*/



	model_->Draw(worldTransforms_[PartId::Chest], viewProjection_, textureHandle_);
	model_->Draw(worldTransforms_[PartId::Head], viewProjection_, textureHandle_);
	model_->Draw(worldTransforms_[PartId::ArmL], viewProjection_, textureHandle_);
	model_->Draw(worldTransforms_[PartId::ArmR], viewProjection_, textureHandle_);
	model_->Draw(worldTransforms_[PartId::Hip], viewProjection_, textureHandle_);
	model_->Draw(worldTransforms_[PartId::LegL], viewProjection_, textureHandle_);
	model_->Draw(worldTransforms_[PartId::LegR], viewProjection_, textureHandle_);

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