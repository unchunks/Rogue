cmake_minimum_required(VERSION 3.15)
project(RogueGame VERSION 1.0 LANGUAGES CXX)

# C++標準の設定 (一般的なC++17を指定。必要に応じて変更してください)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# コンパイラごとのフラグ設定 (Makefileの -Wall に相当)
if(MSVC)
    # Windows/Visual Studio の場合
    add_compile_options(/W3 /utf-8)
else()
    # GCC / Clang の場合
    add_compile_options(-Wall -Wextra)
endif()

# SDL2と拡張ライブラリの検索
# ※Windowsではパッケージマネージャ(vcpkg等)の利用を想定しています
find_package(SDL2 CONFIG REQUIRED)
find_package(SDL2_image CONFIG REQUIRED)
find_package(SDL2_ttf CONFIG REQUIRED)
find_package(SDL2_mixer CONFIG REQUIRED)

# ソースファイルの収集 (Makefileの OBSRC = src/*.cpp に相当)
# サブディレクトリも含むように GLOB_RECURSE を使用しています
file(GLOB_RECURSE SOURCES "src/*.cpp")

# メイン実行ファイルのターゲット (Makefileの OBJ_NAME = rogue に相当)
add_executable(rogue ${SOURCES})

# インクルードディレクトリの指定 (-I./src に相当)
target_include_directories(rogue PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)

# ライブラリのリンク (Makefileの -lSDL2 などに相当)
target_link_libraries(rogue PRIVATE
    SDL2::SDL2main
    SDL2::SDL2
    SDL2_image::SDL2_image
    SDL2_ttf::SDL2_ttf
    SDL2_mixer::SDL2_mixer
)

# 【重要】実行ファイルと同じ階層に assets フォルダを自動コピーする
# ゲーム起動時に画像や音声等のパスエラーを防ぐための処理です
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/assets")
    add_custom_command(TARGET rogue POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_SOURCE_DIR}/assets"
        "$<TARGET_FILE_DIR:rogue>/assets"
    )
endif()

# --- テスト用ターゲットのビルド (Makefileの test に相当) ---
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/tests/test_AinB.cpp")
    add_executable(test_run tests/test_AinB.cpp)
    target_include_directories(test_run PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/tests/include
        ${CMAKE_CURRENT_SOURCE_DIR}/src
    )
    # ターミナルから `cmake --build . --target run_test` でテストを実行可能に
    add_custom_target(run_test
        COMMAND test_run
        DEPENDS test_run
        WORKING_DIRECTORY "$<TARGET_FILE_DIR:test_run>"
    )
endif()
