# ログ設計（C-portfolio）

本ドキュメントは `C-portfolio` で記録すべき操作・イベントとそのログレベルを定義します。

---

## 1. 現状の確認

### 現在のログ出力

本プロジェクトは現時点で以下の出力のみ実装されています。

| 場所 | 出力先 | 内容 |
|------|--------|------|
| `main.c:10` | `stderr` | `dist/index.html` のオープン失敗 |
| `main.c:42` | `stdout` | `"生成完了: dist/index.html"` |
| `render.c:49` | `stderr` | `checked_snprintf` への不正引数 |
| `render.c:58` | `stderr` | `vsnprintf` の失敗 |
| `render.c:63-68` | `stderr` | バッファ上限超過（コンテキスト名・バッファサイズ・必要サイズを含む） |

### 現在ないもの

- ログレベルの分類（info / warn / error）
- 専用ログファイルへの出力
- タイムスタンプ付きのログ行
- ビルド全体のサマリー出力

---

## 2. ログレベル定義

本プロジェクトのビルドツール（C言語製CLIジェネレーター）として適切な3段階を定義します。

| レベル | 記号 | 出力先 | 意味 |
|--------|------|--------|------|
| `INFO` | `[INFO]` | `stdout` | 正常な処理の進行状況。生成完了通知など |
| `WARN` | `[WARN]` | `stderr` | 処理は続行できるが注意が必要な状態。バッファ上限超過など |
| `ERROR` | `[ERROR]` | `stderr` | 処理が中断または失敗した状態。ファイルオープン失敗など |

---

## 3. 記録対象イベントとレベル

### 3.1. ビルド・ファイル操作

| イベント | レベル | 現状 | 推奨メッセージ例 |
|---------|--------|------|----------------|
| 出力ファイルのオープン成功 | INFO | 未実装 | `[INFO] 出力先を開きました: dist/index.html` |
| 出力ファイルのオープン失敗 | ERROR | 実装済み (`stderr`) | `[ERROR] dist/index.html を開けません` |
| HTML生成の完了 | INFO | 実装済み (`stdout`・レベルなし) | `[INFO] 生成完了: dist/index.html` |
| ファイルのクローズ失敗 | WARN | 未実装 | `[WARN] fclose に失敗しました: dist/index.html` |

### 3.2. データ・レンダリング処理

| イベント | レベル | 現状 | 推奨メッセージ例 |
|---------|--------|------|----------------|
| 不正な引数を検出 | ERROR | 実装済み (`stderr`・レベルなし) | `[ERROR] checked_snprintf の引数が不正です` |
| `vsnprintf` の実行失敗 | ERROR | 実装済み (`stderr`・レベルなし) | `[ERROR] [コンテキスト名] の snprintf で失敗しました` |
| バッファ上限超過（出力が切り詰められた） | WARN | 実装済み (`stderr`・レベルなし) | `[WARN] [コンテキスト名] がバッファ上限を超過しました (buf=8192, required=N)` |
| `html_escape` に NULL ポインタを渡した | WARN | 未実装（サイレント復帰） | `[WARN] html_escape に NULL ポインタが渡されました` |
| プロジェクト・キャリア項目の描画スキップ | WARN | 未実装 | `[WARN] projects[N] の描画をスキップしました` |

### 3.3. セクション描画の進行

> 現在は実装不要ですが、デバッグ時に有用なログとして記録します。

| イベント | レベル | 推奨メッセージ例 |
|---------|--------|----------------|
| `render_head` 開始・完了 | INFO | `[INFO] render_head: 完了` |
| `render_profile` 開始・完了 | INFO | `[INFO] render_profile: 完了 (social_links=3)` |
| `render_skills` 開始・完了 | INFO | `[INFO] render_skills: 完了` |
| `render_career` 開始・完了 | INFO | `[INFO] render_career: 完了 (events=4)` |
| `render_projects` 開始・完了 | INFO | `[INFO] render_projects: 完了 (projects=4, categories=3)` |

---

## 4. 実装ガイドライン

### 4.1. 現在の実装を改善する場合

既存の `fprintf(stderr, ...)` にプレフィックスを追加するだけで3段階分類が実現します。

```c
/* 現在 */
fprintf(stderr, "エラー: %s を開けません\n", OUTPUT_PATH);

/* 推奨 */
fprintf(stderr, "[ERROR] %s を開けません\n", OUTPUT_PATH);
```

```c
/* 現在 */
fprintf(stderr,
        "エラー: %s の snprintf がバッファ上限を超えました "
        "(buf=%zu, required=%d)\n",
        context, buf_size, ret);

/* 推奨 */
fprintf(stderr,
        "[WARN] %s の snprintf がバッファ上限を超えました "
        "(buf=%zu, required=%d)\n",
        context, buf_size, ret);
```

### 4.2. ログマクロを導入する場合（将来拡張）

`render.h` または専用の `log.h` に以下のようなマクロを追加することで、ログ出力を一元管理できます。

```c
/* log.h (将来追加案) */
#include <stdio.h>

#define LOG_INFO(fmt, ...)  fprintf(stdout, "[INFO] " fmt "\n", ##__VA_ARGS__)
#define LOG_WARN(fmt, ...)  fprintf(stderr, "[WARN] " fmt "\n", ##__VA_ARGS__)
#define LOG_ERROR(fmt, ...) fprintf(stderr, "[ERROR] " fmt "\n", ##__VA_ARGS__)
```

### 4.3. 禁止事項

- `printf` 系による構造化されていない出力を混在させない
- 正常終了コード（`EXIT_SUCCESS`）で `[ERROR]` ログを出すことのないようにする
- 秘密情報（URLパラメーター、ファイルパス以外の文字列）をログに含めない

---

## 5. GitHub Actions との連携

CI/CDパイプライン（`.github/workflows/deploy.yml`）では、以下のログが有用です。

| 場面 | 記録内容 | Actionsでの確認方法 |
|------|---------|-------------------|
| `make build` の成否 | コンパイルエラー・警告（`-Wall -Wextra -Werror`） | ステップ終了コードで自動判定 |
| `make generate` の成否 | `[INFO] 生成完了` または `[ERROR]` が `stderr` に出るか | ステップログを確認 |
| 生成ファイルの存在確認 | `dist/index.html` のサイズ・存在 | `ls -lh dist/` をステップに追加 |

Actionsは `stderr` に出力された内容を「エラー」として強調表示するため、`[WARN]` と `[ERROR]` を `stderr` に、`[INFO]` を `stdout` に出す本方針は CI との親和性が高いです。

---

## 6. 優先実装順

| 優先度 | 対応内容 |
|--------|---------|
| 高 | 既存の `fprintf(stderr, ...)` に `[ERROR]` / `[WARN]` プレフィックスを追加 |
| 中 | `html_escape` の NULL ポインタ検出を `[WARN]` ログ付きで実装 |
| 低 | セクション描画の開始・完了を `[INFO]` でログ出力 |
| 将来 | `log.h` マクロの導入・ログファイル出力の追加 |
