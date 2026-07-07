// ============================================================================
// コリジョン検出システム
// ============================================================================

#pragma once

#include <DxLib.h>

namespace Game {

class CollisionSystem {
public:
    // 定数定義
    static constexpr float PLAYER_WIDTH = 32.0f;
    static constexpr float PIPE_WIDTH = 60.0f;
    static constexpr float PIPE_HEIGHT = 180.0f;
    
private:
    // メンバー変数
    bool collisionDetected_ = false;

public:
    // コンストラクタ
    CollisionSystem() : collisionDetected_(false) {}

    ~CollisionSystem() = default;

    // 初期化
    void Initialize() {
        collisionDetected_ = false;
    }

    // コリジョン検出処理
    bool CheckCollision(float playerX, float playerY, 
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

    // コリジョン状態の取得
    bool IsCollision() const {
        return collisionDetected_;
    }

    // リセット処理
    void Reset() {
        collisionDetected_ = false;
    }
};

} // namespace Game
