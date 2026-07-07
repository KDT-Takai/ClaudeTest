// ============================================================================
// Player クラス - プレイヤー（鳥）の動作管理（実装部）
// ============================================================================

#include "Actor/Player/Player.h"

namespace Game {

// メンバー関数の実装はすべて inline で header に記述済みです。
// ここでは追加機能を実装します。

void Player::Update(float deltaTime, float& pipeIntervalX) {
    // X 軸移動
    x_ -= 8.0f * deltaTime;

    // 画面左端を超えた場合のリセット
    if (x_ < -WIDTH / 2.0f) {
        x_ = 480 + WIDTH / 2.0f;
    }

    // ジャンプ処理
    if (isJumping_) {
        y_ -= 12.0f * deltaTime;
        if (y_ <= 0) {
            y_ = 0;
            isJumping_ = false;
        }
    }
}

void Player::Jump() {
    if (!isJumping_) {
        isJumping_ = true;
    }
}

void Player::Reset() { Initialize(); }

} // namespace Game
