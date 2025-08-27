#pragma once
#include "CameraController.h"
#include "DeathParticles.h"
#include "Enemy.h"
#include "Fade.h"
#include "MapChipField.h"
#include "Player.h"
#include "Skydome.h"
#include "UpData.h"
#include <KamataEngine.h>

using namespace KamataEngine;

// ゲームシーン
class GameScene {
public:
	// 初期化
	void Initialize();

	// 更新
	void Update();

	// 描画
	void Draw();

	~GameScene();

	void GenerateBlocks();

	//  衝突判定と応答
	void CheckAllCollisions();

	void ChangePhase();

	// 	デスフラグのgetter
	bool IsFinished() const { return finished_; }

private:
	// ゲームのフェーズ（型）
	enum class Phase {
		kFadeIn,  // フェードイン 
		kPlay,    // ゲームプレイ
		kDeath,   // デス演出
		kFadeOut, // フェードアウト
	};

	// ゲームの現在フェーズ（変数）
	Phase phase_;

	////テクスチャーハンドル
	uint32_t textureHandle_ = 0;

	Sprite* sprite_ = nullptr;

	//////3Dモデル
	Model* model_ = nullptr;

	// ブロックの3Dモデル
	Model* blockModel_ = nullptr;

	WorldTransform worldTransform_;

	////カメラ
	Camera camera_;

	std::vector<std::vector<WorldTransform*>> worldTransformBlocks_;

	DebugCamera* debugCamera_ = nullptr;

	// 自キャラ
	Player* player_ = nullptr;

	//  エネミークラス
	Enemy* enemy_ = nullptr;

	// Math* math_ = nullptr;

	// デバッグカメラ有効
	bool isDebugCameraActive_ = false;

	Skydome* skydome_ = nullptr;

	Model* modelSkydome_ = nullptr;

	Model* modelPlayer_ = nullptr;

	Model* modelAttack_ = nullptr;

	Model* enemy_model_ = nullptr;

	MapChipField* mapChipField_;

	CameraController* CController_ = nullptr;

	std::list<Enemy*> enemies_;

	DeathParticles* deathParticles_ = nullptr;


	Model* deathParticle_model_ = nullptr;

	
	bool finished_ = false;

	
	Fade* fade_ = nullptr;

	UpData* upData = nullptr;
};
