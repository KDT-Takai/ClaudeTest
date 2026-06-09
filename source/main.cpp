#include "CLI.h"
#include <thread>
#include <chrono>

int main() {
    // --- テーマ設定 ---
    
    // デフォルトテーマ（青いヘッダー）
    Theme defaultTheme;
    
    // カスタムテーマ例：緑色で、セパレーターを黄色に
    Theme customTheme;
    customTheme.header_bg = Colors::Color::BG_Green;
    customTheme.header_text = Colors::Color::BrightWhite;
    customTheme.separator = Colors::Color::Yellow;
    
    // --- CLI インスタンス作成 ---
    
    // デフォルトテーマで初期化
    CLI cli(defaultTheme);
    
    // カスタムテーマに変更
    cli.setTheme(customTheme);
    
    // --- 基本機能のデモ ---
    
    // ヘッダー表示
    cli.printHeader("CLI Demo", 40);
    
    // セパレーター
    cli.newline();
    cli.printSeparator(50);
    cli.newline();
    
    // インフォメーション
    cli.info("こんにちは！色付き CLI クラスのデモです。");
    
    // ワーニング
    cli.warning("注意：これはテストモードです。");
    
    // エラー
    cli.error("エラーが発生しました（テスト用）。");
    
    // サークセス
    cli.success("処理が正常に完了しました！");
    
    // 通常テキスト
    cli.newline();
    cli.print("通常の出力：これは色なしのテキストです。");
    cli.newline();
    
    // --- メニュー機能のデモ ---
    
    cli.printHeader("サンプルメニュー", 40);
    
    std::vector<std::string> options = {
        "オプション A",
        "オプション B", 
        "オプション C"
    };
    
    int selection = cli.selectFromMenu(options);
    
    if (selection >= 0) {
        cli.info("選択された項目：" + std::to_string(selection + 1));
    } else {
        cli.warning("キャンセルされました。");
    }
    
    // --- 進捗バーのデモ ---
    
    cli.printHeader("進捗バー", 30);
    
    for (int i = 0; i <= 5; ++i) {
        cli.progressBar(i, 5, "処理中:");
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
    }
    
    // --- バージョン情報のデモ ---
    
    cli.printHeader("バージョン", 30);
    cli.info("バージョン 1.0.0 (beta)");
    cli.info("ビルド日：2024-06-09");
    
    // --- 入力プロンプトのデモ ---
    
    cli.printHeader("入力プロンプト", 30);
    
    int age = cli.prompt<int>("年齢", "例: 25", 18);
    std::string name = cli.prompt<std::string>("名前", "", "");
    bool agree = cli.prompt<bool>("同意する？", "(Y/n)", false);
    
    // --- まとめ ---
    
    cli.printHeader("まとめ", 40);
    cli.info("以下の機能を実装しました:");
    cli.newline();
    cli.info("  ✓ ヘッダー/セパレーター表示");
    cli.info("  ✓ 色付きメッセージ（info/warning/error/success）");
    cli.info("  ✓ カスタマイズ可能なテーマ");
    cli.info("  ✓ メニュー選択機能");
    cli.info("  ✓ 進捗バー表示");
    cli.info("  ✓ 入力プロンプト（型自動推論付き）");
    cli.newline();
    
    Colors::colored("カスタムテーマ例:", Colors::Color::Cyan);
    cli.newline();
    cli.print("  ヘッダー背景:", Colors::escape(customTheme.header_bg), 
               "文字色:", Colors::escape(customTheme.header_text));
    cli.newline();
    cli.print("  セパレーター:", Colors::escape(customTheme.separator));
    
    return 0;
}
