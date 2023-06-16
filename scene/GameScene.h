#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "time.h"
#include "DebugText.h"

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {

public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();



	//BG(スプライト)
	uint32_t textureHandleBG_ = 0;
	Sprite* spriteBG_ = nullptr;


	//ビュープロジェクション（共通）
	ViewProjection viewProjection_;

	//ステージ
	uint32_t textureHandleStage_ = 0;
	Model* modelStage_ = nullptr;
	WorldTransform worldTransformStage_;



	//プレイヤー
	uint32_t textureHandlePlayer = 0;
	Model* modelPlayer_ = nullptr;
	WorldTransform worldTransformPlayer_;

	//ビーム
	uint32_t texturehandleBeam_ = 0;
	Model* modelBeam = 0;
	WorldTransform worldTransformBeam_[10];


	//敵
	uint32_t textureHandleEnemy_ = 0; 
	Model* modelEnemy_ = 0;
	WorldTransform worldTransformEnemy_[10];


	//タイトル（スプライト）
	uint32_t textureHandleTitle_ = 0;
	Sprite* spriteTitle_ = nullptr;



	//タイトルエンター
	uint32_t texturehandleTitleEnter_ = 0;
	Sprite* spriteTitleEnter_ = nullptr;



	//ゲームオーバー
	uint32_t textureGameover = 0;
	Sprite* spriteGameover = nullptr;







	
	
	

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();
	void PlayerUpdate();
	
	//ビーム更新
	void BeamUpdate();


	//ビーム移動
	void BeamMove();


	//ビーム発生
	void BeamBorn();


	int beamFlag_[10] = {};

	//敵更新
	void EnemyUpdate();

	//敵移動
	void EnemyMove();

	//敵発生
	void EnemyBorm();


	//敵フラグ
	int enemyflag_[10] = {};



	//衝突判定
	void Collision();

	//衝突判定（プレイヤーと敵）
	void CollisionPlayerEnemy();
	
	//衝突判定（ビームと敵）
	void CollisionBeamEnemy();

	//ゲームスコア
	int gameScore_ = 0;

	//プレイヤーライフ
	int playerLife = 3;


	//ゲームプレイ更新
	void GamePlayUpdate();

	//ゲームプレイ3Ð表示
	void GamePlayDraw3D();

	//ゲームプレイ背景２D表示
	void GamePlayDraw2DBack();

	//ゲームプレイ近景2D表示
	void GamePlayDraw2DNear();


	int sceneMode_ = 1;


	//タイトル更新
	void TitleUpdate();
	//タイトル２D
	void TitleDraw2DNear();


	int gameTimer_ = 0;

	//ゲームオーバー２D
	void GameOverDraw2DNear();

	void GamePlayStart();

	//ビーム発射タイマー
	int beamTimer_ = 0;

	//敵スピード
	float enemySpeed_[10] = {};








	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;
	DebugText* debugText_ = nullptr;

	/// <summary>
	/// ゲームシーン用
	/// </summary>
};
