#pragma once
#include "KamataEngine.h"
#include "MapChipField.h"
#include "Math.h"
#include "UpData.h"
#include "Player.h"


using namespace KamataEngine;

// 02_10 20枚目
class Player;

/// <summary>
/// 敵
/// </summary>
class Enemy {
public:
	// 02_15 13枚目 振るまい
	enum class Behavior {
		kUnknown = -1, // 無効な状態
		kWalk,         // 歩行状態
		kDefeated,     // やられ状態
	};
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model_"></param>
	/// <param name="camera_"></param>
	/// <param name="position"></param>
	void Initialize(Model* model, Camera* camera, const Vector3& position);

	/// <summary>
	/// 更新
	/// </summary>
	void UpDate();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	// 02_10 スライド14枚目
	AABB GetAABB();
	// 02_10 スライド14枚目 ワールド座標を取得
	Vector3 GetWorldPosition();
	// 02_10 スライド20枚目 衝突応答
	void OnCollision(const Player* player);
	// 02_15 6枚目
	bool IsDead() const { return isDead_; }

	// 02_15 20枚目
	bool IsCollisionDisabled() const { return isCollisionDisabled_; }

private:
	// ワールド変換データ
	WorldTransform worldTransform_;

	// モデル
	Model* model_ = nullptr;

	UpData* upData = nullptr;

	// テクスチャハンドル
	//  uint32_t textureHandle_ = 0u;

	Camera* camera_ = nullptr;

	// 02_09 15枚目
	static inline const float kWalkSpeed = 0.02f;
	// 02_09 15枚目
	Vector3 velocity_ = {};

	// 02_09 19枚目
	// 最初の角度
	static inline const float kWalkMotionAngleStart = -10.0f;

	// 02_09 19枚目
	// 最後の角度
	static inline const float kWalkMotionAngleEnd = 40.0f;

	// 02_09 19枚目
	static inline const float kWalkMotionTime = 1.0f;
	// 02_09 20枚目
	float walkTimer = 0.0f;

	// 02_10 14枚目 当たり判定サイズ
	static inline const float kWidth = 0.8f;
	static inline const float kHeight = 0.8f;

	bool isDead_ = false;

	// 02_15 13枚目
	Behavior behavior_ = Behavior::kWalk;
	Behavior behaviorRequest_ = Behavior::kUnknown;

	// 02_15 15枚目
	static inline const float kDefeatedTime = 0.6f;
	static inline const float kDefeatedMotionAngleStart = 0.0f;
	static inline const float kDefeatedMotionAngleEnd = -60.0f;
	float counter_ = 0.0f; // カウンター

	// 02_15 20枚目
	bool isCollisionDisabled_ = false;
};