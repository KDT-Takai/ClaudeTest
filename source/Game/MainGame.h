// ============================================================================
// MainGame - ゲームメインクラス（完全実装）
// シーン管理システムとの統合
// ============================================================================

#pragma once

#include <DxLib.h>
#include "Actor/Player/Player.h"
#include "Actor/Pipe/Pipe.h"
#include "Game/System/SceneManager.h"
#include "Game/System/ScoreManager.h"

namespace Game {

class MainGame {
public:
    // 定数定義
    static constexpr float PLAYER_SPEED_X = -8.0f;
    static constexpr float PIPE_SPEED_X = 3.0f;

private:
    // メンバー変数
    Player player_;
    Pipe pipe_;
    float pipeIntervalX_ = 300.0f;
    bool gameRunning_ = true;
    float deltaTime_ = 0.0f;

    // シーン管理システム
    SceneManager sceneManager_;
    
    // スコア管理システム
    ScoreManager scoreManager_;

public:
    // コンストラクタ
    MainGame() : player_(), pipe_(), 
                 sceneManager_(SceneManager::SceneType::TITLE),
                 scoreManager_() {}

    ~MainGame() = default;

    // 初期化
    void Initialize();

    // メインゲームループ
    void MainGame();

private:
    // タイトル画面処理
    void ProcessTitleScreen();

    // ゲーム中処理
    void ProcessGameScreen();

    // 結果画面処理
    void ProcessResultScreen();

    // シーンごとの描画
    static void DrawTitleScreen();
    static void DrawGameScreen();
    static void DrawResultScreen();

    // --- 共通処理（メインゲームループ内で呼び出します）---

    // ゲーム全体の更新処理
    void Update();

    // ゲーム全体の描画処理
    void Draw();

    // ゲーム全体の入力処理
    void Input();

    // クリーンアップ処理
    void Finalize();
};

} // namespace Game
