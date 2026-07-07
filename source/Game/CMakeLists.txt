// ============================================================================
// CMakeLists.txt - ビルド設定（CMake 使用時）
// ============================================================================

cmake_minimum_required(VERSION 3.10)

# プロジェクト設定
project(FlappyBird VERSION 1.0.0 LANGUAGES CXX)

# C++ 標準設定
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# DxLib の検索（Windows デフォルトパス）
find_path(DXLIB_INCLUDE_DIR
    NAMES "DxLib.h"
    PATHS
        "${CMAKE_SOURCE_DIR}/../.."
        "${CMAKE_SOURCE_DIR}/../../.."
        "C:/Program Files (x86)/DxLib/include"
        "$ENV{PROGRAMFILES}/DxLib/include"
    PATH_SUFFIXES include
)

# DxLib が見つからない場合は警告
if(NOT DXLIB_INCLUDE_DIR)
    message(WARNING "DxLib.h を見つかりませんでした。")
endif()

# ソースファイルのリスト
set(SOURCES
    main.cpp
    Game/MainGame.cpp
    Actor/Player/Player.cpp
    Actor/Pipe/Pipe.cpp
)

# ヘッダーファイルのリスト
set(HEADERS
    Define/Constants.h
    Actor/Player/Player.h
    Actor/Pipe/Pipe.h
    Game/MainGame.h
)

# 実行可能ファイルの作成
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# DxLib の検索結果をインクルードパスに追加
if(DXLIB_INCLUDE_DIR)
    target_include_directories(${PROJECT_NAME} PRIVATE 
        "${DXLIB_INCLUDE_DIR}"
    )
endif()

# 実行ファイルの出力先設定
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin"
)
