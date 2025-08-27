#pragma once
#include "KamataEngine.h"
#include "MapChipField.h"
#include "Math.h"
#include "UpData.h"
#include "Player.h"


using namespace KamataEngine;

// 
class Player;

/// <summary>
/// 敵
/// </summary>
class Enemy {
public:
	//  振るまい
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

	
	AABB GetAABB();
	
	Vector3 GetWorldPosition();
	//  衝突応答
	void OnCollision(const Player* player);

	bool IsDead() const { return isDead_; }

	
	bool IsCollisionDisabled() const { return isCollisionDisabled_; }

private:
	// ワールド変換データ
	WorldTransform worldTransform_;

	// モデル
	Model* model_ = nullptr;

	UpData* upData = nullptr;


	Camera* camera_ = nullptr;

	
	static inline const float kWalkSpeed = 0.02f;
	
	Vector3 velocity_ = {};

	
	// 最初の角度
	static inline const float kWalkMotionAngleStart = -10.0f;

	
	// 最後の角度
	static inline const float kWalkMotionAngleEnd = 40.0f;

	
	static inline const float kWalkMotionTime = 1.0f;
	float walkTimer = 0.0f;

	//  当たり判定サイズ
	static inline const float kWidth = 0.8f;
	static inline const float kHeight = 0.8f;

	bool isDead_ = false;


	Behavior behavior_ = Behavior::kWalk;
	Behavior behaviorRequest_ = Behavior::kUnknown;

	
	static inline const float kDefeatedTime = 0.6f;
	static inline const float kDefeatedMotionAngleStart = 0.0f;
	static inline const float kDefeatedMotionAngleEnd = -60.0f;
	float counter_ = 0.0f; // カウンター

	
	bool isCollisionDisabled_ = false;
};