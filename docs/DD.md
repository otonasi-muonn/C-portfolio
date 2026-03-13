# 📄 詳細仕様書：C言語駆動型ポートフォリオサイト

本ドキュメントは、プロジェクト概要書に基づく具体的な実装方針、データ構造、およびCSSを用いた動的UIの実現手法を定義する。

## 1. システムアーキテクチャ

本システムは、C言語プログラムを静的サイトジェネレーター（SSG）として利用する。

* **データ層 (`data.c`, `models.h`)**: コンパイル時に組み込まれる静的な構造体データ。
* **ロジック層 (`render.c`, `main.c`)**: データを読み込み、HTML文字列を組み立てる処理。
* **ビュー層 (`main.css`)**: 出力されたHTMLに対し、装飾と状態管理（動的UI）を提供するCSS。
* **インフラ層 (GitHub Actions)**: 上記のコンパイルと実行、および成果物（`dist/`）のデプロイを全自動化する。

## 2. データモデル仕様 (C言語構造体設計)

動的メモリ割り当て（`malloc` / `free`）によるメモリリークやバグを防ぐため、データはすべて静的なポインタ（`const char*`）として保持する。

### 2.1. ベース構造体

* **`SiteConfig`**: サイト全体のメタ情報（タイトル、言語設定、description）。
* **`Profile`**: 氏名、所属、タグライン、バイオグラフィ。
* **`SocialLink`**: 外部SNS等へのリンク（サービス名、URL、表示ラベル）。

### 2.2. セクション特化型構造体

* **`SkillSection`**: 詳細な技術スタック表示は外部サービス（Gitty）に委譲するため、誘導用の説明文とURLのみを保持する。
* **`CareerEvent`**: キャリアのタイムライン用。`date`（年月）、`title`、`description` を保持。
* **`Project`**: 成果物用。`title`, `url`, `period`, `category` (フィルタリング用), `description` を保持。

## 3. HTMLジェネレーター・出力仕様

### 3.1. バッファ管理ルールとフェイルセーフの徹底

* 文字列の組み立てには必ず `snprintf` を使用し、バッファオーバーフローを防止する。
* 各セクションの描画関数内で確保する一時バッファは `#define BUF_SIZE 8192` 等の十分かつ固定のサイズとする。
* **フェイルセーフ**: `snprintf` の戻り値を検証し、確保したバッファサイズ（`BUF_SIZE`）を超過する書き込みが発生した場合は、処理を安全に切り上げるかエラーログを出力する。
* **【設計の背景と根拠】** エスケープ処理により、出力文字列のバイト数は元データの最大数倍に膨張する可能性がある。固定バッファを使用するC言語の設計において、戻り値検証を義務付けることで、想定外の入力によるサイレントなデータ欠損やメモリ破壊を論理的に防ぐ。



### 3.2. サニタイズ（エスケープ）処理

* HTMLに出力するすべての文字列データは、必ず事前に `html_escape()` 関数を経由させる。
* **変換要件**:
* `<` → `&lt;`
* `>` → `&gt;`
* `&` → `&amp;`
* `"` → `&quot;`
* `'` → `&#39;`



## 4. CSS動的UI 実装仕様（JS代替ロジック）

状態管理をHTMLとCSSのみで実装し、JavaScriptによるDOM操作を完全に代替する。

### 4.1. インタラクティブ・フィルタリング（Projects一覧）

ラジオボタンの選択状態を利用し、対象外のプロジェクトを非表示にする。

* **C言語側の出力ルール**:
プロジェクト一覧を囲む親要素（`.filter-container`）の直下に、カテゴリごとの隠しラジオボタン（例: `id="filter-web"`, `name="category"`）とラベルを出力する。各プロジェクトカードには `data-category="Web"` のような属性を付与する。
* **CSS側の制御ルール**:
`:has()` 擬似クラスを用いて親要素から状態を判定し、選択されたカテゴリと一致しないカードを非表示（`display: none;`）にする。
```css
/* 例: Webが選択されている時、data-categoryがWeb以外のカードを消す */
.filter-container:has(#filter-web:checked) .project-card:not([data-category="Web"]) {
    display: none;
}

```



### 4.2. 詳細情報のアコーディオン（Projectカード内）

チェックボックスを利用して、詳細テキストの表示・非表示を切り替える。

* **C言語側の出力ルール**:
プロジェクトのループ処理において、ループ変数 `i` を利用し、`id="detail-toggle-0"`, `id="detail-toggle-1"` のような「一意の連番ID」を `snprintf` で動的に生成する。これを隠しチェックボックスとトグルラベル（「詳細を見る」）に適用する。
* **CSS側の制御ルール**:
間接セレクタ（`~`）とCSS Gridを利用し、チェックボックスがONの時に後続の詳細コンテナを展開する。
* **【設計の背景と根拠】** 従来の `max-height` を用いたトランジションは、実際のコンテンツの高さに関わらず指定した最大値を基準にアニメーション時間が計算されるため、動作に遅延が生じる。CSS Gridの `grid-template-rows: 0fr` から `1fr` への推移を利用することで、ブラウザのレンダリングエンジンがコンテンツの自然な高さを正確に計算し、JavaScriptを用いずに完璧な開閉アニメーションが実現可能となる。


```css
.project-detail-wrapper {
    display: grid;
    grid-template-rows: 0fr;
    transition: grid-template-rows 0.3s ease;
}
.project-detail-inner {
    min-height: 0;
    overflow: hidden;
}
.detail-toggle:checked ~ .project-detail-wrapper {
    grid-template-rows: 1fr;
}

```



## 5. デプロイメント仕様

GitHub Actionsを利用したCI/CDパイプラインを構築する。静的ファイルを専用ブランチ（`gh-pages`など）にコミットする旧来の手法は採用せず、Actionsから直接アーティファクトとしてデプロイするモダンな手法を規定する。

* **設定ファイル**: `.github/workflows/deploy.yml`
* **トリガー**: `main` ブランチへの Push または Workflow dispatch（手動実行）。
* **実行環境**: `ubuntu-latest`
* **パイプライン・ステップ**:
1. ソースコードのチェックアウト (`actions/checkout`)。
2. `make all` の実行（`gcc` によるコンパイルと、バイナリ実行による `dist/index.html` 生成、CSSのコピー）。
3. 生成された `dist` ディレクトリを GitHub Pages 用のアーティファクトとしてアップロード (`actions/upload-pages-artifact` を使用)。
4. アップロードされたアーティファクトを用いて GitHub Pages へのデプロイを実行 (`actions/deploy-pages` を使用)。