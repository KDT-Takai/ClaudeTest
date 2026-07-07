// ============================================================================
// SceneManager - シーン管理システム（実装部）
// ============================================================================

#include "Game/System/SceneManager.h"

namespace Game {

// メンバー関数の実装はすべて inline で header に記述済みです。
// ここでは追加機能を実装します。

void SceneManager::Initialize() {
    currentScene_ = SceneType::TITLE;
}

void SceneManager::Update() {
    // シーン遷移ロジック
    switch (currentScene_) {
        case SceneType::GAME:
            // ゲーム中での遷移条件
            break;
        
        default:
            break;
    }
}

void SceneManager::Draw() const {
    // シーンごとの描画ロジック
    switch (currentScene_) {
        case SceneType::TITLE:
            DrawTitleScreen();
            break;
        
        case SceneType::GAME:
            DrawGameScreen();
            break;
        
        case SceneType::RESULT:
            DrawResultScreen();
            break;
    }
}

void SceneManager::TransitionTo(SceneType newScene) {
    currentScene_ = newScene;
}

SceneManager::SceneType SceneManager::GetCurrentScene() const {
    return currentScene_;
}

void SceneManager::Reset() {
    Initialize();
}

// タイトル画面描画
void SceneManager::DrawTitleScreen() {
    DxLib_SetColor(0xFFFFFFFF);
    char title[128];
    sprintf_s(title, sizeof(title), "Flappy Bird\nPress SPACE to Start");
    DxLib_DrawString(10, 150, title, true);
}

// ゲーム画面描画
void SceneManager::DrawGameScreen() {
    // ゲーム中のメイン描画は MainGame で処理します
}

// 結果画面描画
void SceneManager::DrawResultScreen() {
    DxLib_SetColor(0xFFFFFFFF);
    char result[128];
    sprintf_s(result, sizeof(result), "Game Over\nPress R to Restart");
    DxLib_DrawString(10, 150, result, true);
}

} // namespace Game
