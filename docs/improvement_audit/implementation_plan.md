# 実装計画書 (Implementation Plan)

## 1. データモデルの拡張 (`src/models.h`)
以下の項目を構造体に追加し、表現力を高めます。

*   **`SiteConfig`**: `favicon_url`, `og_image_url`, `author` を追加。
*   **`Profile`**: `avatar_url` を追加。
*   **`Project`**: `image_url` を追加し、`category` を「技術スタック（文字列配列）」に拡張するか、別途タグとして保持。

## 2. レンダリングロジックの強化 (`src/render.c`)

### 2.1. SEO/OGP 対応
`render_head` 関数を更新し、以下のタグを出力するようにします。
*   `<link rel="icon" ...>`
*   `<meta property="og:title" ...>`
*   `<meta property="og:image" ...>`
*   `<meta name="twitter:card" ...>`

### 2.2. アクセシビリティ (ARIA) の追加
動的 UI 部分に HTML 属性を追加します。
*   プロジェクトのアコーディオン開閉状態を示す `aria-expanded`（CSS のチェックボックス状態と同期させるための工夫）。
*   フィルターボタンに `aria-controls` を付与。

### 2.3. 技術スタックのループ出力
プロジェクトごとに、使用技術（C, Makefile, CSS 等）をバッジとして複数出力するループ処理を実装します。

## 3. CSS の「こだわり」機能実装 (`styles/main.css`)

### 3.1. テーマスイッチ (Dark/Light Mode)
JavaScript を使わず、隠しチェックボックスと `:has()` 擬似クラスを用いて、サイト全体のカラーパレット（CSS 変数）を切り替える機能を実装します。

### 3.2. プロフィール・プロジェクト画像の装飾
アバター画像やスクリーンショットを美しく表示するためのレイアウト（`aspect-ratio` 等を活用）を追加します。

## 4. ビルドプロセスの改善 (`Makefile`)

*   **アセット同期**: `images/` ディレクトリの内容を `dist/images/` に `cp -r` するターゲットを追加。
*   **404 ページ生成**: `main.c` または別のエントリポイントから `dist/404.html` を出力する処理を追加。
