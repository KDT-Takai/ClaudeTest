// ============================================================================
// Pipe クラス - パイプ（障害物）の動作管理
// ============================================================================

#pragma once

#include <DxLib.h>

namespace Game {

class Pipe {
public:
    // 定数定義
    static constexpr float WIDTH = 60.0f;
    static constexpr float HEIGHT = 180.0f;
    
private:
    // メンバー変数
    float x_ = 0.0f;        // X 座標
    float y_ = 0.0f;        // Y 座標
    bool isPassed_ = false; // 通過済みフラグ

public:
    // コンストラクタ
    Pipe() : x_(480), 
             y_(640 - 180) {}

    // 初期化
    void Initialize() {
        x_ = 480;
        y_ = 640 - 180;
        isPassed_ = false;
    }

    // 更新処理（毎フレーム呼び出します）
    void Update(float deltaTime, float& pipeIntervalX) {
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

    // 描画処理
    void Draw() const {
        DxLib_SetColor(0xFF00FF00);  // 緑色

        // パイプ本体
        DxLib_DrawRect(x_, y_, x_ + WIDTH, y_ + HEIGHT, true);

        // パイプの縁（立体感）
        DxLib_DrawLine(x_, y_, x_ + WIDTH, y_, true);
        DxLib_DrawLine(x_, y_ + HEIGHT, x_ + WIDTH, y_ + HEIGHT, true);

        // --- 詳細描画オプション（必要に応じて追加）---

        // パイプの縁を強調表示
        DxLib_SetColor(0xFF80FF80);  // 明るい緑
        DxLib_DrawRect(x_, y_, x_ + WIDTH, y_ + HEIGHT, true);

        // パイプ内部の装飾（簡易実装）
        float decorationOffset = (int)(x_ * 7.0f) % 2;
        if (decorationOffset == 0) {
            DxLib_DrawLine(x_ + WIDTH / 4, y_, x_ + WIDTH / 4, y_ + HEIGHT - 10, true);
        } else {
            DxLib_DrawLine(x_ + WIDTH * 3 / 4, y_, x_ + WIDTH * 3 / 4, y_ + HEIGHT - 10, true);
        }
    }

    // リセット処理
    void Reset() { isPassed_ = false; Initialize(); }

    // 通過済み状態の取得
    bool IsPassed() const {
        return isPassed_;
    }
};

} // namespace Game
