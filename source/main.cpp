// ============================================================================
// FlappyBird - 単一ソースファイル実装版（最終統合）
// Build ファイルなしで完結するシンプルなゲームエンジン
// ============================================================================

#include <DxLib.h>
#include <cstdio>

// --- インクルードパス設定 ---
#define ACTOR_PLAYER_DIR "Actor/Player"
#define ACTOR_PIPE_DIR   "Actor/Pipe"
#define GAME_DIR         "Game"
#define SYSTEM_DIR       "Game/System"

// --- ヘッダーインクルード ---
#include "Define/Constants.h"
#include "Actor/Player/Player.h"
#include "Actor/Pipe/Pipe.h"
#include "Game/MainGame.h"
#include "Game/System/CollisionSystem.h"
#include "Game/System/InputManager.h"
#include "Game/System/ResourceManager.h"
#include "Game/System/SceneManager.h"
#include "Game/System/ScoreManager.h"

// ============================================================================
// メイン関数 - 完全実装版
// ============================================================================

int main() {
    // DxLib 初期化
    DxLib_Init();
    
    // ゲーム初期化
    Game::MainGame game;
    
    // システムの初期化
    game.Initialize();
    
    // メインゲームループ実行
    game.MainGame();
    
    // クリーンアップ
    game.Finalize();
    
    // DxLib 終了処理
    DxLib_End();
    
    return 0;
}
