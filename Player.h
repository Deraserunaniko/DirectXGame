#pragma once
#include "Enemy.h"
#include "KamataEngine.h"
#include "MapChipField.h"
#include "Math.h"
#include "UpData.h"
#include <algorithm>
#include <cassert>
#include <numbers>

class MapChipField;

using namespace KamataEngine;
class Enemy;

class Player {
public:
	// 左右
	enum class LRDirection {
		kRight,
		kLeft,
	};

	
	enum Corner { kRightBottom, kLeftBottom, kRightTop, kLeftTop, kNumCorner };

	
	enum class Behavior {
		kUnknown = -1,
		kRoot,   // 通常状態
		kAttack, // 攻撃中
	};

	//  攻撃フェーズ
	enum class AttackPhase {
		kUnknown = -1, // 無効な状態

		kAnticipation, // 予備動作
		kAction,       // 前進動作
		kRecovery,     // 余韻動作
	};

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Model* model_, Model* modelAttack, Camera* camera_, const Vector3& position);

	/// <summary>
	/// 更新
	/// </summary>
	void UpDate();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	// getter
	const WorldTransform& GetWorldTransform() const { return worldTransform_; }

	
	const Vector3& GetVelocity() const { return velocity_; }

	
	void SetMapChipField(MapChipField* mapChipField) { mapChipField_ = mapChipField; }

	//  ワールド座標を取得
	Vector3 GetWorldPosition();

	// AABBを取得
	AABB GetAABB();

	//  衝突応答
	void OnCollision(const Enemy* enemy);

	//  デスフラグ
	bool IsDead() const { return isDead_; }

	// 通常行動更新
	void BehavoirRootUpdate();

	//  攻撃行動更新
	void BehaviorAttackUpdate();

	//  通常行動初期化
	void BehaviorRootInitialize();

	// 攻撃行動初期化
	void BehaviorAttackInitialize();


	bool IsAttack() const { return behavior_ == Behavior::kAttack && attackPhase_ == AttackPhase::kAction; }

private:
	// ワールド変換データ
	WorldTransform worldTransform_;

	// モデル
	Model* model_ = nullptr;

	Model* modelAttack_ = nullptr;

	

	Camera* camera_ = nullptr;

	

	UpData* upData = nullptr;

	Vector3 velocity_ = {};

	// フレームごとの加速度
	static inline const float kAcceleration = 0.1f;

	// 顔の向き
	LRDirection lrDirection_ = LRDirection::kRight;

	// 非入力時の摩擦係数
	static inline const float kAttenuation = 0.05f;

	// 速度制限
	
	static inline const float kLimitRunSpeed = 0.3f;

	

	// 旋回開始時の角度
	float turnFirstRotationY_ = 0.0f;

	// 旋回タイマー
	float turnTimer_ = 0.0f;

	// 旋回時間<秒>
	static inline const float kTimeTurn = 0.3f;

	// 接地状態フラグ
	bool onGround_ = true;

	//  ジャンプ定数もろもろ

	// ジャンプ初速(上方向)
	static inline const float kJumpAcceleration = 20.0f;

	// 重力加速度(下方向)
	static inline const float kGravityAcceleration = 0.98f;

	// 最大落下速度(下方向)
	static inline const float kLimitFallSpeed = 0.5f;

	// マップチップによるフィールド
	MapChipField* mapChipField_ = nullptr;

	// キャラクターの当たり判定サイズ
	static inline const float kWidth = 0.8f;
	static inline const float kHeight = 0.8f;

	
	static inline const float kBlank = 0.04f;

	// 着地時の速度減算衰率
	static inline const float kAttenuationLanding = 0.0f;

	// 微小な数値
	static inline const float kGroundSearchHeight = 0.06f;

	//  着地時の速度減衰率
	static inline const float kAttenuationWall = 0.2f;

	void InputMove();

	// マップチップとの当たり判定情報

	struct CollisionMapInfo {
		// 天井衝突フラグ
		bool ceiling = false;
		// 着地フラグ
		bool landing = false;
		// 壁接触フラグ
		bool hitWall = false;
		// 移動量
		Vector3 move;
	};


	void CheckMapCollision(CollisionMapInfo& info);

	
	void CheckMapCollisionUp(CollisionMapInfo& info);
	void CheckMapCollisionDown(CollisionMapInfo& info);
	void CheckMapCollisionRight(CollisionMapInfo& info);
	void CheckMapCollisionLeft(CollisionMapInfo& info);

	
	Vector3 CornerPosition(const Vector3& center, Corner corner);

	// 設置状態の切り替え処理
	void UpdateOnGround(const CollisionMapInfo& info);

	// 壁接触している場合の処理
	void UpdateOnWall(const CollisionMapInfo& info);

	//  デスフラグ
	bool isDead_ = false;

	// 振るまい
	Behavior behavior_ = Behavior::kRoot;

	//  次の振るまいリクエスト
	Behavior behaviorRequest_ = Behavior::kUnknown;

	// 攻撃ギミックの経過時間カウンター
	uint32_t attackParameter_ = 0;

	// 攻撃フェーズ
	AttackPhase attackPhase_ = AttackPhase::kUnknown;

	//  予備動作の時間
	static inline const uint32_t kAnticipationTime = 8;
	// 前進動作の時間
	static inline const uint32_t kActionTime = 5;
	//  余韻動作の時間
	static inline const uint32_t kRecoveryTime = 12;
	WorldTransform worldTransformAttack_;
};