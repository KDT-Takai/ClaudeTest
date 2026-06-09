#include "CLI.h"
#include <windows.h>
#include <thread>
#include <chrono>

// コンストラクタ - 修正：明示的な初期化リスト
CLI::CLI(const Theme& t) : theme_(t), initialized_(false) {}

// 初期化 - 修正：明示的な戻り値なし
void CLI::initialize() {
    if (!initialized_) {
        // ANSI コード対応の確認（Windows では ENABLE_VIRTUAL_TERMINAL_PROCESSING を設定）
        HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
        DWORD mode = 0;
        if (GetConsoleMode(hOut, &mode) && !(mode & ENABLE_VIRTUAL_TERMINAL_PROCESSING)) {
            SetConsoleMode(hOut, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING);
        }
        initialized_ = true;
    }
}

// テーマの設定と変更 - 修正：const 参照を使用
void CLI::setTheme(const Theme& t) {
    theme_ = t;
}

// ヘッダー表示 - 修正：明示的な戻り値なし
void CLI::printHeader(const std::string& title, int width) {
    initialize();
    
    // 背景色で囲むヘッダー - 修正：std::cout を直接出力
    printSeparator(width + 4);
    Colors::bgcolored(title, theme_.header_bg);
    newline();
    printSeparator(width + 4);
}

// セパレーター表示 - 修正：明示的な戻り値なし
void CLI::printSeparator(int width) {
    initialize();
    
    // 色付きのセパレーター - 修正：std::cout を直接出力
    std::string sep = Colors::escape(theme_.separator) + 
                     "╔" + std::string(width - 2, '-') + "╗";
    print(sep);
}

// インフォメーションメッセージ - 修正：std::cout を直接出力
void CLI::info(const std::string& message) {
    initialize();
    Colors::colored(message, theme_.info);
    newline();
}

// ワーニングメッセージ - 修正：std::cout を直接出力
void CLI::warning(const std::string& message) {
    initialize();
    Colors::bgcolored(message, theme_.warning);
    newline();
}

// エラーメッセージ - 修正：std::cout を直接出力
void CLI::error(const std::string& message) {
    initialize();
    Colors::colored(message, theme_.error);
    newline();
}

// サークセスメッセージ - 修正：std::cout を直接出力
void CLI::success(const std::string& message) {
    initialize();
    Colors::bgcolored(message, theme_.success);
    newline();
}

// 通常テキスト出力（色なし）- 修正：std::cout を直接出力
void CLI::print(const std::string& text) {
    initialize();
    // 修正：Colors::RESET_CODE を追加して出力
    std::cout << text + Colors::RESET_CODE;
}

// 改行 - 修正：明示的な戻り値なし
void CLI::newline() {
    initialize();
    std::cout << "\n";
}

// 入力プロンプトの実装 - 修正：const 参照を使用
template<typename T>
T CLI::prompt(const std::string& label, const std::string& placeholder, T defaultValue) {
    initialize();
    
    // ラベルとプレースホルダーを表示 - 修正：std::cout を直接出力
    print(label + ": ");
    if (!placeholder.empty()) {
        Colors::colored(placeholder, theme_.info);
    }
    newline();
    
    // デフォルト値がある場合は表示 - 修正：明示的な文字列連結
    std::string defaultStr;
    if (defaultValue != T() && !std::is_same<T, bool>::value) {
        defaultStr = " [デフォルト: " + Colors::escape(theme_.info) + 
                      std::to_string(defaultValue) + Colors::RESET_CODE + "]";
    } else if (defaultValue == T()) {
        defaultStr = " [Enter で OK]";
    }
    
    // 入力を受け付ける - 修正：明示的な変数宣言
    T result;
    if (!defaultStr.empty() && defaultValue != T()) {
        std::string input;
        std::getline(std::cin, input);
        
        if (input.empty()) {
            result = defaultValue;
        } else {
            try {
                // 型に応じたパース - 修正：if constexpr で最適化
                if constexpr (std::is_same<T, int>::value || 
                             std::is_same<T, long>::value) {
                    result = static_cast<T>(std::stol(input));
                } else if constexpr (std::is_same<T, double>::value) {
                    result = static_cast<T>(std::stod(input));
                } else if constexpr (std::is_same<T, bool>::value) {
                    std::string lower = input;
                    std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);
                    result = (lower == "y" || lower == "yes") ? true : false;
                } else if constexpr (std::is_same<T, std::string>::value) {
                    result = input;
                } else if constexpr (std::is_floating_point<T>::value) {
                    // 浮動小数点数の追加処理 - 修正：明示的な変数宣言
                    result = static_cast<T>(std::stod(input));
                } else {
                    // 汎用的なパース（文字列として）- 修正：明示的な変数宣言
                    std::string numStr = input;
                    size_t pos;
                    T parsed = static_cast<T>(std::stol(numStr, &pos));
                    
                    if (pos == numStr.length()) {
                        result = parsed;
                    } else {
                        // 文字列として扱う - 修正：明示的な変数宣言
                        result = static_cast<T>(input);
                    }
                }
            } catch (...) {
                Colors::colored("入力エラー。デフォルト値を使用します。", theme_.warning);
                newline();
                result = defaultValue;
            }
        }
    } else if constexpr (std::is_same<T, bool>::value) {
        // bool の場合は Y/n で確認 - 修正：明示的な変数宣言
        std::string input;
        while (true) {
            Colors::colored("(Y/n)", theme_.info);
            newline();
            std::getline(std::cin, input);
            
            if (input.empty() || input[0] == 'y' || input[0] == 'Y') {
                result = true;
                break;
            } else if (input[0] == 'n' || input[0] == 'N') {
                result = false;
                break;
            }
        }
    } else {
        // 汎用的なパース（文字列として）- 修正：明示的な変数宣言
        std::string input;
        std::getline(std::cin, input);
        
        if (input.empty()) {
            result = defaultValue;
        } else {
            try {
                if constexpr (std::is_floating_point<T>::value) {
                    result = static_cast<T>(std::stod(input));
                } else {
                    // 整数や文字列としてパース - 修正：明示的な変数宣言
                    std::string numStr = input;
                    size_t pos;
                    T parsed = static_cast<T>(std::stol(numStr, &pos));
                    
                    if (pos == numStr.length()) {
                        result = parsed;
                    } else {
                        // 文字列として扱う - 修正：明示的な変数宣言
                        result = static_cast<T>(input);
                    }
                }
            } catch (...) {
                Colors::colored("入力エラー。デフォルト値を使用します。", theme_.warning);
                newline();
                result = defaultValue;
            }
        }
    }
    
    return result;
}

// メニュー表示 - 修正：明示的な戻り値なし
void CLI::printMenu(const std::vector<std::pair<std::string, int>>& menu) {
    initialize();
    
    printHeader("メニュー", 40);
    
    for (size_t i = 0; i < menu.size(); ++i) {
        drawMenuItem(i, menu[i].first);
    }
}

// メニュー描画のヘルパー - 修正：明示的な戻り値なし
void CLI::drawMenuItem(int index, const std::string& text) {
    // インデックス番号 - 修正：std::cout を直接出力
    Colors::print("  ", theme_.separator);
    
    // 選択可能な番号 - 修正：明示的な変数宣言
    if (index == 0) {
        Colors::colored("[1]", theme_.success);
    } else if (index == 1) {
        Colors::colored("[2]", theme_.success);
    } else if (index == 2) {
        Colors::colored("[3]", theme_.success);
    } else {
        Colors::colored("[" + std::to_string(index + 1) + "]", theme_.info);
    }
    
    // テキスト - 修正：std::cout を直接出力
    print(text, theme_.header_text);
    newline();
}

// メニュー選択 - 修正：明示的な戻り値なし
int CLI::selectFromMenu(const std::vector<std::string>& options) {
    initialize();
    
    if (options.empty()) return -1;
    
    printHeader("メニュー", 40);
    
    for (size_t i = 0; i < options.size(); ++i) {
        Colors::print("  ", theme_.separator);
        
        // インデックス番号 - 修正：明示的な変数宣言
        std::string numStr = std::to_string(i + 1);
        if (options.size() <= 3) {
            Colors::colored("[", theme_.success, i + 1, "]");
        } else {
            Colors::colored("[" + numStr + "]", theme_.info);
        }
        
        // テキスト - 修正：std::cout を直接出力
        print(options[i], theme_.header_text);
        newline();
    }
    
    // 選択を促すメッセージ - 修正：明示的な文字列連結
    Colors::print("\n", theme_.separator);
    Colors::colored("選択してください（1-", theme_.info, 
                   std::to_string(options.size()), "、q でキャンセル）\n\n");
    
    int selection = -1;
    while (true) {
        std::string input;
        std::getline(std::cin, input);
        
        if (input.empty() || input[0] == 'q' || input[0] == 'Q') {
            return -1; // キャンセル
        } else if (!input.empty()) {
            try {
                selection = static_cast<int>(std::stol(input));
                
                // 範囲チェック - 修正：明示的な変数宣言
                if (selection >= 1 && selection <= static_cast<int>(options.size())) {
                    break;
                } else {
                    Colors::colored("無効な選択。", theme_.warning);
                    newline();
                }
            } catch (...) {
                Colors::colored("入力エラー。数字を入力してください。", theme_.warning);
                newline();
            }
        }
    }
    
    return selection - 1; // 0 インデックスに変換
}

// 進捗バー - 修正：明示的な戻り値なし
void CLI::progressBar(int current, int total, const std::string& label) {
    initialize();
    
    if (total <= 0) return;
    
    // 進行度計算 - 修正：明示的な変数宣言
    double progress = static_cast<double>(current) / total;
    int barWidth = 20;
    int filled = static_cast<int>(barWidth * progress);
    
    // バー描画 - 修正：std::cout を直接出力
    std::string bar = "[" + 
                    std::string(filled, '#') + 
                    std::string(barWidth - filled, '-') + "]";
    
    // ラベルとバーを表示 - 修正：std::cout を直接出力
    print(label, theme_.info);
    Colors::print(" ", theme_.separator);
    Colors::colored(bar, progress < 0.5 ? theme_.warning : theme_.success);
    
    // 進捗率（パーセント）- 修正：明示的な変数宣言
    int percent = static_cast<int>(progress * 100);
    print(" ", theme_.separator);
    Colors::colored(percent, theme_.info);
    newline();
}

// バージョン情報表示 - 修正：明示的な戻り値なし
void CLI::printVersionInfo(const std::string& version, const std::string& buildDate) {
    initialize();
    
    printHeader("バージョン", 30);
    
    // バージョン - 修正：std::cout を直接出力
    Colors::colored(version, theme_.header_text);
    newline();
    
    // ビルド日付（あれば）- 修正：明示的な変数宣言
    if (!buildDate.empty()) {
        Colors::colored(buildDate, theme_.info);
        newline();
    }
}
