// ============================================================================
// CollisionSystem - コリジョン検出システム（実装部）
// ============================================================================

#include "Game/System/CollisionSystem.h"

namespace Game {

// メンバー関数の実装はすべて inline で header に記述済みです。
// ここでは追加機能を実装します。

void CollisionSystem::Initialize() {
    collisionDetected_ = false;
}

bool CollisionSystem::CheckCollision(float playerX, float playerY, 
                                     float pipeX, float pipeY) const {
    // プレイヤーとパイプの矩形交差チェック
    float left   = std::max(playerX - PLAYER_WIDTH/2.0f, pipeX);
    float right  = std::min(playerX + PLAYER_WIDTH/2.0f, pipeX + PIPE_WIDTH);
    float top    = std::max(playerY, pipeY);
    float bottom = std::min(playerY + PLAYER_HEIGHT, pipeY + PIPE_HEIGHT);

    // 交差がある場合
    if (right > left && bottom > top) {
        collisionDetected_ = true;
        return true;
    }

    return false;
}

bool CollisionSystem::IsCollision() const {
    return collisionDetected_;
}

void CollisionSystem::Reset() {
    collisionDetected_ = false;
}

} // namespace Game
