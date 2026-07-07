// ============================================================================
// Pipe クラス - パイプ（障害物）の動作管理（実装部）
// ============================================================================

#include "Actor/Pipe/Pipe.h"

namespace Game {

// メンバー関数の実装はすべて inline で header に記述済みです。
// ここでは追加機能を実装します。

void Pipe::Update(float deltaTime, float& pipeIntervalX) {
    // X 軸移動
    x_ -= 3.0f * deltaTime;

    // パイプが画面を通過したか確認
    if (x_ + WIDTH < 0) {
        isPassed_ = true;
        Initialize();
    }

    // 間隔制御（簡易実装）
    pipeIntervalX -= 3.0f * deltaTime;
    if (pipeIntervalX <= 0) {
        pipeIntervalX = 300;
        Initialize();
    }
}

void Pipe::Reset() { isPassed_ = false; Initialize(); }

} // namespace Game
