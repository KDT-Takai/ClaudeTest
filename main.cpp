/*
 * オセロゲーム - Visual Studio 2022 C++20
 * 単一ファイルでそのままビルド・実行可能
 */

#include <iostream>
#include <vector>
#include <string>
#include <conio.h>
#include <cstdlib>

// ============================================================================
// 定数定義
// ============================================================================

const int BOARD_SIZE = 8;
const int BLACK = 0;      // 黒石（先手）
const int WHITE = 1;      // 白石（後手）
const int EMPTY = 2;      // 空きマス

// 初期配置の中央座標
const int CENTER_X = 3;   // 黒石 1
const int CENTER_Y = 3;   // 黒石 2
const int CENTER_X_2 = 4; // 白石 1
const int CENTER_Y_2 = 4; // 白石 2

// ============================================================================
// ゲーム状態構造体
// ============================================================================

struct GameState {
    int board[BOARD_SIZE][BOARD_SIZE];   // 盤面（0:黒，1:白，2:空）
    int currentPlayer;                   // 現在のプレイヤー（0:黒，1:白）
    int blackScore;                      // 黒石の数
    int whiteScore;                      // 白石の数
    bool gameOver;                       // ゲーム終了フラグ
    int cursorX;                         // カーソル X 座標
    int cursorY;                         // カーソル Y 座標
};

// ============================================================================
// 描画関連関数
// ============================================================================

void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    std::cout << "\033[2J\033[H";
#endif
}

void drawBoard(const GameState& game) {
    // ヘッダー：プレイヤー情報とスコア
    std::string header = "┌─────────────────────────────────────────────────────┐\n";
    header += "│  オセロゲーム - スコア: ";
    
    // スコア計算（盤面から再計算）
    int bScore = 0, wScore = 0;
    for (int y = 0; y < BOARD_SIZE; y++) {
        for (int x = 0; x < BOARD_SIZE; x++) {
            if (game.board[y][x] == BLACK) bScore++;
            else if (game.board[y][x] == WHITE) wScore++;
        }
    }
    
    header += "黒(" + std::to_string(bScore) + ") vs 白(" + std::to_string(wScore) + ")\n│\n";
    header += "│  現在のプレイヤー: ";
    if (game.currentPlayer == BLACK) {
        header += "●（黒）";
    } else {
        header += "○（白）";
    }
    header += "\n└─────────────────────────────────────────────────────┘\n\n";
    
    // 盤面描画
    std::string topBorder = "┌";
    for (int i = 0; i < BOARD_SIZE * 3 - 1; i++) {
        topBorder += "─";
    }
    topBorder += "┐\n";
    
    // 行番号と列番号を表示
    std::string rowHeader = "   ";
    for (int x = 0; x < BOARD_SIZE; x++) {
        rowHeader += " ─";
    }
    rowHeader += "\n";
    // ローカル変数宣言（line のスコープを明確化）
    std::string lineBuffer;
    
    for (int y = 0; y < BOARD_SIZE; y++) {
        // 行番号と列番号
        lineBuffer = std::to_string(y + 1) + " │";
        
        // 各マスを描画
        for (int x = 0; x < BOARD_SIZE; x++) {
            int stone = game.board[y][x];
            
            if (stone == EMPTY) {
                lineBuffer += " ・";
            } else if (stone == BLACK) {
                lineBuffer += " ●";
            } else {
                lineBuffer += " ○";
            }
        }
        
        // 行番号と列番号の補完
        for (int x = 0; x < BOARD_SIZE; x++) {
            lineBuffer += " ─";
        }
        lineBuffer += "\n";
    }
    
    // 最後の行番号
    std::string bottomRowHeader = "   ";
    for (int x = 0; x < BOARD_SIZE; x++) {
        bottomRowHeader += " ─";
    }
    bottomRowHeader += "\n";
    
    // フッター
    std::string footer = "┌─────────────────────────────────────────────────────┐\n";
    footer += "│  矢印キーで移動 | Enter/Space で石を置く | q で終了\n";
    footer += "└─────────────────────────────────────────────────────┘\n";
    
    // すべて表示
    std::cout << header;
    std::cout << topBorder;
    std::cout << rowHeader;
    for (int y = 0; y < BOARD_SIZE; y++) {
        std::cout << lineBuffer;
    }
    std::cout << bottomRowHeader;
    std::cout << footer;
}

// ============================================================================
// 入力処理関数
// ============================================================================

bool handleInput(GameState& game) {
    // キー入力待ち（ブロック）
    int key = _getch();
    
    if (key == 'q' || key == 'Q') {
        std::cout << "\nゲームを終了します。\n";
        return true;  // 終了フラグ
    }
    
    else if (key == 13 || key == 32) {  // Enter または Space
        placeStone(game);
        return false;
    }
    
    else if (key >= 75 && key <= 80) {  // 矢印キー（ASCII: K,L,J,H）
        switch (key) {
            case 75:  // ↑
                game.cursorY--;
                break;
            case 80:  // ↓
                game.cursorY++;
                break;
            case 76:  // ←
                game.cursorX--;
                break;
            case 72:  // →
                game.cursorX++;
                break;
        }
        
        // 範囲チェックと巻き戻し
        if (game.cursorY < 0) game.cursorY = BOARD_SIZE - 1;
        else if (game.cursorY >= BOARD_SIZE) game.cursorY = 0;
        if (game.cursorX < 0) game.cursorX = BOARD_SIZE - 1;
        else if (game.cursorX >= BOARD_SIZE) game.cursorX = 0;
    }
    
    return false;
}

// ============================================================================
// ゲームロジック関数
// ============================================================================

void initializeBoard(GameState& game) {
    // 盤面を初期化（すべて空きマス）
    for (int y = 0; y < BOARD_SIZE; y++) {
        for (int x = 0; x < BOARD_SIZE; x++) {
            game.board[y][x] = EMPTY;
        }
    }
    
    // 初期配置：中央に交互に配置
    game.board[CENTER_Y][CENTER_X]     = BLACK;  // (3,3) - 黒
    game.board[CENTER_Y_2][CENTER_X_2] = WHITE;  // (4,4) - 白
    game.board[CENTER_Y][CENTER_X_2]   = WHITE;  // (3,4) - 白
    game.board[CENTER_Y_2][CENTER_X]   = BLACK;  // (4,3) - 黒
    
    // スコア初期化
    game.blackScore = 2;
    game.whiteScore = 2;
    
    // プレイヤー設定：黒から開始
    game.currentPlayer = BLACK;
    game.gameOver = false;
}

// 指定したマスに石を置く
void placeStone(GameState& game) {
    int x = game.cursorX;
    int y = game.cursorY;
    
    // 同じ場所に既に石がある場合はスキップ
    if (game.board[y][x] != EMPTY) {
        return;
    }
    
    // 石を置く
    game.board[y][x] = game.currentPlayer;
    
    // スコア更新
    if (game.currentPlayer == BLACK) {
        game.blackScore++;
    } else {
        game.whiteScore++;
    }
    
    // 反転処理
    flipStones(game);
}

// 8 方向のベクトル定義
int dx[] = {-1, -1, -1, 0, 0, 1, 1, 1};
int dy[] = {-1, 0, 1, -1, 1, -1, 0, 1};

// 指定した方向に相手の石を挟めるか判定し、反転させる
bool tryFlipDirection(GameState& game, int x, int y) {
    int dirX = dx[game.currentPlayer];  // 現在のプレイヤーによる方向決定
    int dirY = dy[game.currentPlayer];
    
    // 逆方向から相手の石を探す
    for (int i = -1; i >= -8; i--) {
        int nx = x + i * dirX;
        int ny = y + i * dirY;
        
        if (nx < 0 || nx >= BOARD_SIZE || ny < 0 || ny >= BOARD_SIZE) break;
        
        if (game.board[ny][nx] == game.currentPlayer) {
            // 自分の石に到達、反転完了
            return true;
        } else if (game.board[ny][nx] == EMPTY) {
            // 空きマスに到達、この方向は失敗
            break;
        } else {
            // 相手の石、反転可能
            game.board[ny][nx] = game.currentPlayer;
        }
    }
    
    return false;
}

// すべての方向で反転処理を行う
void flipStones(GameState& game) {
    int flippedCount = 0;
    
    // 8 方向すべてを試す
    for (int i = 0; i < 8; i++) {
        if (tryFlipDirection(game, game.cursorX, game.cursorY)) {
            flippedCount++;
        }
    }
    
    // 反転があった場合はターンを切り替える
    if (flippedCount > 0) {
        switchPlayer(game);
    }
}

// ターンを切り替える
void switchPlayer(GameState& game) {
    game.currentPlayer = (game.currentPlayer == BLACK) ? WHITE : BLACK;
}

// 現在プレイヤーが置ける場所があるか判定
bool hasValidMove(GameState& game, int player) {
    for (int y = 0; y < BOARD_SIZE; y++) {
        for (int x = 0; x < BOARD_SIZE; x++) {
            if (game.board[y][x] == EMPTY) {
                // このマスに石を置いた場合、反転できるか試す
                game.board[y][x] = player;
                
                int flippedCount = 0;
                for (int i = 0; i < 8; i++) {
                    if (tryFlipDirection(game, x, y)) {
                        flippedCount++;
                    }
                }
                
                // 元の状態に戻す
                game.board[y][x] = EMPTY;
                
                if (flippedCount > 0) return true;
            }
        }
    }
    
    return false;
}

// パス判定と処理
void handlePass(GameState& game) {
    bool blackCanMove = hasValidMove(game, BLACK);
    bool whiteCanMove = hasValidMove(game, WHITE);
    
    if (!blackCanMove && !whiteCanMove) {
        // 両者とも置けない：ゲーム終了
        std::cout << "\n盤面が埋まりました！\n";
        game.gameOver = true;
    } else if (game.currentPlayer == BLACK && !blackCanMove) {
        // 黒がパス
        switchPlayer(game);
    } else if (game.currentPlayer == WHITE && !whiteCanMove) {
        // 白がパス
        switchPlayer(game);
    }
}

// メイン関数
// ============================================================================

void placeStone(GameState& game);
void flipStones(GameState& game);
void switchPlayer(GameState& game);
void endGame(const GameState& game);

int main() {
    // ゲーム状態初期化
    GameState game;
    initializeBoard(game);

    // カーソル位置を中央に設定
    game.cursorX = CENTER_X;
    game.cursorY = CENTER_Y;

    // メインゲームループ
    while (!game.gameOver) {
        clearScreen();

        drawBoard(game);

        if (handleInput(game)) {
            break;  // 終了フラグが立った場合、ループから抜ける
        }

        // パス判定（置ける場所がない場合は自動パス）
        handlePass(game);
    }

    // ゲーム終了処理
    endGame(game);

    return 0;
}

// ゲーム終了処理関数
void endGame(const GameState& game) {
    clearScreen();
    
    std::cout << "\n========================================\n";
    std::cout << "       オセロゲームが終了しました        \n";
    std::cout << "========================================\n\n";
    
    // 最終スコア表示
    int bScore = 0, wScore = 0;
    for (int y = 0; y < BOARD_SIZE; y++) {
        for (int x = 0; x < BOARD_SIZE; x++) {
            if (game.board[y][x] == BLACK) bScore++;
            else if (game.board[y][x] == WHITE) wScore++;
        }
    }
    
    std::cout << "最終スコア:\n";
    std::cout << "  黒（先手）: " << bScore << " 石\n";
    std::cout << "  白（後手）: " << wScore << " 石\n\n";
    
    // 勝敗判定
    if (bScore > wScore) {
        std::cout << "結果：黒の勝ち！\n";
    } else if (wScore > bScore) {
        std::cout << "結果：白の勝ち！\n";
    } else {
        std::cout << "結果：引き分け\n";
    }
    
    std::cout << "\n========================================\n";
    std::cout << "       再度実行するには q を押してください\n";
    std::cout << "========================================\n";
}
