// ============================================================================
// InputManager - 入力管理システム（実装部）
// ============================================================================

#include "Game/System/InputManager.h"

namespace Game {

// メンバー関数の実装はすべて inline で header に記述済みです。
// ここでは追加機能を実装します。

void InputManager::Initialize() {
    jumpRequested_ = false;
    resetRequested_ = false;
}

void InputManager::Update() {
    // ジャンプ入力チェック
    if (DxLib_IsKeyDown(KEY_JUMP)) {
        jumpRequested_ = true;
    } else {
        jumpRequested_ = false;
    }

    // リセット入力チェック
    if (DxLib_IsKeyDown(KEY_RESET)) {
        resetRequested_ = true;
    } else {
        resetRequested_ = false;
    }
}

bool InputManager::IsJump() const {
    return jumpRequested_;
}

bool InputManager::IsReset() const {
    return resetRequested_;
}

void InputManager::Reset() {
    jumpRequested_ = false;
    resetRequested_ = false;
}

} // namespace Game
