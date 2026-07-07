// ============================================================================
// InputManager - 入力管理システム
// ============================================================================

#pragma once

#include <DxLib.h>

namespace Game {

class InputManager {
public:
    // 定数定義
    static constexpr int KEY_JUMP = DX_KEY_SPACE;
    static constexpr int KEY_RESET = DX_KEY_R;
    
private:
    // メンバー変数
    bool jumpRequested_ = false;
    bool resetRequested_ = false;

public:
    // コンストラクタ
    InputManager() : jumpRequested_(false), resetRequested_(false) {}

    ~InputManager() = default;

    // 初期化
    void Initialize() {
        jumpRequested_ = false;
        resetRequested_ = false;
    }

    // 更新処理（毎フレーム呼び出します）
    void Update() {
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

    // 入力状態の取得
    bool IsJump() const {
        return jumpRequested_;
    }

    bool IsReset() const {
        return resetRequested_;
    }

    // リセット処理
    void Reset() {
        jumpRequested_ = false;
        resetRequested_ = false;
    }
};

} // namespace Game
