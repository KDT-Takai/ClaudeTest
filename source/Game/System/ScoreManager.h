// ============================================================================
// ScoreManager - スコア管理システム
// ============================================================================

#pragma once

#include <DxLib.h>

namespace Game {

class ScoreManager {
public:
    // 定数定義
    static constexpr float POINT_PER_PIPE = 10.0f;
    
private:
    // メンバー変数
    float score_ = 0.0f;

public:
    // コンストラクタ
    ScoreManager() : score_(0.0f) {}

    ~ScoreManager() = default;

    // 初期化
    void Initialize() {
        score_ = 0.0f;
    }

    // 更新処理（毎フレーム呼び出します）
    void Update() {
        // スコア増加ロジック
        // ゲーム進行に伴うスコア増加などを実装します
        // 例：時間経過によるボーナス、距離走行など
    }

    // パイプ通過時のスコア追加
    void AddPipeScore() {
        score_ += POINT_PER_PIPE;
    }

    // スコアの取得
    float GetScore() const {
        return score_;
    }

    // スコアフォーマット（整数に変換）
    int GetIntegerScore() const {
        return static_cast<int>(score_);
    }

    // スコア表示用の文字列を取得
    std::string GetFormattedScore() const {
        char buffer[64];
        sprintf_s(buffer, sizeof(buffer), "%d", static_cast<int>(score_));
        return std::string(buffer);
    }

    // リセット処理
    void Reset() {
        Initialize();
    }
};

} // namespace Game
