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
	delete spriteTitle_;
	delete spriteTitleEnter_;
	delete spriteGameover;

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
	
		for (int i = 0; i < 10; i++) {

		    modelBeam = Model::Create();
		    worldTransformBeam_[i].scale_ = {
		        0.3f,
		        0.3f,
		        0.3f,
		    };

		    worldTransformBeam_[i].Initialize();
	    }
		
		textureHandleEnemy_ = TextureManager::Load("enemy.png");
	    modelEnemy_ = Model::Create();
		//敵の数ループする
	    for (int i = 0; i < 10; i++) {
		    worldTransformEnemy_[i].scale_ = {
		        0.5f,
		        0.5f,
		        0.5f,
		    };

		    worldTransformEnemy_[i].Initialize();
	    }


		srand((unsigned int)time(NULL));




		//デバッグテキスト
	    debugText_ = DebugText::GetInstance();
	    debugText_->Initialize();



		//タイトル（2Dスプライト）
	    textureHandleTitle_ = TextureManager::Load("title.png");
	    spriteTitle_ = Sprite::Create(textureHandleTitle_, {0, 0});


		//タイトルエンター
	    texturehandleTitleEnter_ = TextureManager::Load("enter.png");
	    spriteTitleEnter_ = Sprite::Create(texturehandleTitleEnter_, {400, 400});


		//ゲームオーバー
	    textureGameover = TextureManager::Load("gameover.png");
	    spriteGameover = Sprite::Create(textureGameover, {0, 0});









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
	case 1:
		
		//タイトル更新
		TitleUpdate();
		gameTimer_ += 1;
		break;
	case 2:
		gameTimer_ += 1;
		break;
	
	}

}

//プレイヤー更新
void GameScene::PlayerUpdate() {
	
	



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




	worldTransformPlayer_.matWorld_ = MakeAffineMatrix(
	    worldTransformPlayer_.scale_, worldTransformPlayer_.rotation_,
	    worldTransformPlayer_.translation_);

	worldTransformPlayer_.TransferMatrix();

	





}



	// ビーム更新
void GameScene::BeamUpdate() {

	// 移動
	BeamMove();
	
	// ビーム発生
	BeamBorn();

	for (int i = 0; i < 10; i++) {
		worldTransformBeam_[i].matWorld_ = MakeAffineMatrix(
		    worldTransformBeam_[i].scale_, worldTransformBeam_[i].rotation_,
		    worldTransformBeam_[i].translation_);

		worldTransformBeam_[i].TransferMatrix();
	}
	

}



void GameScene::BeamMove() {

	for (int i = 0; i < 10; i++) {

		if (beamFlag_[i] == 1) {
			worldTransformBeam_[i].translation_.z += 0.3f;
		}

		worldTransformBeam_[i].rotation_.x += 0.1f;
	}
	
}



void GameScene::BeamBorn() {

	if (input_->PushKey(DIK_SPACE)) {
		for (int i = 0; i < 10; i++) {
			if (beamFlag_[i] == 0) {

				worldTransformBeam_[i].translation_.x = worldTransformPlayer_.translation_.x;
				worldTransformBeam_[i].translation_.y = worldTransformPlayer_.translation_.y;
				worldTransformBeam_[i].translation_.z = worldTransformPlayer_.translation_.z;

				// 発射タイマーが1ならば
				if (beamTimer_ == 0) {
					beamFlag_[i] = 1;
					beamTimer_ = 1;
					break;
				} else {
					//発射タイマーを1以上
					//10を超えると再び発射が可能
					beamTimer_++;
					if (beamTimer_ > 10) {
						beamTimer_ = 0;
					
					}
				}
			}
		}
	}
	
	for (int i = 0; i < 10; i++) {
		if (worldTransformBeam_[i].translation_.z > 40) {
			beamFlag_[i] = 0;
		}
	}

	
	
	

	


}


//敵更新
void GameScene::EnemyUpdate() {

	//敵発生
	EnemyMove();

	//発生
	EnemyBorm();

	for (int i = 0; i < 10; i++) {
		worldTransformEnemy_[i].matWorld_ = MakeAffineMatrix(
		    worldTransformEnemy_[i].scale_, worldTransformEnemy_[i].rotation_,
		    worldTransformEnemy_[i].translation_);

		worldTransformEnemy_[i].TransferMatrix();
	}

}


//敵移動
void GameScene::EnemyMove() {

	for (int i = 0; i < 10; i++) {
		worldTransformEnemy_[i].translation_.z -= 0.5f;

		worldTransformEnemy_[i].rotation_.x -= 0.1f;

		worldTransformEnemy_[i].translation_.x += enemySpeed_[i];

		if (worldTransformEnemy_[i].translation_.x > 4)
		{
			enemySpeed_[i] = -0.1f;
		}

		if (worldTransformEnemy_[i].translation_.x < -4)
		{
			enemySpeed_[i] = 0.1f;
		}

	}

}

void GameScene::EnemyBorm()
{ 
	if (rand() % 10 == 0) {
		for (int i = 0; i < 10; i++) {
			if (enemyflag_[i] == 0) {

				worldTransformEnemy_[i].translation_.x = 0.0f;
				worldTransformEnemy_[i].translation_.y = 0.0f;
				worldTransformEnemy_[i].translation_.z = 40.0f;

				enemyflag_[i] = 1;

				// 乱数でX座標の指定
				int x = rand() % 80;

				// 80は4の10倍の2倍
				float x2 = (float)x / 10 - 4;

				// 10で割り、4を引く
				worldTransformEnemy_[i].translation_.x = x2;

				//敵のスピード
				if (rand() % 2 == 0) {
					enemySpeed_[i] = 0.1f;
				} else {
					enemySpeed_[i] = -0.1f;
				}

				break;
			}

			if (worldTransformEnemy_[i].translation_.z < -10) {
				enemyflag_[i] = 0;
			}
		}
	}
}


void GameScene::Collision() {
	//衝突判定（プレイヤーと敵）
	CollisionPlayerEnemy();

	CollisionBeamEnemy();
}




//衝突判定（プレイヤーと敵）
void GameScene::CollisionPlayerEnemy() {
	
	for (int i = 0; i < 10; i++) {
		// 敵が存在すれば
		if (enemyflag_[i] == 1) {
			// 差を求める
			float dx =
			    abs(worldTransformPlayer_.translation_.x - worldTransformEnemy_[i].translation_.x);
			float dz =
			    abs(worldTransformPlayer_.translation_.z - worldTransformEnemy_[i].translation_.z);

			// 衝突したら
			if (dx < 1 && dz < 1) {
				// 存在しない
				enemyflag_[i] = 0;
				playerLife--;
			}
		}
	}

}



//衝突判定（ビームと敵）
void GameScene::CollisionBeamEnemy() {


	for (int e = 0; e < 10; e++) {

		if (enemyflag_[e] == 1) {

			for (int b = 0; b < 10; b++)
				{
				if (beamFlag_[b] == 1) {
					float x =
					    abs(worldTransformBeam_[b].translation_.x -
					        worldTransformEnemy_[e].translation_.x);
					float z =
					    abs(worldTransformBeam_[b].translation_.z -
					        worldTransformEnemy_[e].translation_.z);

					if (x < 1 && z < 1) {
						enemyflag_[e] = 0;
						beamFlag_[b] = 0;
						gameScore_+= 1;
					}
				}
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

	

	for (int i = 0; i < 10; i++) {
		if (beamFlag_[i] == 1) {

			modelBeam->Draw(worldTransformBeam_[i], viewProjection_, texturehandleBeam_);
		}
	}

	for (int i = 0; i < 10; i++) {
		if (enemyflag_[i] == 1) {
			modelBeam->Draw(worldTransformEnemy_[i], viewProjection_, textureHandleEnemy_);
		}
	}
	



	if (playerLife == 0) {
		sceneMode_ = 2;
	}

	modelStage_->Draw(worldTransformStage_, viewProjection_, textureHandleStage_);

	modelPlayer_->Draw(worldTransformPlayer_, viewProjection_, textureHandlePlayer);

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



//タイトル更新
void GameScene::TitleUpdate()
{
	
		if (input_->TriggerKey(DIK_RETURN)) {
			// モードをゲームプレイへ変更
			sceneMode_ = 0;
		    GamePlayStart();
		}
	


	

	

}


//タイトル表示
void GameScene::TitleDraw2DNear() {

	//タイトル表示
	spriteTitle_->Draw();


	
	if (gameTimer_ % 40 >= 20) {
		// タイトルエンター表示
		spriteTitleEnter_->Draw();
	}

}



//ゲームオーバー表示
void GameScene::GameOverDraw2DNear() {
	
	

		spriteGameover->Draw();

		if (sceneMode_ == 2) {
		if (input_->TriggerKey(DIK_RETURN)) {
			sceneMode_ = 1;
		}
	    }


		

	    if (gameTimer_ % 40 >= 20) {
		// タイトルエンター表示
		spriteTitleEnter_->Draw();
	    }
	

}



void GameScene::GamePlayStart() {

	playerLife = 3;
	gameScore_ = 0;
	worldTransformPlayer_.translation_.x = 0.0f;
	for (int i = 0; i < 10; i++)
	{
		enemyflag_[i] = 0;
		beamFlag_[i] = 0;
	}

	gameTimer_ = 0;

	GamePlayUpdate();


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
	case 2:
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
	case 2:
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
	case 1:
		TitleDraw2DNear();
		break;
	case 2:
		GamePlayDraw2DNear();
		GameOverDraw2DNear();
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
