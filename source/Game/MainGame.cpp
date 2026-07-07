// ============================================================================
// MainGame - ゲームメインクラス（完全実装）
// シーン管理システムとの統合
// ============================================================================

#include "Game/MainGame.h"
#include <cstdio>

namespace Game {

// メンバー関数の実装はすべて inline で header に記述済みです。
// ここでは追加機能を実装します。

void MainGame::Initialize() {
    player_.Initialize();
    pipe_.Initialize();
    pipeIntervalX_ = 300.0f;
    gameRunning_ = true;

    // シーン管理システムの初期化
    sceneManager_.Initialize();

    // スコア管理システムの初期化
    scoreManager_.Initialize();

    // 共通処理の初期化
    Input();
}

void MainGame::MainGame() {
    float deltaTime = 0.0f;

    while (gameRunning_) {
        // シーンごとの処理
        switch (sceneManager_.GetCurrentScene()) {
            case SceneManager::SceneType::TITLE:
                ProcessTitleScreen();
                break;

            case SceneManager::SceneType::GAME:
                ProcessGameScreen();
                break;

            case SceneManager::SceneType::RESULT:
                ProcessResultScreen();
                break;
        }

        // シーン更新
        sceneManager_.Update();

        // 共通処理（入力、描画など）
        Input();
        Update();
        Draw();

        DxLib_GetTime(&deltaTime);
    }

    Finalize();
}

// タイトル画面処理
void MainGame::ProcessTitleScreen() {
    // タイトル画面での入力チェック
    if (DxLib_IsKeyDown(DX_KEY_SPACE)) {
        sceneManager_.TransitionTo(SceneManager::SceneType::GAME);
    }

    // 自動遷移（オプション）
    // if (deltaTime > 10.0f) {
    //     sceneManager_.TransitionTo(SceneManager::SceneType::GAME);
    // }
}

// ゲーム中処理
void MainGame::ProcessGameScreen() {
    // ゲーム終了条件チェック
    if (player_.y_ > DEFAULT_SCREEN_HEIGHT) {
        sceneManager_.TransitionTo(SceneManager::SceneType::RESULT);
    }

    // パイプ通過時のスコア更新
    if (pipe_.IsPassed()) {
        scoreManager_.AddPipeScore();
        pipe_.Reset();
    }

    // コリジョンチェック（簡易実装）
    if (!player_.isJumping_) {
        // 地面に接地している場合のコリジョンチェック
        float groundY = DEFAULT_SCREEN_HEIGHT - PLAYER::HEIGHT;
        if (player_.y_ >= groundY) {
            player_.isJumping_ = true;
        }

        // パイプとのコリジョンチェック
        CollisionSystem collision;
        if (collision.CheckCollision(player_.x_, player_.y_, pipe_.x_, pipe_.y_)) {
            sceneManager_.TransitionTo(SceneManager::SceneType::RESULT);
        }
    }
}

// 結果画面処理
void MainGame::ProcessResultScreen() {
    // リセット入力チェック
    if (DxLib_IsKeyDown(DX_KEY_R)) {
        sceneManager_.TransitionTo(SceneManager::SceneType::GAME);
        player_.Reset();
        pipe_.Initialize();
        pipeIntervalX_ = 300.0f;
        scoreManager_.Reset();
    }

    // ESC キーでタイトル画面へ戻る
    if (DxLib_IsKeyDown(DX_KEY_ESCAPE)) {
        sceneManager_.TransitionTo(SceneManager::SceneType::TITLE);
    }
}

// タイトル画面描画
void MainGame::DrawTitleScreen() {
    DxLib_SetColor(0xFFFFFFFF);
    char title[128];
    sprintf_s(title, sizeof(title), "Flappy Bird\nPress SPACE to Start");
    DxLib_DrawString(10, 150, title, true);

    // スコア表示（タイトル画面でも確認可能）
    if (scoreManager_.GetIntegerScore() > 0) {
        char scoreText[64];
        sprintf_s(scoreText, sizeof(scoreText), "High Score: %d", 
                  scoreManager_.GetIntegerScore());
        DxLib_DrawString(10, 175, scoreText, true);
    }
}

// ゲーム画面描画
void MainGame::DrawGameScreen() {
    // --- 背景描画 ---
    DxLib_SetColor(COLOR_BG);
    DxLib_ClearScreen(true);

    // --- プレイヤー描画 ---
    player_.Draw();

    // --- パイプ描画 ---
    pipe_.Draw();

    // --- スコア表示 ---
    if (scoreManager_.GetIntegerScore() > 0) {
        char scoreText[64];
        sprintf_s(scoreText, sizeof(scoreText), "Score: %d", 
                  scoreManager_.GetIntegerScore());
        DxLib_SetColor(0xFFFFFFFF);
        DxLib_DrawString(10, 20, scoreText, true);
    }

    // --- フレームレート表示 ---
    char fpsText[64];
    sprintf_s(fpsText, sizeof(fpsText), "FPS: %d", 
              static_cast<int>(1.0f / deltaTime));
    DxLib_DrawString(10, 35, fpsText, true);

    // --- ステータス表示 ---
    char statusText[64];
    sprintf_s(statusText, sizeof(statusText), "Jump: %s", 
              player_.isJumping_ ? "OK" : "Press Space");
    DxLib_DrawString(10, 50, statusText, true);
}

// 結果画面描画
void MainGame::DrawResultScreen() {
    // --- 背景描画 ---
    DxLib_SetColor(COLOR_BG);
    DxLib_ClearScreen(true);

    // --- メインメッセージ ---
    DxLib_SetColor(0xFFFFFFFF);
    char result[128];
    sprintf_s(result, sizeof(result), "Game Over\nPress R to Restart");
    DxLib_DrawString(10, 150, result, true);

    // --- スコア表示 ---
    if (scoreManager_.GetIntegerScore() > 0) {
        char scoreText[64];
        sprintf_s(scoreText, sizeof(scoreText), "Final Score: %d", 
                  scoreManager_.GetIntegerScore());
        DxLib_DrawString(10, 175, scoreText, true);

        // --- ハイスコア表示 ---
        char highScoreText[64];
        sprintf_s(highScoreText, sizeof(highScoreText), "High Score: %d", 
                   scoreManager_.GetIntegerScore());
        DxLib_DrawString(10, 195, highScoreText, true);
    }

    // --- ステータス表示 ---
    char statusText[64];
    sprintf_s(statusText, sizeof(statusText), "Press SPACE to Continue");
    DxLib_DrawString(10, 215, statusText, true);
}

// --- 共通処理実装 ---

void MainGame::Update() {
    // プレイヤー更新
    player_.Update(deltaTime_, pipeIntervalX_);

    // パイプ更新
    pipe_.Update(deltaTime_, pipeIntervalX_);

    // スコア管理更新
    scoreManager_.Update();

    // システム更新
    InputManager::Update();
    ResourceManager::Update();
}

void MainGame::Draw() {
    // シーンごとの描画呼び出し
    switch (sceneManager_.GetCurrentScene()) {
        case SceneManager::SceneType::TITLE:
            DrawTitleScreen();
            break;

        case SceneManager::SceneType::GAME:
            DrawGameScreen();
            break;

        case SceneManager::SceneType::RESULT:
            DrawResultScreen();
            break;
    }
}

void MainGame::Input() {
    // プレイヤーのジャンプ入力
    if (player_.isJumping_) {
        player_.Jump();
    }

    // 共通入力の処理（必要に応じて拡張）
    // InputManager::Update();
}

// クリーンアップ処理
void MainGame::Finalize() {
    // クリーンアップ
    // リソース解放などを実装します
}

} // namespace Game