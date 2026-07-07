// ============================================================================
// ResourceManager - リソース管理システム（実装部）
// ============================================================================

#include "Game/System/ResourceManager.h"

namespace Game {

// メンバー関数の実装はすべて inline で header に記述済みです。
// ここでは追加機能を実装します。

void ResourceManager::Initialize() {
    state_ = RESOURCE_STATE_IDLE;
}

void ResourceManager::Update() {
    // 状態遷移ロジック
    if (state_ == RESOURCE_STATE_LOADING) {
        // ローディング完了判定
        state_ = RESOURCE_STATE_READY;
    }
}

bool ResourceManager::LoadResources() {
    // ここに実際のリソース読み込みを追加します
    // 例：画像、音声ファイルの読み込みなど
    
    state_ = RESOURCE_STATE_LOADING;
    
    // シンプルな判定（実際には時間やイベントで制御）
    return true;
}

void ResourceManager::UnloadResources() {
    // ここに実際のリソース解放を追加します
    state_ = RESOURCE_STATE_IDLE;
}

int ResourceManager::GetState() const {
    return state_;
}

void ResourceManager::Reset() {
    UnloadResources();
    Initialize();
}

} // namespace Game
