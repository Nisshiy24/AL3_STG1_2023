#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include "MathUtilityForText.h"
 


//コンストラクタ
GameScene::GameScene() {  }

//デストラクタ
GameScene::~GameScene() { 
	delete spriteBG_;
	delete modelStage_;
	delete modelPlayer_;
}

//初期化
void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	//BG(2Dスプライト)
	textureHandleBG_ = TextureManager::Load("bg.jpg");
	spriteBG_ = Sprite::Create(textureHandleBG_, {0, 0});

	viewProjection_.Initialize();

	textureHandleStage_ = TextureManager::Load("stage.jpg");
	modelStage_ = Model::Create();
	worldTransformStage_.Initialize();






	//ビュープロジェクションの初期化
	viewProjection_.translation_.y = 1;
	viewProjection_.translation_.z = -6;
	viewProjection_.Initialize();





	//ステージの位置を変更
	worldTransformStage_.translation_ = {0, -1.5f, 0};
	worldTransformStage_.scale_ = {4.5f, 1, 40};

	//変換行列を更新
	worldTransformStage_.matWorld_ = MakeAffineMatrix(
	    worldTransformStage_.scale_, worldTransformStage_.rotation_,
	    worldTransformStage_.translation_);

	//変換行列を定数バッファに転送
	worldTransformStage_.TransferMatrix();



	//プレイヤー
	textureHandlePlayer = TextureManager::Load("player.png");
	modelPlayer_ = Model::Create();
	worldTransformPlayer_.scale_ = {
	    0.5f,
	    0.5f,
	    0.5f,
	};
	worldTransformPlayer_.Initialize();


	
	
	
		texturehandleBeam_ = TextureManager::Load("beam.png");
	
		modelBeam = Model::Create();
		worldTransformBeam_.scale_ = {
		    0.3f,
		    0.3f,
		    0.3f,
		};
	
		worldTransformBeam_.Initialize();

		
		textureHandleEnemy_ = TextureManager::Load("enemy.png");
	    modelEnemy_ = Model::Create();
	    worldTransformEnemy_.scale_ = {
	        0.5f,
	        0.5f,
	        0.5f,
	    };
	
		worldTransformEnemy_.Initialize();


		srand((unsigned int)time(NULL));




		//デバッグテキスト
	    debugText_ = DebugText::GetInstance();
	    debugText_->Initialize();




}
//更新
void GameScene::Update() {
	   
	

	//各シーンの更新処理を呼び出す
	switch (sceneMode_)
	{
	case 0:
		//ゲームプレイ更新
		GamePlayUpdate();
		break;
	}

}

//プレイヤー更新
void GameScene::PlayerUpdate() {
	worldTransformPlayer_.matWorld_ = MakeAffineMatrix(
	    worldTransformPlayer_.scale_, worldTransformPlayer_.rotation_,
	    worldTransformPlayer_.translation_);

	worldTransformPlayer_.TransferMatrix();


	//右移動
	if (input_->PushKey(DIK_RIGHT)) {
		worldTransformPlayer_.translation_.x += 0.1f;
	}

	//左へ移動
	if (input_->PushKey(DIK_LEFT)) {
		worldTransformPlayer_.translation_.x -= 0.1f;
	}




	if (worldTransformPlayer_.translation_.x > 4)
	{
		worldTransformPlayer_.translation_.x = 4;
	}



	if (worldTransformPlayer_.translation_.x < -4) {
		worldTransformPlayer_.translation_.x = -4;
	}






	





}



	// ビーム更新
void GameScene::BeamUpdate() {

	// 移動
	BeamMove();
	
	// ビーム発生
	BeamBorn();


	worldTransformBeam_.matWorld_ = MakeAffineMatrix(
	    worldTransformBeam_.scale_, worldTransformBeam_.rotation_,
	    worldTransformBeam_.translation_);

	worldTransformBeam_.TransferMatrix();

	

}



void GameScene::BeamMove() {

	
		if (beamFlag_ == 1) {
			worldTransformBeam_.translation_.z += 0.3f;
		}
	
	worldTransformBeam_.rotation_.x += 0.1f;
	
}



void GameScene::BeamBorn() {

	if (input_->PushKey(DIK_SPACE)) {
		if (beamFlag_ == 0) {
		
			worldTransformBeam_.translation_.x = worldTransformPlayer_.translation_.x;
			worldTransformBeam_.translation_.y = worldTransformPlayer_.translation_.y;
			worldTransformBeam_.translation_.z = worldTransformPlayer_.translation_.z;

			beamFlag_ = 1;

		


		}
	}
	

	if (worldTransformBeam_.translation_.z > 40)
	{
		beamFlag_ = 0;
	}

	


}


//敵更新
void GameScene::EnemyUpdate() {

	//敵発生
	EnemyMove();

	//発生
	EnemyBorm();

		worldTransformEnemy_.matWorld_ = MakeAffineMatrix(
	    worldTransformEnemy_.scale_, worldTransformEnemy_.rotation_,
	    worldTransformEnemy_.translation_);

	worldTransformEnemy_.TransferMatrix();

}


//敵移動
void GameScene::EnemyMove() {

	worldTransformEnemy_.translation_.z -= 0.5f;

	worldTransformEnemy_.rotation_.x -= 0.1f;

}

void GameScene::EnemyBorm()
{ 
	if (enemyflag_ == 0)
	{
		worldTransformEnemy_.translation_.x = 0.0f;
		worldTransformEnemy_.translation_.y = 0.0f;
		worldTransformEnemy_.translation_.z = 40.0f;

		enemyflag_ = 1;


		// 乱数でX座標の指定
		int x = rand() % 80;

		// 80は4の10倍の2倍
		float x2 = (float)x / 10 - 4;

		// 10で割り、4を引く
		worldTransformEnemy_.translation_.x = x2;
	


	}

	
		if (worldTransformEnemy_.translation_.z < -10) {
		enemyflag_ = 0;
	}
	

}


void GameScene::Collision() {
	//衝突判定（プレイヤーと敵）
	CollisionPlayerEnemy();

	CollisionBeamEnemy();
}




//衝突判定（プレイヤーと敵）
void GameScene::CollisionPlayerEnemy() {
	//敵が存在すれば
	if (enemyflag_ == 1)
	{
		//差を求める
		float dx = abs(worldTransformPlayer_.translation_.x - worldTransformEnemy_.translation_.x);
		float dz = abs(worldTransformPlayer_.translation_.z - worldTransformEnemy_.translation_.z);

		//衝突したら
		if (dx < 1 && dz < 1)
		{
			//存在しない
			enemyflag_ = 0;
			playerLife--;
			
		}
		
	}
}



//衝突判定（ビームと敵）
void GameScene::CollisionBeamEnemy() {

	if (enemyflag_ == 1)
	{
		if (beamFlag_ == 1) {
			float x = abs(worldTransformBeam_.translation_.x - worldTransformEnemy_.translation_.x);
			float z = abs(worldTransformBeam_.translation_.z - worldTransformEnemy_.translation_.z);

			if (x < 1 && z < 1) {
				enemyflag_ = 0;
				beamFlag_ = 0;
				gameScore_++;
			}
		}

	}

}





void GameScene::GamePlayUpdate(){

	
	// プレイヤー更新
	PlayerUpdate();

	// ビーム更新
	BeamUpdate();

	// 敵更新
	EnemyUpdate();

	// 衝突判定
	Collision();



	
	
	
	
}




//ゲームプレイ表示３D
void GameScene::GamePlayDraw3D() {

	modelStage_->Draw(worldTransformStage_, viewProjection_, textureHandleStage_);

	modelPlayer_->Draw(worldTransformPlayer_, viewProjection_, textureHandlePlayer);

	if (beamFlag_ == 1) {

		modelBeam->Draw(worldTransformBeam_, viewProjection_, texturehandleBeam_);
	}

	if (enemyflag_ == 1) {
		modelBeam->Draw(worldTransformEnemy_, viewProjection_, textureHandleEnemy_);
	}

}






//ゲームプレイ表示2D背景
void GameScene::GamePlayDraw2DBack() {
	//背景
	spriteBG_->Draw();
}



//ゲームプレイ表示２D近景
void GameScene::GamePlayDraw2DNear() {
	// ゲームスコア
	char str[100];
	sprintf_s(str, "SCORE %d", gameScore_);
	debugText_->Print(str, 200, 10, 2);

	// プレイヤーHP
	sprintf_s(str, "LIFE %d", playerLife);
	debugText_->Print(str, 800, 10, 2);

}






//表示
void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	
	


	
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	
	
	switch (sceneMode_)
	{ 
	case 0:
		GamePlayDraw2DBack();
		break;

	}


	//デバックテキスト
	//debugText_->Print("AAA", 10, 10, 2);
	

	


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

	
	

	switch (sceneMode_)
	{ 
	case 0:
		GamePlayDraw3D();
		break;
		}


	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);



	switch (sceneMode_)
	{ 
	case 0:
		GamePlayDraw2DNear();
		break;
	}


	debugText_->DrawAll();

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}
