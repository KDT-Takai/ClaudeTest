// ============================================================================
// Player クラス - プレイヤー（鳥）の動作管理
// ============================================================================

#pragma once

#include <DxLib.h>

namespace Game {

class Player {
public:
    // 定数定義
    static constexpr float WIDTH = 32.0f;
    static constexpr float HEIGHT = 32.0f;
    
private:
    // メンバー変数
    float x_ = 200.0f;      // X 座標
    float y_ = 300.0f;      // Y 座標
    bool isJumping_ = false;// ジャンプ中フラグ

public:
    // コンストラクタ
    Player() : x_(200.0f), y_(300.0f) {}

    // 初期化
    void Initialize() {
        x_ = 200.0f;
        y_ = 300.0f;
        isJumping_ = false;
    }

    // 更新処理（毎フレーム呼び出します）
    void Update(float deltaTime, float& pipeIntervalX) {
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

    // ジャンプアクション
    void Jump() {
        if (!isJumping_) {
            isJumping_ = true;
        }
    }

    // 描画処理
    void Draw() const {
        // プレイヤー本体（簡易描画）
        DxLib_SetColor(0xFFFF0000);  // 赤色
        DxLib_DrawRect(x_ - WIDTH / 2.0f, y_, 
                       x_ + WIDTH / 2.0f, y_ + HEIGHT, true);

        // 目（方向を示す）
        float eyeOffset = (x_ < 100) ? 4 : -4;
        DxLib_DrawCircle(x_ - WIDTH / 2.0f + 8 + eyeOffset, y_ + HEIGHT / 3.0f, 
                         3, true);

        // --- 詳細描画オプション（必要に応じて追加）---
        
        // 羽ばたきのアニメーション（簡易実装）
        float flapOffset = (int)(x_ * 10.0f) % 2;
        if (flapOffset == 0) {
            // 羽ばたき左
            DxLib_DrawCircle(x_ - WIDTH / 2.0f + 6, y_ + HEIGHT / 4.0f, 
                            2, true);
        } else {
            // 羽ばたき右
            DxLib_DrawCircle(x_ - WIDTH / 2.0f + 10, y_ + HEIGHT / 4.0f, 
                            2, true);
        }

        // 足（簡易描画）
        float footOffset = (int)(x_ * 5.0f) % 2;
        if (footOffset == 0) {
            DxLib_DrawCircle(x_ - WIDTH / 2.0f + 4, y_ + HEIGHT - 8, 
                            3, true);
        } else {
            DxLib_DrawCircle(x_ - WIDTH / 2.0f + 8, y_ + HEIGHT - 8, 
                            3, true);
        }
    }

    // リセット処理
    void Reset() { Initialize(); }
};

} // namespace Game
