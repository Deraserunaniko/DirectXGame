#include "Fade.h"
#include <algorithm>

void Fade::Initialize() {
	
	sprite_ = Sprite::Create(0, Vector2{});

	sprite_->SetSize(Vector2(WinApp::kWindowWidth, WinApp::kWindowHeight));
	sprite_->SetColor(Vector4(0, 0, 0, 1));
}

void Fade::Update() {

	//  フェード状態による分岐
	switch (status_) {
	case Status::None:

		break;
	case Status::FadeIn:
		

		// 1フレーム分の秒数をカウントアップ
		counter_ += 1.0f / 60.0f;
		// フェード継続時間に達したら打ち止め
		if (counter_ >= duration_) {
			counter_ = duration_;
		}
		
		sprite_->SetColor(Vector4(0, 0, 0, std::clamp(1.0f - counter_ / duration_, 0.0f, 1.0f)));

		break;
	case Status::FadeOut:
		

		
		counter_ += 1.0f / 60.0f;
		
		if (counter_ >= duration_) {
			counter_ = duration_;
		}
		
		sprite_->SetColor(Vector4(0, 0, 0, std::clamp(counter_ / duration_, 0.0f, 1.0f)));
		break;
	}
}

void Fade::Draw() {

	
	if (status_ == Status::None) {
		return;
	}

	
	Sprite::PreDraw(DirectXCommon::GetInstance()->GetCommandList());
	sprite_->Draw();
	Sprite::PostDraw();
}

//  フェード開始
void Fade::Start(Status status, float duration) {

	status_ = status;
	duration_ = duration;
	counter_ = 0.0f;
}

//  フェード停止
void Fade::Stop() { status_ = Status::None; }

// フェード終了判定
bool Fade::IsFinished() const {

	// フェード状態による分岐
	switch (status_) {
	case Status::FadeIn:
	case Status::FadeOut:

		return (counter_ >= duration_) ? true : false;
	}

	return true;
}