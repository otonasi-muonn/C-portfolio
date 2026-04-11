# テスト設計（C-portfolio）

本ドキュメントは `C-portfolio` のテスト方針・対象・カバレッジ目標を定義します。

---

## 1. 現状の確認

### 現在のテスト状況

本プロジェクトには現時点で専用のテストコードは存在しません。
品質確認は以下の手動手順に依存しています。

| 確認方法 | 内容 |
|---------|------|
| `make all` の成功 | コンパイルが通り、`dist/index.html` が生成されること |
| ブラウザでの目視確認 | 全セクションが正しく表示されること |
| GitHub Actions の成功 | CI上でのビルドとデプロイが通ること |

`-Wall -Wextra -Werror -pedantic` による厳格なコンパイラ警告設定が、静的な品質保証として機能しています。

---

## 2. テスト方針

### 基本方針

1. **コンパイラ警告を第一の防衛線とする** — `-Wall -Wextra -Werror` が有効なため、多くのバグはコンパイル時に検出できる
2. **テストは実行可能な単体関数を対象にする** — `html_escape`・`checked_snprintf` など、入出力が明確な関数から始める
3. **結合テストは `make all` + 出力HTML検証で行う** — C言語製SSGの性質上、最終成果物（HTML）の正しさが最重要
4. **テストコードもコンパイラ警告の対象にする** — Makefile のフラグ（`-Wall -Wextra -Werror`）をテストビルドにも適用する

---

## 3. テスト対象と種別

### 3.1. 単体テスト対象（Unit Test）

#### `html_escape()` 関数 (`render.c`)

最も重要なセキュリティ関数です。入力と期待出力のペアでテストします。

| テストケース | 入力 | 期待出力 |
|------------|------|---------|
| アンパサンドのエスケープ | `"A&B"` | `"A&amp;B"` |
| 小なり記号のエスケープ | `"<tag>"` | `"&lt;tag&gt;"` |
| ダブルクォートのエスケープ | `"say \"hi\""` | `"say &quot;hi&quot;"` |
| シングルクォートのエスケープ | `"it's"` | `"it&#39;s"` |
| 特殊文字なしの通常文字列 | `"hello"` | `"hello"` |
| 空文字列 | `""` | `""` |
| NULL ポインタ渡し | `NULL` | `""` (サイレント復帰) |
| バッファ境界ちょうどの入力 | `dest_size=5` に 5バイト以上の入力 | 末尾が `\0` で切り詰められること |
| 5種すべての特殊文字混在 | `"<>&\"'"` | `"&lt;&gt;&amp;&quot;&#39;"` |

#### `checked_snprintf()` 関数 (`render.c`)

バッファ管理の安全弁として機能する内部ヘルパー関数です。

| テストケース | 条件 | 期待動作 |
|------------|------|---------|
| 正常な書き込み | バッファに十分な余裕がある | 戻り値 `1`、正しく文字列が書き込まれる |
| バッファ上限ちょうど | 書き込みバイト数 == `buf_size` | 切り詰め発生、`stderr` に `[WARN]` 出力、戻り値 `0` |
| バッファ上限超過 | 書き込みバイト数 > `buf_size` | 切り詰め発生、`stderr` に `[WARN]` 出力、戻り値 `0` |
| `buf = NULL` | NULL ポインタ渡し | `stderr` に `[ERROR]` 出力、戻り値 `0` |
| `buf_size = 0` | サイズが0 | `stderr` に `[ERROR]` 出力、戻り値 `0` |

---

### 3.2. 結合テスト対象（Integration Test）

`render_*` 系の関数は FILE* への出力を行うため、`fmemopen` または `tmpfile` を使ったバッファ比較でテストします。

#### `render_profile()` の出力検証

| 確認項目 | 検証方法 |
|---------|---------|
| `<section id="about">` が出力に含まれること | `strstr` で検索 |
| `profile.name` がエスケープ済みで含まれること | エスケープ後の文字列を `strstr` で検索 |
| `profile.bio` がエスケープ済みで含まれること | 同上 |
| ソーシャルリンクが `links_count` 個含まれること | `<li>` タグの出現回数をカウント |
| `<a href` の URL が XSS を含まないこと | `<script>` が出力に含まれないことを確認 |

#### `render_projects()` の出力検証

| 確認項目 | 検証方法 |
|---------|---------|
| `projects_count` 個の `<article>` タグが含まれること | `<article` の出現回数をカウント |
| カテゴリフィルター用 `<input type="radio">` が出力されること | ユニークカテゴリ数と一致する数を確認 |
| `detail-toggle-0` から `detail-toggle-N` のIDが連番で含まれること | 各IDを `strstr` で検索 |
| 特殊文字を含むプロジェクト名が正しくエスケープされること | `<`, `>`, `&` が `&lt;`, `&gt;`, `&amp;` になっていること |

#### `render_career()` の出力検証

| 確認項目 | 検証方法 |
|---------|---------|
| `career_events_count` 個の `.timeline-item` が含まれること | 出現回数をカウント |
| 各 `date` フィールドの値が含まれること | `strstr` で検索 |

---

### 3.3. エンドツーエンドテスト（E2E）

ビルドから出力HTMLまでの全体フローを検証します。

| 確認項目 | 実行方法 | 合否基準 |
|---------|---------|---------|
| `make all` が成功すること | `make all` の終了コードが `0` | `echo $?` が `0` |
| `dist/index.html` が生成されること | ファイルの存在確認 | `ls dist/index.html` が成功 |
| `dist/main.css` がコピーされること | ファイルの存在確認 | `ls dist/main.css` が成功 |
| HTMLが有効な構造を持つこと | `<html>`, `<head>`, `<body>` が含まれること | `grep` で各タグを確認 |
| `<script>` タグが含まれないこと | XSS混入がないことの確認 | `grep -c '<script'` が `0` |
| `dist/index.html` のサイズが0バイトでないこと | ファイルサイズ確認 | `wc -c dist/index.html` が `> 0` |

---

## 4. テストコードの配置

テストコードは `tests/` ディレクトリに配置します（現在は存在しないため新規作成が必要です）。

```text
tests/
├── test_html_escape.c      # html_escape() 単体テスト
├── test_snprintf.c         # checked_snprintf() 単体テスト
├── test_render_profile.c   # render_profile() 結合テスト
├── test_render_projects.c  # render_projects() 結合テスト
└── test_render_career.c    # render_career() 結合テスト
```

テストビルドは `make test` で実行できるよう `Makefile` に追加します。

```makefile
# Makefile への追加案
TEST_DIR = tests
TEST_SRCS = $(wildcard $(TEST_DIR)/*.c)
TEST_BINS = $(TEST_SRCS:.c=)

test: $(TEST_BINS)
	@for t in $(TEST_BINS); do ./$$t && echo "PASS: $$t" || echo "FAIL: $$t"; done

$(TEST_DIR)/%: $(TEST_DIR)/%.c $(SRC_DIR)/render.c $(SRC_DIR)/data.c
	$(CC) $(CFLAGS) -o $@ $^ -I$(SRC_DIR)
```

---

## 5. カバレッジ目標

| 対象 | 目標カバレッジ | 優先度 |
|------|--------------|--------|
| `html_escape()` | 100% | 高 — セキュリティ関数のため全ケースを網羅 |
| `checked_snprintf()` | 100% | 高 — バッファ管理の要 |
| `render_profile()` | 80% | 中 — XSSリスクがある出力関数 |
| `render_projects()` | 80% | 中 — ID生成ロジックがある複雑な関数 |
| `render_career()` | 70% | 中 |
| `render_head()` / `render_header()` | 60% | 低 — 単純な文字列出力 |
| `render_footer()` | 60% | 低 — `__DATE__`/`__TIME__` の固定出力 |
| `main()` | E2Eテストで代替 | — |

---

## 6. CI での自動テスト

GitHub Actions（`.github/workflows/deploy.yml`）に以下のステップを追加することを推奨します。

```yaml
# .github/workflows/deploy.yml への追加案
- name: Run unit and integration tests
  run: make test

- name: Verify generated HTML
  run: |
    test -f dist/index.html || (echo "ERROR: dist/index.html not found" && exit 1)
    test -s dist/index.html || (echo "ERROR: dist/index.html is empty" && exit 1)
    grep -q '<html' dist/index.html || (echo "ERROR: no <html> tag" && exit 1)
    ! grep -q '<script' dist/index.html || (echo "ERROR: <script> tag found" && exit 1)
```

---

## 7. 実装優先順位

| 優先度 | 対応内容 |
|--------|---------|
| 高 | `html_escape()` の単体テスト (`tests/test_html_escape.c`) を作成 |
| 高 | E2Eテストをシェルスクリプトまたは `Makefile` のターゲットとして追加 |
| 中 | `checked_snprintf()` の単体テスト (`tests/test_snprintf.c`) を作成 |
| 中 | `render_projects()` の結合テスト（ID連番とXSSチェック）を作成 |
| 低 | CI に `make test` ステップを追加 |
| 将来 | カバレッジ計測ツール（`gcov`）の導入 |
