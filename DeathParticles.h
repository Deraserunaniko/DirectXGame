#pragma once

#include "KamataEngine.h"
#include "Math.h"
#include "UpData.h"
#include <algorithm>
#include <array>
#include <numbers>

using namespace KamataEngine;

/// <summary>
/// デス演出用パーティクル
/// </summary>
//  デス演出用パーティクル
class DeathParticles {
public:
	//  Initialize,Update,Draw関数追加
	void Initialize(Model* model, Camera* camera, const Vector3& position);
	void Update();
	void Draw();


	bool IsFinished() { return isFinished_; }

private:
	// パーティクルの個数
	static inline const uint32_t kNumParticles = 8;

	//パーティクル座標配列
	std::array<WorldTransform, kNumParticles> worldTransforms_;

	//  モデル
	Model* model_ = nullptr;

	//  カメラ
	Camera* camera_ = nullptr;

	// 存続時間（消滅までの時間）<秒>
	static inline const float kDuration_ = 2.0f;

	//  移動の速さ
	static inline const float kSpeed_ = 0.05f;

	//  分割した1個分の角度(#include <numbers>)
	static inline const float kAngleUnit_ = 2.0f * std::numbers::pi_v<float> / kNumParticles;

	// 終了フラグ
	bool isFinished_ = false;

	//  経過時間カウント
	float counter_ = 0.0f;

	// 色変更オブジェクト
	ObjectColor objectColor_;

	// 色の数値
	Vector4 color_;

	UpData* upData = nullptr;

	// ワールド変換データ
	WorldTransform worldTransform_;
};