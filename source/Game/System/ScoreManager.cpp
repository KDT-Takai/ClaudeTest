// ============================================================================
// ScoreManager - スコア管理システム（実装部）
// ============================================================================

#include "Game/System/ScoreManager.h"

namespace Game {

// メンバー関数の実装はすべて inline で header に記述済みです。
// ここでは追加機能を実装します。

void ScoreManager::Initialize() {
    score_ = 0.0f;
}

void ScoreManager::Update() {
    // スコア増加ロジック
}

void ScoreManager::AddPipeScore() {
    score_ += POINT_PER_PIPE;
}

float ScoreManager::GetScore() const {
    return score_;
}

int ScoreManager::GetIntegerScore() const {
    return static_cast<int>(score_);
}

void ScoreManager::Reset() {
    Initialize();
}

} // namespace Game
