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

**真のCCSS v2**はこれを完全に逆転させる：
- **C言語 → WebAssembly → ブラウザで実行** ＝ 本物のフロントエンド
- **CSS → 状態・DB・ルーター・通信を担当** ＝ 本物の（？）バックエンド

### ゴール

C-portfolioに真のCCSS実装を**追加**し、現行CCSSと切り替えられるようにする。  
実装後に「真のCCSSを実装してみた」記事をQiitaに投稿する。

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
│  │  DOM操作        │◄───│  - チェックボックス    │    │
│  │  ロジック実行    │    │    ルーター           │    │
│  │  CSSバックから   │    │  - 認証              │    │
│  │  データ読み取り  │    │  - 狼煙通信          │    │
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

CSSがバックエンドとして担う機能は4つ。

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

**読み取り方法:**  
- CSS側: `content: var(--db-user-name)` で表示用に参照
- C (WASM)側: `getComputedStyle(document.documentElement).getPropertyValue('--db-user-name')` でJSブリッジ経由取得

**制約:**  
- CSS変数はランタイムでの「書き込み」はCSS単体ではできない
- 書き込みが必要な場合はC (WASM) が `element.style.setProperty()` で更新する
- これはCSSバックエンドへのC（フロント）からのリクエストと解釈する

---

### 3-2. チェックボックスルーター

ページ遷移・ルーティングをhidden radioボタン ＋ 隣接結合子で実現。

```html
<!-- index.html: ルーター状態 -->
<input type="radio" name="route" id="route-home"    checked hidden />
<input type="radio" name="route" id="route-works"         hidden />
<input type="radio" name="route" id="route-about"         hidden />

<!-- ナビゲーション（CSSで制御） -->
<label for="route-home">Home</label>
<label for="route-works">Works</label>
<label for="route-about">About</label>

<!-- ページコンテンツ -->
<div class="page" id="page-home">...</div>
<div class="page" id="page-works">...</div>
<div class="page" id="page-about">...</div>
```

```css
/* backend.css: ルーター */

/* 全ページ非表示がデフォルト */
.page { display: none; }

/* radioの状態に応じてページ表示 */
#route-home:checked   ~ * #page-home  { display: block; }
#route-works:checked  ~ * #page-works { display: block; }
#route-about:checked  ~ * #page-about { display: block; }

/* アクティブなナビリンクのスタイル */
#route-home:checked   ~ * label[for="route-home"]  { font-weight: bold; }
#route-works:checked  ~ * label[for="route-works"] { font-weight: bold; }
#route-about:checked  ~ * label[for="route-about"] { font-weight: bold; }
```

**URL連携:**  
C (WASM) がURLのhashを監視し、対応するradioを `checked` にする。  
これにより `/#works` のようなURLでも直接遷移できる。

---

### 3-3. 認証（CSSチェックボックス認証）

特定のチェックボックス操作シーケンスを「パスワード」として扱う。

```html
<!-- 認証チェックボックス群（hidden） -->
<input type="checkbox" id="auth-1" hidden />
<input type="checkbox" id="auth-2" hidden />
<input type="checkbox" id="auth-3" hidden />

<!-- 管理者専用コンテンツ（デフォルト非表示） -->
<div class="admin-content">...</div>
```

```css
/* backend.css: 認証ロジック */

/* auth-1 AND auth-3 がchecked（auth-2はunchecked）で認証成功 */
#auth-1:checked ~ #auth-2:not(:checked) ~ #auth-3:checked ~ .admin-content {
  display: block;
}

/* 認証失敗メッセージ */
.auth-failed { display: block; }
#auth-1:checked ~ #auth-2:not(:checked) ~ #auth-3:checked ~ .auth-failed {
  display: none;
}
```

**セキュリティについての正直な評価:**  
CSSはクライアントサイドで丸見えなので認証として機能しない。  
これは「CSSバックエンドの認証」というコンセプトの実証であり、実用目的ではない。  
記事でもこの点を正直に書く。

---

### 3-4. 狼煙通信（background-image: url()）

CSSのみでサーバーにGETリクエストを送信する。

```css
/* backend.css: 狼煙通信 */

/* ページ表示時に狼煙を放つ */
#route-home:checked ~ * #page-home {
  display: block;
  background-image: url("https://proxmox.example.com/log?event=PAGE_HOME&ts=0");
}

#route-works:checked ~ * #page-works {
  display: block;
  background-image: url("https://proxmox.example.com/log?event=PAGE_WORKS&ts=0");
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
- **ブラウザキャッシュ**: 同じURLは2回目以降リクエストしない場合あり → `?ts=` パラメータで回避（C WASMからCSSに時刻を書き込む）
- **ブロッカー**: uBlockOriginなどの広告ブロッカーでブロックされる可能性あり

---

## 4. C (WASM) フロントエンド仕様

### 役割

- DOMの動的操作（CSSバックの変数DBからデータを読み取り描画）
- URLハッシュの監視 → チェックボックスルーターとの連携
- 狼煙通信のタイムスタンプ更新（キャッシュ回避）
- アニメーション・インタラクション処理

### ビルド

```makefile
# Makefile（WASMビルド部分）

WASM_SRC  = src/wasm/main.c
WASM_OUT  = dist/main.wasm
JS_GLUE   = dist/main.js

wasm:
	emcc $(WASM_SRC) \
	  -o $(JS_GLUE) \
	  -s WASM=1 \
	  -s EXPORTED_FUNCTIONS='["_init", "_on_hash_change"]' \
	  -s EXPORTED_RUNTIME_METHODS='["ccall", "cwrap"]' \
	  -s ALLOW_MEMORY_GROWTH=1 \
	  -O2
```

### ソース構成（src/wasm/main.c）

```c
#include <emscripten.h>
#include <string.h>

/* CSSバックエンドのDB変数を読み取り、DOMに反映する処理はJSグルー経由 */

EMSCRIPTEN_KEEPALIVE
void init(void) {
    /* 起動時処理: URLハッシュを読み取りルーターに反映 */
    EM_ASM({
        const hash = window.location.hash.slice(1) || 'home';
        const radio = document.getElementById('route-' + hash);
        if (radio) radio.checked = true;

        /* 狼煙通信のタイムスタンプを更新してキャッシュを無効化 */
        const ts = Date.now();
        document.documentElement.style.setProperty('--noro-ts', ts);
    });
}

EMSCRIPTEN_KEEPALIVE
void on_hash_change(void) {
    /* ハッシュ変更時にルーターを同期 */
    EM_ASM({
        const hash = window.location.hash.slice(1) || 'home';
        const radio = document.getElementById('route-' + hash);
        if (radio) radio.checked = true;
    });
}
```

### HTMLへの組み込み

```html
<!-- index.html -->
<script src="main.js"></script>
<script>
  Module.onRuntimeInitialized = () => {
    Module.ccall('init', null, [], []);
    window.addEventListener('hashchange', () => {
      Module.ccall('on_hash_change', null, [], []);
    });
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

<!-- 真のCCSS（WASM + CSSバックエンド） -->
<div id="true-ccss">
  <!-- WASMが描画するコンテンツ -->
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
│   └── backend.css     新規: CSSバックエンド（DB・ルーター・認証・狼煙）
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
| 1 | CSSバックエンド基盤（変数DB・ルーター） | backend.css 基本形 |
| 2 | チェックボックス認証 | backend.css 認証追加 |
| 3 | C WASM フロントエンド | src/wasm/main.c + Makefile更新 |
| 4 | 狼煙通信 + PROXMOXサーバー構築 | proxmox/log_receiver.c + Apache設定 |
| 5 | 切り替え機能 + 統合 | index.html 更新 |
| 6 | 記事執筆 | Qiita: 「真のCCSSを実装してみた」 |

---

## 10. 技術的注意点まとめ

| 項目 | 内容 |
|------|------|
| HTTPS | PROXMOXをHTTPS化しないとMixed Contentでブロックされる |
| キャッシュ | background-image URLは同一URLをキャッシュする。TSパラメータで回避 |
| 広告ブロッカー | 狼煙通信がuBlockなどでブロックされる可能性あり（記事に正直に書く） |
| WASM CORS | GitHub Pagesから.wasmを読む場合はCORSヘッダーが必要（GitHub Pagesは自動付与） |
| CSS変数の書き込み | CSSからCSSへの書き込み不可。C(WASM)→setProperty経由で行う |
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
7. 「結局Cが全部やっている」という哲学的結論

---

*本企画書はCCSSアーキテクチャ研究会（情報技術研究部）により策定された。*
*JavaScriptは一切使用しない予定だったが、Emscriptenのグルーコードが生成するJSについては見なかったことにする。*
