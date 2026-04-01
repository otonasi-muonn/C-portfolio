# 真のCCSS v2 企画書
## C-language Client-side & Cascading Style Sheets Server-side

**作成者:** 無音（情報技術研究部 2年）  
**作成日:** 2026-04-01  
**ステータス:** 企画フェーズ

---

## 1. 概要・背景

### 現行CCSSとの違い

現行CCSS（C-portfolio / megalo2026で実装済み）は：
- C言語でHTMLを**静的生成**（実態はバックエンド処理）
- CSSで**UI状態管理**（実態はフロントエンド）

コメント欄でも指摘された通り、技術的には「C=バック、CSS=フロント」が正しい。

**真のCCSS v2**はこれを「逆転」ではなく**責務分離として固定**する：
- **C言語（WASM）**はフロントエンド実行系として、描画・イベント処理・遷移トリガーを担当
- **CSS**はバックエンド実行系として、状態管理・データ提供・ルーティング判定・通信発火を担当

### 責務境界（この企画で守るルール）

1. フロントロジックはCに集約する（UI構築、入力処理、表示更新）。
2. バックロジックはCSSに集約する（状態保持、表示可否判定、外部通知）。
3. Cから直接`fetch`/`XHR`は呼ばない。外部送信はCSSの`url()`経由を原則とする。
4. Cは「CSSバックエンドAPI（Custom Properties / hidden controls）」を通じてのみ状態にアクセスする。

### ゴール

C-portfolioに真のCCSS実装を**追加**し、現行CCSSと切り替えられるようにする。  
さらに「Cフロント / CSSバック」の責務分離を説明可能な設計として、Qiita記事に落とし込む。

---

## 2. アーキテクチャ全体図

```
┌─────────────────────────────────────────────────────┐
│                    ブラウザ                           │
│                                                     │
│  ┌─────────────────┐    ┌──────────────────────┐    │
│  │   C (WASM)      │    │   CSS バックエンド    │    │
│  │   フロントエンド  │    │                      │    │
│  │                 │    │  - CSS変数DB          │    │
│  │  UI描画/再描画   │◄───│  - 変数DB（状態）     │    │
│  │  入力イベント処理 │    │  - ルーティング判定   │    │
│  │  画面遷移トリガー │    │  - 表示ゲート判定     │    │
│  │  CSS API呼び出し │    │  - 狼煙通信発火       │    │
│  └─────────────────┘    └──────────┬───────────┘    │
│                                    │ background-image: url()
└────────────────────────────────────┼────────────────┘
                                     │ GET /log?event=XXX
                              ┌──────▼──────────┐
                              │  PROXMOXサーバー │
                              │  C言語 CGI       │
                              │  （狼煙受信ログ）  │
                              └─────────────────┘

ホスティング:
  静的ファイル（HTML/CSS/WASM） → GitHub Pages
  ビルドパイプライン            → GitHub Actions
  狼煙受信エンドポイント         → サークルPROXMOXサーバー
```

---

## 3. CSS バックエンド仕様（詳細）

CSSがバックエンドとして担う機能は5つ（DB / ルーター / 表示ゲート / 狼煙通信 / フロント向けAPI公開）。

### 3-1. CSS変数DB

CSS Custom Propertiesを静的データストアとして使用する。

```css
/* style/backend.css */

/* ユーザーDB */
:root {
  --db-user-name: "無音";
  --db-user-role: "admin";
  --db-user-since: "2024";
}

/* プロジェクトDB（件数分定義） */
:root {
  --db-project-1-title: "C-portfolio";
  --db-project-1-category: "web";
  --db-project-1-lang: "C";

  --db-project-2-title: "megalo2026";
  --db-project-2-category: "game";
  --db-project-2-lang: "TypeScript";
}
```

**読み取り方法（CSSバックエンドAPI）:**  
- CSS側: `content: var(--db-user-name)` で表示用に参照
- C (WASM)側: `getComputedStyle(document.documentElement).getPropertyValue('--db-user-name')` で取得
- CはこのAPI経由でのみデータアクセスし、データの正本はCSSに置く

**制約:**  
- CSS変数はランタイムでの「書き込み」はCSS単体ではできない
- 書き込みが必要な場合はC (WASM) が `element.style.setProperty()` で更新する
- これはCSSバックエンドへのC（フロント）からのリクエストと解釈する

---

### 3-2. CSSルーター（`:target` + `:has`）

ページ遷移・ルーティング判定はCSS単体で行う。  
Cフロントはリンク操作をトリガーするだけで、判定ロジックはCSS側に置く。

```html
<nav class="router-nav">
  <a href="#page-home">Home</a>
  <a href="#page-works">Works</a>
  <a href="#page-about">About</a>
</nav>

<main class="router-view">
  <section class="page" id="page-home">...</section>
  <section class="page" id="page-works">...</section>
  <section class="page" id="page-about">...</section>
</main>
```

```css
/* backend.css: ルーター */

/* デフォルトはHome表示 */
.page { display: none; }
#page-home { display: block; }

/* targetがある時はtargetのみ表示 */
.router-view:has(.page:target) #page-home { display: none; }
.page:target { display: block; }

/* アクティブナビ表示 */
body:has(#page-home:target) .router-nav a[href="#page-home"],
body:has(#page-works:target) .router-nav a[href="#page-works"],
body:has(#page-about:target) .router-nav a[href="#page-about"] {
  font-weight: bold;
}
```

**責務:**  
- Cフロント: `window.location.hash` の更新トリガー
- CSSバック: どのページを表示するかの判定・適用

---

### 3-3. 表示ゲート（CSSチェックボックス認可）

特定のチェックボックス操作シーケンスを「ゲート条件」として扱う。

```html
<!-- 表示ゲート用チェックボックス群（hidden） -->
<input type="checkbox" id="auth-1" hidden />
<input type="checkbox" id="auth-2" hidden />
<input type="checkbox" id="auth-3" hidden />

<!-- ゲート通過時のみ表示するコンテンツ（デフォルト非表示） -->
<div class="admin-content">...</div>
```

```css
/* backend.css: 表示ゲートロジック */

/* auth-1 AND auth-3 がchecked（auth-2はunchecked）でゲート通過 */
#auth-1:checked ~ #auth-2:not(:checked) ~ #auth-3:checked ~ .admin-content {
  display: block;
}

/* ゲート未通過メッセージ */
.auth-failed { display: block; }
#auth-1:checked ~ #auth-2:not(:checked) ~ #auth-3:checked ~ .auth-failed {
  display: none;
}
```

**セキュリティについての正直な評価:**  
CSSはクライアントサイドで丸見えなので「認証」にはならない。  
本仕様はあくまで「表示ゲート（UI権限制御の演出）」の実証であり、実運用の認証機構とは分離する。  
記事でもこの点を正直に書く。

---

### 3-4. 狼煙通信（background-image: url()）

CSSのみでサーバーにGETリクエストを送信する。

```css
/* backend.css: 狼煙通信 */

/* ページ表示時に狼煙を放つ */
#page-home:target {
  display: block;
  background-image: url("https://proxmox.example.com/log?event=PAGE_HOME_V1");
}

#page-works:target {
  display: block;
  background-image: url("https://proxmox.example.com/log?event=PAGE_WORKS_V1");
}

/* ボタンhover時に狼煙を放つ */
.project-card:hover {
  background-image: url("https://proxmox.example.com/log?event=CARD_HOVER");
}
```

**仕組み:**  
1. CSSが対象要素をレンダリングする際に `background-image: url()` を解決
2. ブラウザがGETリクエストを発火
3. PROXMOXのC CGIがリクエストを受信・ログ保存
4. レスポンスは1x1の透明PNGを返す（必須）

**制約・注意事項:**  
- **HTTPS必須**: GitHub PagesはHTTPS。Mixed Contentエラーを防ぐためPROXMOXもHTTPS化が必要
- **CORS**: CSSからのリクエストはCORSの対象外（画像リクエスト扱い）なのでサーバー側のCORS設定は不要
- **一方向のみ**: レスポンス内容をCSSが読み取ることはできない。あくまで「放つ」だけ
- **ブラウザキャッシュ**: 同じURLは2回目以降リクエストしない場合あり → イベントごとにバージョン付きURLを定義して回避
- **ブロッカー**: uBlockOriginなどの広告ブロッカーでブロックされる可能性あり

---

## 4. C (WASM) フロントエンド仕様

### 役割（Cフロントを主役にする）

- UIコンポーネントをCで生成・再描画する（カード、一覧、詳細パネル）
- キーボード/ポインタ入力をCで処理し、遷移トリガーを発行する
- CSSバックエンドAPIからデータを読み取り、表示モデルを構築する
- 画面状態（選択中カード、フォーカス、展開状態）をCの状態機械で管理する

### CSSバックエンドとの接続ルール

Cは以下の2種類だけを使ってCSSバックエンドと通信する。

1. **Read API**: Custom Propertiesの読み取り  
   `getComputedStyle(...).getPropertyValue("--db-*")`
2. **Request API**: hidden control / hash の更新  
   `window.location.hash = ...` や hidden input の `checked` 変更

これにより「判定はCSS、実行はC」の境界を保つ。

### ビルド

```makefile
# Makefile（WASMビルド部分）

WASM_SRC = src/wasm/main.c
JS_GLUE  = dist/main.js

wasm:
	emcc $(WASM_SRC) \
	  -o $(JS_GLUE) \
	  -s WASM=1 \
	  -s EXPORTED_FUNCTIONS='["_frontend_boot","_frontend_open_route"]' \
	  -s EXPORTED_RUNTIME_METHODS='["ccall","cwrap"]' \
	  -s ALLOW_MEMORY_GROWTH=1 \
	  -O2
```

### ソース構成（src/wasm/main.c）

```c
#include <emscripten.h>
#include <stdio.h>

#define KEY_BUF_SIZE 64
#define VAL_BUF_SIZE 128
#define MAX_PROJECTS 16

EM_JS(int, css_get_var, (const char *key, char *out, int out_size), {
  const name = UTF8ToString(key);
  const raw = getComputedStyle(document.documentElement)
    .getPropertyValue(name)
    .trim()
    .replace(/^"(.*)"$/, "$1");
  if (!raw) return 0;
  stringToUTF8(raw, out, out_size);
  return 1;
});

EM_JS(void, ui_append_project, (const char *title, const char *category, const char *lang), {
  const list = document.getElementById("project-list");
  if (!list) return;
  const card = document.createElement("article");
  card.className = "project-card";
  card.setAttribute("data-category", UTF8ToString(category));
  card.innerHTML = "<h3>" + UTF8ToString(title) + "</h3><p>" + UTF8ToString(lang) + "</p>";
  list.appendChild(card);
});

EMSCRIPTEN_KEEPALIVE
void frontend_boot(void) {
  char key[KEY_BUF_SIZE];
  char title[VAL_BUF_SIZE];
  char category[VAL_BUF_SIZE];
  char lang[VAL_BUF_SIZE];

  for (int i = 1; i <= MAX_PROJECTS; ++i) {
    snprintf(key, sizeof(key), "--db-project-%d-title", i);
    if (!css_get_var(key, title, sizeof(title))) {
      break;
    }

    snprintf(key, sizeof(key), "--db-project-%d-category", i);
    if (!css_get_var(key, category, sizeof(category))) {
      snprintf(category, sizeof(category), "unknown");
    }

    snprintf(key, sizeof(key), "--db-project-%d-lang", i);
    if (!css_get_var(key, lang, sizeof(lang))) {
      snprintf(lang, sizeof(lang), "n/a");
    }

    ui_append_project(title, category, lang);
  }
}

EMSCRIPTEN_KEEPALIVE
void frontend_open_route(const char *route) {
  EM_ASM({
    const routeName = UTF8ToString($0);
    window.location.hash = "page-" + routeName;
  }, route);
}
```

### HTMLへの組み込み

```html
<!-- index.html -->
<script src="main.js"></script>
<script>
  Module.onRuntimeInitialized = () => {
    Module.ccall("frontend_boot", null, [], []);
  };
</script>
```

---

## 5. PROXMOXサーバー（C言語 CGI）仕様

### 役割

狼煙（`background-image: url()`）を受信してログに記録する。  
レスポンスとして1x1透明PNGを返す。

### 環境設定

```bash
# Ubuntu/Debian VM on PROXMOX
apt install apache2
a2enmod cgi
# /etc/apache2/sites-enabled/000-default.conf に追記:
# ScriptAlias /log /var/www/cgi-bin/log.cgi
```

### ソース（proxmox/log_receiver.c）

```c
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

/* 1x1 透明PNG（44バイト）*/
static const unsigned char TRANSPARENT_PNG[] = {
    0x89,0x50,0x4e,0x47,0x0d,0x0a,0x1a,0x0a,
    0x00,0x00,0x00,0x0d,0x49,0x48,0x44,0x52,
    0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x01,
    0x08,0x06,0x00,0x00,0x00,0x1f,0x15,0xc4,
    0x89,0x00,0x00,0x00,0x0b,0x49,0x44,0x41,
    0x54,0x78,0x9c,0x62,0x00,0x00,0x00,0x02,
    0x00,0x01,0xe2,0x21,0xbc,0x33,0x00,0x00,
    0x00,0x00,0x49,0x45,0x4e,0x44,0xae,0x42,
    0x60,0x82
};

int main(void) {
    /* クエリ文字列を取得 */
    const char *query = getenv("QUERY_STRING");
    const char *remote = getenv("REMOTE_ADDR");

    /* タイムスタンプ */
    time_t now = time(NULL);
    struct tm *t = gmtime(&now);
    char timebuf[32];
    strftime(timebuf, sizeof(timebuf), "%Y-%m-%dT%H:%M:%SZ", t);

    /* ログファイルに書き込み */
    FILE *log = fopen("/var/log/ccss/noro.log", "a");
    if (log) {
        fprintf(log, "[%s] ip=%s query=%s\n",
                timebuf,
                remote  ? remote : "unknown",
                query   ? query  : "");
        fclose(log);
    }

    /* レスポンス: 1x1透明PNG */
    fputs("Content-Type: image/png\r\n", stdout);
    fputs("Cache-Control: no-cache, no-store\r\n", stdout);
    fputs("Access-Control-Allow-Origin: *\r\n", stdout);
    fputs("\r\n", stdout);
    fwrite(TRANSPARENT_PNG, 1, sizeof(TRANSPARENT_PNG), stdout);

    return 0;
}
```

```makefile
# proxmox/Makefile
log_receiver.cgi: log_receiver.c
	gcc -Wall -Wextra -O2 -o log_receiver.cgi log_receiver.c
	mkdir -p /var/log/ccss
	chmod 755 log_receiver.cgi
	cp log_receiver.cgi /var/www/cgi-bin/log.cgi
```

### HTTPS化

Let's Encrypt（Certbot）でDDNS or 固定IPに証明書を発行する。  
GitHub PagesはHTTPSのため、Mixed Content回避に必須。

---

## 6. 切り替え機能（現行CCSS ⇔ 真のCCSS）

当然チェックボックスで切り替える。

```html
<!-- index.html -->
<input type="checkbox" id="ccss-mode" hidden />
<label for="ccss-mode" class="mode-toggle">真のCCSSモード</label>

<!-- 現行CCSS（静的生成HTML） -->
<div id="legacy-ccss">
  <!-- C言語で静的生成した既存コンテンツ -->
</div>

<!-- 真のCCSS（Cフロント + CSSバックエンド） -->
<div id="true-ccss">
  <!-- C (WASM) が描画し、CSSバックが判定するコンテンツ -->
</div>
```

```css
/* 現行CCSSをデフォルト表示 */
#legacy-ccss { display: block; }
#true-ccss   { display: none; }

/* チェックで切り替え */
#ccss-mode:checked ~ * #legacy-ccss { display: none; }
#ccss-mode:checked ~ * #true-ccss   { display: block; }
```

---

## 7. ファイル構成

```
C-portfolio/
├── src/
│   ├── main.c          既存: SSGジェネレーター
│   ├── render.c        既存
│   ├── data.c          既存
│   ├── models.h        既存
│   ├── render.h        既存
│   └── wasm/
│       └── main.c      新規: WASMフロントエンド
├── styles/
│   ├── main.css        既存
│   └── backend.css     新規: CSSバックエンド（DB・ルーター・表示ゲート・狼煙）
├── proxmox/
│   ├── log_receiver.c  新規: C CGI（狼煙受信）
│   └── Makefile        新規
├── dist/               ビルド成果物（gitignore）
│   ├── index.html      静的生成HTML
│   ├── main.css
│   ├── main.wasm       WASMバイナリ
│   └── main.js         Emscriptenグルーコード
├── docs/
│   ├── DD.md           既存: 現行CCSS設計書
│   └── 20260401_ccss_v2_proposal.md  本ドキュメント
├── Makefile            更新: wasmターゲット追加
└── .github/
    └── workflows/
        └── build.yml   更新: Emscriptenビルド追加
```

---

## 8. GitHub Actions ビルドパイプライン

```yaml
# .github/workflows/build.yml
name: Build CCSS

on:
  push:
    branches: [main]

jobs:
  build:
    runs-on: ubuntu-latest
    steps:
      - uses: actions/checkout@v4

      # 現行CCSS: C静的生成
      - name: Build SSG (legacy CCSS)
        run: |
          make build
          make generate

      # 真のCCSS: C → WASM
      - name: Install Emscripten
        uses: mymindstorm/setup-emsdk@v14
        with:
          version: '3.1.50'

      - name: Build WASM (true CCSS)
        run: make wasm

      # GitHub Pages へデプロイ
      - name: Deploy to GitHub Pages
        uses: peaceiris/actions-gh-pages@v3
        with:
          github_token: ${{ secrets.GITHUB_TOKEN }}
          publish_dir: ./dist
```

---

## 9. 実装フェーズ

| フェーズ | 内容 | 成果物 |
|---------|------|--------|
| 1 | CSSバックエンド基盤（変数DB・ルーター・API化） | backend.css 基本形 |
| 2 | 表示ゲート（認可演出） | backend.css ゲート追加 |
| 3 | C WASM フロントエンド主実装 | src/wasm/main.c + Makefile更新 |
| 4 | 狼煙通信 + PROXMOXサーバー構築 | proxmox/log_receiver.c + Apache設定 |
| 5 | 切り替え機能 + 統合 | index.html 更新 |
| 6 | 記事執筆 | Qiita: 「真のCCSSを実装してみた」 |

---

## 10. 技術的注意点まとめ

| 項目 | 内容 |
|------|------|
| HTTPS | PROXMOXをHTTPS化しないとMixed Contentでブロックされる |
| キャッシュ | background-image URLは同一URLをキャッシュする。イベントIDにバージョンを持たせて回避 |
| 広告ブロッカー | 狼煙通信がuBlockなどでブロックされる可能性あり（記事に正直に書く） |
| WASM CORS | GitHub Pagesから.wasmを読む場合はCORSヘッダーが必要（GitHub Pagesは自動付与） |
| CSS API境界 | Cは`--db-*`読取とhash/hidden control操作のみ。判定ロジックはCSS側に固定する |
| CGI速度 | リクエストごとにプロセス生成。高負荷非想定（ポートフォリオなので問題なし） |
| セキュリティ | CGIはQUERY_STRINGをログに書くだけ。コマンドインジェクション対策のためsystemやpopenは使わない |

---

## 11. 記事構成メモ（Qiita）

1. 前回記事（現行CCSS）のおさらい
2. コメント欄の正論「SSRはバックだしCSSはフロントだ」
3. 「じゃあ本当に逆にする」宣言
4. CSS変数DB・チェックボックスルーター・狼煙通信の解説
5. PROXMOXのサークルサーバーに C CGI を建てた話
6. 実装してわかったこと（広告ブロッカー問題など）
7. 「Cフロント / CSSバック」の境界をどこまで守れたかの検証

---

*本企画書はCCSSアーキテクチャ研究会（情報技術研究部）により策定された。*
*ランタイムで使うJSはEmscriptenグルーに限定し、アプリケーションロジックはC側に保持する。*
