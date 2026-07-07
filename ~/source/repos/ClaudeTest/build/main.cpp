// ============================================================================
// FlappyBird - 単一ソースファイル実装版
// Build ファイルなしで完結するシンプルなゲームエンジン
// ============================================================================

#include <DxLib.h>
#include <cstdio>

// ============================================================================
// 定数定義（Constants.h の内容）
// ============================================================================

#define DEFAULT_SCREEN_WIDTH   480
#define DEFAULT_SCREEN_HEIGHT  640

constexpr int GAME_UPDATE_INTERVAL = 16;
constexpr float PLAYER_INITIAL_X = 200.0f;
constexpr float PLAYER_INITIAL_Y = 300.0f;
constexpr float PLAYER_SPEED_X = -8.0f;
constexpr float JUMP_FORCE = 12.0f;
constexpr int PIPE_INTERVAL_X = 300;
constexpr float PIPE_WIDTH = 60.0f;
constexpr float PIPE_HEIGHT = 180.0f;

// ============================================================================
// プレイヤークラス（インライン実装）
// ============================================================================

class Player {
public:
    static constexpr float WIDTH = 32.0f;
    static constexpr float HEIGHT = 32.0f;

private:
    float x_ = PLAYER_INITIAL_X;
    float y_ = PLAYER_INITIAL_Y;
    bool isJumping_ = false;

public:
    Player() : x_(PLAYER_INITIAL_X), y_(PLAYER_INITIAL_Y) {}

    void Initialize() {
        x_ = PLAYER_INITIAL_X;
        y_ = PLAYER_INITIAL_Y;
        isJumping_ = false;
    }

    void Update(float deltaTime) {
        // X 軸移動
        x_ += PLAYER_SPEED_X * deltaTime;

        // 画面左端を超えた場合のリセット
        if (x_ < -WIDTH / 2.0f) {
            x_ = DEFAULT_SCREEN_WIDTH + WIDTH / 2.0f;
        }

        // ジャンプ処理
        if (isJumping_) {
            y_ -= JUMP_FORCE * deltaTime;
            if (y_ <= 0) {
                y_ = 0;
                isJumping_ = false;
            }
        }
    }

    void Jump() {
        if (!isJumping_) {
            isJumping_ = true;
        }
    }

    void Draw() const {
        // プレイヤー本体（簡易描画）
        DxLib_SetColor(0xFFFF0000);  // 赤色
        DxLib_DrawRect(x_ - WIDTH / 2.0f, y_, 
                       x_ + WIDTH / 2.0f, y_ + HEIGHT, true);
        
        // 目（方向を示す）
        float eyeOffset = (x_ < 100) ? 4 : -4;
        DxLib_DrawCircle(x_ - WIDTH / 2.0f + 8 + eyeOffset, y_ + HEIGHT / 3.0f, 
                         3, true);
    }

    void Reset() { Initialize(); }
};

// ============================================================================
// パイプクラス（インライン実装）
// ============================================================================

class Pipe {
public:
    static constexpr float WIDTH = PIPE_WIDTH;
    static constexpr float HEIGHT = PIPE_HEIGHT;

private:
    float x_ = 0.0f;
    float y_ = 0.0f;
    bool isPassed_ = false;

public:
    Pipe() : x_(DEFAULT_SCREEN_WIDTH), 
             y_(DEFAULT_SCREEN_HEIGHT - PIPE_HEIGHT) {}

    void Initialize() {
        x_ = DEFAULT_SCREEN_WIDTH;
        y_ = DEFAULT_SCREEN_HEIGHT - PIPE_HEIGHT;
        isPassed_ = false;
    }

    void Update(float deltaTime, float& pipeIntervalX) {
        // X 軸移動
        x_ -= PIPE_SPEED_X * deltaTime;

        // パイプが画面を通過したか確認
        if (x_ + WIDTH < 0) {
            isPassed_ = true;
            Initialize();
        }

        // 間隔制御（簡易実装）
        pipeIntervalX -= PIPE_SPEED_X * deltaTime;
        if (pipeIntervalX <= 0) {
            pipeIntervalX = PIPE_INTERVAL_X;
            Initialize();
        }
    }

    void Draw() const {
        DxLib_SetColor(0xFF00FF00);  // 緑色
        
        // パイプ本体
        DxLib_DrawRect(x_, y_, x_ + WIDTH, y_ + HEIGHT, true);
        
        // パイプの縁（立体感）
        DxLib_DrawLine(x_, y_, x_ + WIDTH, y_, true);
        DxLib_DrawLine(x_, y_ + HEIGHT, x_ + WIDTH, y_ + HEIGHT, true);
    }

    void Reset() { isPassed_ = false; Initialize(); }
};

// ============================================================================
// ゲームメインクラス
// ============================================================================

class Game {
public:
    static constexpr float PLAYER_SPEED_X = -8.0f;
    static constexpr float PIPE_SPEED_X = 3.0f;

private:
    Player player_;
    Pipe pipe_;
    float pipeIntervalX_ = PIPE_INTERVAL_X;
    bool gameRunning_ = true;

public:
    Game() : player_(), pipe_() {}

    ~Game() = default;

    void Initialize() {
        player_.Initialize();
        pipe_.Initialize();
        pipeIntervalX_ = PIPE_INTERVAL_X;
        gameRunning_ = true;
    }

    void MainGame() {
        float deltaTime = 0.0f;
        
        while (gameRunning_) {
            Input();
            Update();
            Draw();
            
            DxLib_GetTime(&deltaTime);
            
            if (player_.y_ > DEFAULT_SCREEN_HEIGHT) {
                gameRunning_ = false;
            }
        }

        Finalize();
    }

private:
    void Input() {
        DxLib_EnableKeyRepeat(10);
        
        // ジャンプ（スペースキー）
        if (DxLib_IsKeyDown(DX_KEY_SPACE)) {
            player_.Jump();
        }

        // ゲーム終了（ESC キー）
        if (DxLib_IsKeyDown(DX_KEY_ESCAPE)) {
            gameRunning_ = false;
        }
    }

    void Update() {
        player_.Update(deltaTime, pipeIntervalX_);
        pipe_.Update(deltaTime, pipeIntervalX_);
    }

    void Draw() {
        // クリア画面
        DxLib_ClearScreen();
        
        // プレイヤー描画
        player_.Draw();
        
        // パイプ描画
        pipe_.Draw();
        
        // ステータス表示
        char status[64];
        sprintf_s(status, sizeof(status), "Score: %d", static_cast<int>(pipeIntervalX_));
        DxLib_DrawString(10, DEFAULT_SCREEN_HEIGHT - 30, status, true);
    }

    void Finalize() {
        // クリーンアップ
    }
};

// ============================================================================
// メイン関数
// ============================================================================

int main() {
    Game game;
    
    game.Initialize();
    game.MainGame();
    
    return 0;
}
