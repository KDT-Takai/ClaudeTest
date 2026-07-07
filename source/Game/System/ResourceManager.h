// ============================================================================
// ResourceManager - リソース管理システム
// ============================================================================

#pragma once

#include <DxLib.h>

namespace Game {

class ResourceManager {
public:
    // 定数定義
    static constexpr int RESOURCE_STATE_IDLE = 0;
    static constexpr int RESOURCE_STATE_LOADING = 1;
    static constexpr int RESOURCE_STATE_READY = 2;
    
private:
    // メンバー変数
    int state_ = RESOURCE_STATE_IDLE;

public:
    // コンストラクタ
    ResourceManager() : state_(RESOURCE_STATE_IDLE) {}

    ~ResourceManager() = default;

    // 初期化
    void Initialize() {
        state_ = RESOURCE_STATE_IDLE;
    }

    // 更新処理（毎フレーム呼び出します）
    void Update() {
        // 状態遷移ロジック
        if (state_ == RESOURCE_STATE_LOADING) {
            // ローディング完了判定
            state_ = RESOURCE_STATE_READY;
        }
    }

    // リソース読み込み処理
    bool LoadResources() {
        // ここに実際のリソース読み込みを追加します
        // 例：画像、音声ファイルの読み込みなど
        
        state_ = RESOURCE_STATE_LOADING;
        
        // シンプルな判定（実際には時間やイベントで制御）
        return true;
    }

    // リソース解放処理
    void UnloadResources() {
        // ここに実際のリソース解放を追加します
        state_ = RESOURCE_STATE_IDLE;
    }

    // 状態の取得
    int GetState() const {
        return state_;
    }

    // リセット処理
    void Reset() {
        UnloadResources();
        Initialize();
    }
};

} // namespace Game
