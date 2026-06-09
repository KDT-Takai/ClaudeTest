#ifndef CLI_H
#define CLI_H

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <type_traits>
#include <utility>

// 色定義
namespace Colors {
    enum class Color : int {
        // 通常色
        Black = 0, Red, Green, Yellow, Blue, Magenta, Cyan, White,
        // 強調色 (bold)
        BrightBlack, BrightRed, BrightGreen, BrightYellow, 
        BrightBlue, BrightMagenta, BrightCyan, BrightWhite,
        // 背景色
        BG_Black = 8, BG_Red, BG_Green, BG_Yellow, BG_Blue, BG_Magenta, BG_Cyan, BG_White,
        // 強調背景色 (bright bg)
        BRIGHT_BG_Black = 9, BRIGHT_BG_Red, BRIGHT_BG_Green, BRIGHT_BG_Yellow, 
        BRIGHT_BG_Blue, BRIGHT_BG_Magenta, BRIGHT_BG_Cyan, BRIGHT_BG_White
    };

    // ANSI エスケープコードを生成する関数
    static inline std::string escape(Color c) {
        static const std::map<Color, int> codes = {
            {Color::Black, 30},   {Color::Red, 31},   {Color::Green, 32},
            {Color::Yellow, 33},  {Color::Blue, 34},   {Color::Magenta, 35},
            {Color::Cyan, 36},    {Color::White, 37},
            {Color::BrightBlack, 90}, {Color::BrightRed, 91}, {Color::BrightGreen, 92},
            {Color::BrightYellow, 93}, {Color::BrightBlue, 94}, {Color::BrightMagenta, 95},
            {Color::BrightCyan, 96}, {Color::BrightWhite, 97},
            {Color::BG_Black, 40},  {Color::BG_Red, 41},  {Color::BG_Green, 42},
            {Color::BG_Yellow, 43}, {Color::BG_Blue, 44}, {Color::BG_Magenta, 45},
            {Color::BG_Cyan, 46},   {Color::BG_White, 47},
            {Color::BRIGHT_BG_Black, 100}, {Color::BRIGHT_BG_Red, 101}, {Color::BRIGHT_BG_Green, 102},
            {Color::BRIGHT_BG_Yellow, 103}, {Color::BRIGHT_BG_Blue, 104}, 
            {Color::BRIGHT_BG_Magenta, 105}, {Color::BRIGHT_BG_Cyan, 106},
            {Color::BRIGHT_BG_White, 107}
        };

        auto it = codes.find(c);
        return (it != codes.end()) ? "\033[" + std::to_string(it->second) + "m" : "";
    }

    // リセットコード（定数）- 修正：static inline で宣言
    static inline const std::string RESET_CODE = "\033[0m";

    // 色付きテキストを生成するヘルパー関数 - 修正：明示的な戻り値
    template<typename T>
    static inline std::string colored(T text, Color c) {
        return escape(c) + text + RESET_CODE;
    }

    template<typename T>
    static inline std::string colored(T text, Color c1, T text2, Color c2) {
        return escape(c1) + text + escape(c2) + text2 + RESET_CODE;
    }

    static inline std::string colored(const std::string& text1, Color c1, const std::string& text2, Color c2, const std::string& text3) {
        return escape(c1) + text1 + escape(c2) + text2 + escape(c1) + text3 + RESET_CODE;
    }

    // 背景色付きテキスト - 修正：明示的な戻り値
    template<typename T>
    static inline std::string bgcolored(T text, Color c) {
        return escape(c) + text + RESET_CODE;
    }

    // 色付きテキストを出力するヘルパー関数 - 修正：std::cout を直接出力
    static inline void print(const std::string& text, Color c) {
        std::cout << colored(text, c);
    }

    // 色付きテキストを出力するヘルパー関数（複数色）- 修正：std::cout を直接出力
    template<typename T>
    static inline void print(const std::string& text1, Color c1, const std::string& text2, Color c2) {
        std::cout << colored(text1, c1, text2, c2);
    }

    // 色付きテキストを出力するヘルパー関数（背景色）- 修正：std::cout を直接出力
    static inline void print(const std::string& text, Color fg, Color bg) {
        std::cout << escape(bg) << escape(fg) << text << RESET_CODE;
    }


}

// テーマ設定 - 修正：static メソッドを使用可能に
struct Theme {
    Colors::Color header_bg = Colors::Color::BG_Blue;      // ヘッダー背景色
    Colors::Color header_text = Colors::Color::BrightWhite; // ヘッダー文字色
    Colors::Color separator = Colors::Color::Cyan;          // セパレーター色
    Colors::Color info = Colors::Color::Green;              // インフォメーション色
    Colors::Color warning = Colors::Color::Yellow;          // ワーニング色
    Colors::Color error = Colors::Color::Red;               // エラー色
    Colors::Color success = Colors::Color::BrightGreen;     // サークセス色
    
    // 修正：static メソッドとしてインスタンス化可能に
    static Theme defaultTheme() {
        return {};
    }
};

// CLI クラス - 修正：明示的なコンストラクタ引数
class CLI {
private:
    Theme theme_;
    bool initialized_ = false;
    
public:
    // コンストラクタでテーマを設定する - 修正：static メソッドを使用
    explicit CLI(const Theme& t = Theme::defaultTheme());

    // 初期化（ANSI コード対応の確認）- 修正：明示的な戻り値なし
    void initialize();

    // テーマの設定と変更 - 修正：const 参照を使用
    void setTheme(const Theme& t);
    
    // インスタンスから現在のテーマを取得 - 修正：const メソッド
    const Theme& getTheme() const { return theme_; }

    // ヘッダー表示 - 修正：明示的な戻り値なし
    void printHeader(const std::string& title, int width = 60);

    // セパレーター表示 - 修正：明示的な戻り値なし
    void printSeparator(int width = 60);

    // インフォメーションメッセージ - 修正：std::cout を直接出力
    void info(const std::string& message);

    // ワーニングメッセージ - 修正：std::cout を直接出力
    void warning(const std::string& message);

    // エラーメッセージ - 修正：std::cout を直接出力
    void error(const std::string& message);

    // サークセスメッセージ - 修正：std::cout を直接出力
    void success(const std::string& message);

    // 通常テキスト出力（色なし）- 修正：std::cout を直接出力
    void print(const std::string& text);

    // 改行 - 修正：明示的な戻り値なし
    void newline();

    template<typename T>
    T prompt(const std::string& label, const std::string& placeholder, T defaultValue);

    // 入力プロンプト - 修正：const 参照を使用
    template<typename T>
    T prompt(const std::string& label, T defaultValue = T()) {
        return prompt<T>(label, "", defaultValue);
    }

    void printMenu(const std::vector<std::pair<std::string, int>>& menu);

    void drawMenuItem(int index, const std::string& text);

    // メニュー選択 - 修正：明示的な戻り値なし
    int selectFromMenu(const std::vector<std::string>& options);

    // 進捗バー表示 - 修正：明示的な戻り値なし
    void progressBar(int current, int total, const std::string& label);

    // バージョン情報表示 - 修正：明示的な戻り値なし
    void printVersionInfo(const std::string& version, const std::string& buildDate = "");
};

#endif // CLI_H
