#include "GameScene.h"

using namespace KamataEngine;

GameScene::~GameScene() {
	delete sprite_;

	delete debugCamera_;

	delete model_;

	delete blockModel_;
	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			delete worldTransformBlock;
		}
	}

	worldTransformBlocks_.clear();

	delete skydome_;

	delete modelSkydome_;

	delete mapChipField_;

	// 02_10 6枚目 敵クラス削除
	for (Enemy* enemy : enemies_) {
		delete enemy;
	}

	// 02_11_17枚目
	delete deathParticles_;
	delete deathParticle_model_;
}

void GameScene::Initialize() {
	// ここにインゲームの初期化処理を書く
	// textureHandle_ = TextureManager::Load("player.png");

	////スプライトインスタンスの生成
	sprite_ = Sprite::Create(textureHandle_, {100, 50});

	model_ = Model::Create();

	worldTransform_.Initialize();

	// カメラの初期化
	camera_.Initialize();

	blockModel_ = Model::CreateFromOBJ("block");

	debugCamera_ = new DebugCamera(WinApp::kWindowWidth, WinApp::kWindowHeight);

	camera_.farZ = 1000.0f;

	// 02_03天球
	// skydome生成
	skydome_ = new Skydome();
	// 初期化
	modelSkydome_ = Model::CreateFromOBJ("skyDome", true);

	skydome_->Initialize(modelSkydome_, &camera_);

	mapChipField_ = new MapChipField;

	mapChipField_->LoadMapChipCsv("Resources/blocks.csv");

	GenerateBlocks();

	// 自キャラの生成
	player_ = new Player();

	modelPlayer_ = Model::CreateFromOBJ("player", true);

	modelAttack_ = Model::CreateFromOBJ("attack_effect", true);

	// 座標をマップチップ番号で指定
	Vector3 playerPosition = mapChipField_->GetMapChipPositionByIndex(2, 18);

	player_->SetMapChipField(mapChipField_);

	// 自キャラの初期化
	player_->Initialize(modelPlayer_, modelAttack_, &camera_, playerPosition);

	CController_ = new CameraController(); // 生成

	CController_->Initialize(&camera_); // 初期化

	CController_->SetTarget(player_); // 追従対象セット

	CController_->Reset(); // リセット

	CameraController::Rect cameraArea = {12.0f, 100 - 12.0f, 6.0f, 6.0f};
	CController_->SetMovableArea(cameraArea);

	// マップチップフィールドの生成と初期化
	// 自キャラの生成と初期化
	//  02_07 スライド5枚目

	// 02_09 10枚目 敵クラス
	// enemy_ = new Enemy();
	// 02_09 10枚目 敵モデル
	enemy_model_ = Model::CreateFromOBJ("enemy");
	// 02_09 10枚目 敵位置決めて敵クラス初期化
	// Vector3 enemyPosition = mapChipField_->GetMapChipPositionByIndex(14, 18);
	// enemy_->Initialize(enemy_model_, &camera_, enemyPosition);

	// 02_10 5枚目（for文の中身全部）
	for (int32_t i = 0; i < 6; ++i) {
		Enemy* newEnemy = new Enemy();

		Vector3 enemyPosition = mapChipField_->GetMapChipPositionByIndex(14 + i * 7, 18);

		newEnemy->Initialize(enemy_model_, &camera_, enemyPosition);

		enemies_.push_back(newEnemy);
	}

	// 02_11_16枚目 モデル読み込み
	deathParticle_model_ = Model::CreateFromOBJ("deathParticle");

	//// 02_11_16枚目 仮の生成処理 後で消す
	// deathParticles_ = new DeathParticles;
	// deathParticles_->Initialize(deathParticle_model_, &camera_, playerPosition);

	// 02_12 4枚目 ゲームプレイフェーズから開始
	phase_ = Phase::kFadeIn;

	// 02_13 27枚目
	fade_ = new Fade();
	fade_->Initialize();
	fade_->Start(Fade::Status::FadeIn, 1.0f);
}

void GameScene::ChangePhase() {

	switch (phase_) {
	case Phase::kPlay:
		// 02_12 13枚目 if文から中身まで全部実装
		// Initialize関数のいきなりパーティクル発生処理は消す
		if (player_->IsDead()) {
			// 死亡演出
			phase_ = Phase::kDeath;

			const Vector3& deathParticlesPosition = player_->GetWorldPosition();

			deathParticles_ = new DeathParticles;
			deathParticles_->Initialize(deathParticle_model_, &camera_, deathParticlesPosition);
		}
		// 敵が全員死んだらタイトルに戻る
	{
		bool allDead = true;
		for (Enemy* enemy : enemies_) {
			if (!enemy->IsDead()) {
				allDead = false;
				break;
			}
		}
		if (allDead) {
			fade_->Start(Fade::Status::FadeOut, 1.0f);
			phase_ = Phase::kFadeOut;
			
		}
	}
		if (Input::GetInstance()->PushKey(DIK_ESCAPE)) {
			// エスケープキーでフェードアウトしてタイトルへ
			fade_->Start(Fade::Status::FadeOut, 1.0f);
			phase_ = Phase::kFadeOut;
		}
		break;
	case Phase::kDeath:

		break;
	}
}

void GameScene::GenerateBlocks() {
	// 要素数
	uint32_t numBlockVirtical = mapChipField_->GetNumBlockVirtical();
	uint32_t numBlockHorizontal = mapChipField_->GetNumBlockHorizontal();

	// 要素数を変更する

	worldTransformBlocks_.resize(numBlockVirtical);

	// キューブの生成
	for (uint32_t i = 0; i < numBlockVirtical; ++i) {
		worldTransformBlocks_[i].resize(numBlockHorizontal);
	}
	// ブロックの生成
	for (uint32_t i = 0; i < numBlockVirtical; ++i) {
		for (uint32_t j = 0; j < numBlockHorizontal; ++j) {
			if (mapChipField_->GetMapChipTypeByIndex(j, i) == MapChipType::kBlock) {
				WorldTransform* worldTransform = new WorldTransform();
				worldTransform->Initialize();
				worldTransformBlocks_[i][j] = worldTransform;
				worldTransformBlocks_[i][j]->translation_ = mapChipField_->GetMapChipPositionByIndex(j, i);
			}
		}
	}
}

void GameScene::Update() {
	// ここにインゲームの更新処理を書
#pragma region 前の更新処理
	
#pragma endregion

	// 02_15 7枚目 デスフラグの立った敵を削除
	enemies_.remove_if([](Enemy* enemy) {
		if (enemy->IsDead()) {
			delete enemy;
			return true;
		}
		return false;
	});

	ChangePhase();

	switch (phase_) {

	case Phase::kFadeIn:
		fade_->Update();
		if (fade_->IsFinished()) {
			fade_->Start(Fade::Status::FadeOut, 1.0f);
			phase_ = Phase::kPlay;
		}

		skydome_->Update();
		CController_->Updata();
		//		worldTransformSkydome_.UpdateMatrix();
		//		cameraController->Update();

		// 自キャラの更新
		player_->UpDate();

		for (Enemy* enemy : enemies_) {
			enemy->UpDate();
		}

		// UpdateCamera();
#ifdef _DEBUG
		if (Input::GetInstance()->TriggerKey(DIK_0)) {
			// フラグをトグル
			isDebugCameraActive_ = !isDebugCameraActive_;
		}
#endif

		// カメラの処理
		if (isDebugCameraActive_) {
			debugCamera_->Update();
			camera_.matView = debugCamera_->GetCamera().matView;
			camera_.matProjection = debugCamera_->GetCamera().matProjection;
			// ビュープロジェクション行列の転送
			camera_.TransferMatrix();
		} else {
			// ビュープロジェクション行列の更新と転送
			camera_.UpdateMatrix();
		}

		// UpdateBlocks();
		// ブロックの更新
		for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
			for (WorldTransform*& worldTransformBlock : worldTransformBlockLine) {

				if (!worldTransformBlock)
					continue;

				// アフィン変換～DirectXに転送
				upData->WorldTransformUpData(*worldTransformBlock);
			}
		}
		break;

	case Phase::kPlay:
		// ゲームプレイフェーズの処理

		//   skydome生成
		skydome_->Update();

		CController_->Updata();

		//  自キャラの更新
		player_->UpDate();

		for (Enemy* enemy : enemies_) {
			enemy->UpDate();
		}

#ifdef _DEBUG
		if (Input::GetInstance()->TriggerKey(DIK_SPACE)) {
			// フラグをトグル
			isDebugCameraActive_ = !isDebugCameraActive_;
		}
#endif

		// カメラの処理
		if (isDebugCameraActive_) {
			debugCamera_->Update();
			camera_.matView = debugCamera_->GetCamera().matView;
			camera_.matProjection = debugCamera_->GetCamera().matProjection;
			// ビュープロジェクション行列の転送AL3_02_02*/
			camera_.TransferMatrix();
		} else {
			// ビュープロジェクション行列の更新と転送AL3_02_02*/

			camera_.UpdateMatrix();
		}

		// ブロックの更新
		for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
			for (WorldTransform*& worldTransformBlock : worldTransformBlockLine) {
				if (!worldTransformBlock)
					continue;
				// アフィン変換行列の生成
				upData->WorldTransformUpData(*worldTransformBlock);
			}
		}

		CheckAllCollisions();
		break;

	case Phase::kDeath:
		// デス演出フェーズ

		if (deathParticles_ && deathParticles_->IsFinished()) {
			phase_ = Phase::kFadeOut;
		}

		//    skydome生成
		skydome_->Update();
		CController_->Updata();

		// 敵の更新
		for (Enemy* enemy : enemies_) {
			enemy->UpDate();
		}

		// 02_11 18枚目 デスパーティクルあれば更新
		if (deathParticles_) {
			deathParticles_->Update();
		}

		break;

	case Phase::kFadeOut:
		fade_->Update();
		if (fade_->IsFinished()) {
			finished_ = true;
		}

		skydome_->Update();
		CController_->Updata();

		for (Enemy* enemy : enemies_) {
			enemy->UpDate();
		}

		break;
	}
}

void GameScene::Draw() {

	DirectXCommon* dxCommon = DirectXCommon::GetInstance();

	Model::PreDraw(dxCommon->GetCommandList());

	if (!player_->IsDead()) {
		player_->Draw();
	}

	// 天球描画
	skydome_->Draw();

	// ブロックの描画
	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform*& worldTransformBlock : worldTransformBlockLine) {
			if (!worldTransformBlock)
				continue;
			blockModel_->Draw(*worldTransformBlock, camera_);
		}
	}

	// 02_09 12枚目 敵更新
	for (Enemy* enemy : enemies_) {
		enemy->Draw();
	}

	// 02_11 18枚目 デスパーティクルあれば描画
	if (deathParticles_) {
		deathParticles_->Draw();
	}

	Model::PostDraw();

	// スプライト描画前処理
	Sprite::PreDraw(dxCommon->GetCommandList());

	// スプライト描画後処理
	Sprite::PostDraw();

	// 02_13 28枚目
	fade_->Draw();
}

// 02_10 16枚目
void GameScene::CheckAllCollisions() {

	// 判定対象1と2の座標
	AABB aabb1, aabb2;

#pragma region 自キャラと敵キャラの当たり判定
	{
		// 自キャラの座標
		aabb1 = player_->GetAABB();

		// 自キャラと敵弾全ての当たり判定
		for (Enemy* enemy : enemies_) {

			// コリジョン無効の敵はスキップ
			if (enemy->IsCollisionDisabled())
				continue;

			// 敵弾の座標
			aabb2 = enemy->GetAABB();

			// AABB同士の交差判定
			if (IsCollision(aabb1, aabb2)) {
				// 自キャラの衝突時コールバックを呼び出す
				player_->OnCollision(enemy);
				// 敵弾の衝突時コールバックを呼び出す
				enemy->OnCollision(player_);
			}
		}
	}
#pragma endregion
}