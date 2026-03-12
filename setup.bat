@echo off
chcp 65001 > nul
echo 🚀 プロジェクトの規模を選択してください:
echo 1) 小規模 (Small) - 1ペライチの簡単な設計書のみ
echo 2) 中・大規模 (Standard) - 詳細なフルセット設計書
set /p size="番号を入力 (1 or 2): "

if "%size%"=="1" (
    rmdir /s /q docs\standard 2>nul
    move docs\small\* docs\ >nul
    rmdir /s /q docs\small 2>nul
    echo ✅ 小規模プロジェクト用のドキュメントをセットアップしました！
) else if "%size%"=="2" (
    rmdir /s /q docs\small 2>nul
    move docs\standard\* docs\ >nul
    rmdir /s /q docs\standard 2>nul
    echo ✅ 中・大規模プロジェクト用のドキュメントをセットアップしました！
) else (
    echo ❌ 1 か 2 を入力してください。終了します。
    pause
    exit /b 1
)

:: Mac/Linux用のスクリプトも不要になるので道連れにして削除
if exist setup.sh del setup.sh

echo 🎉 セットアップ完了！さあ、開発を始めましょう。
pause

:: 自分自身(setup.bat)を削除して完全に自壊する魔法のコマンド
(goto) 2>nul & del "%~f0"