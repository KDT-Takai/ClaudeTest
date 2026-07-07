// ============================================================================
// SceneManager - シーン管理システム
// ============================================================================

#pragma once

#include <DxLib.h>

namespace Game {

class SceneManager {
public:
    // シーン定義
    enum class SceneType {
        TITLE,      // タイトル画面
        GAME,       // ゲーム中
        RESULT      // 結果画面
    };

private:
    // メンバー変数
    SceneType currentScene_ = SceneType::TITLE;

public:
    // コンストラクタ
    SceneManager() : currentScene_(SceneType::TITLE) {}

    ~SceneManager() = default;

    // 初期化
    void Initialize() {
        currentScene_ = SceneType::TITLE;
    }

    // 更新処理（毎フレーム呼び出します）
    void Update() {
        // シーン遷移ロジック
        switch (currentScene_) {
            case SceneType::GAME:
                // ゲーム中での遷移条件
                // コリジョン発生時の遷移などは ProcessGameScreen で処理
                break;

            default:
                break;
        }
    }

    // 描画処理
    void Draw() const {
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

    // シーン遷移処理
    void TransitionTo(SceneType newScene) {
        currentScene_ = newScene;
    }

    // 現在のシーンの取得
    SceneType GetCurrentScene() const {
        return currentScene_;
    }

    // リセット処理
    void Reset() {
        Initialize();
    }

private:
    // タイトル画面描画
    static void DrawTitleScreen() {
        DxLib_SetColor(0xFFFFFFFF);
        char title[128];
        sprintf_s(title, sizeof(title), "Flappy Bird\nPress SPACE to Start");
        DxLib_DrawString(10, 150, title, true);
    }

    // ゲーム画面描画
    static void DrawGameScreen() {
        // ゲーム中のメイン描画は MainGame で処理します
    }

    // 結果画面描画
    static void DrawResultScreen() {
        DxLib_SetColor(0xFFFFFFFF);
        char result[128];
        sprintf_s(result, sizeof(result), "Game Over\nPress R to Restart");
        DxLib_DrawString(10, 150, result, true);
    }
};

} // namespace Game
