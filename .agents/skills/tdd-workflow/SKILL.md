---
name: tdd-workflow
description: C-portfolio向けTDD運用。実装前に検証観点を定義し、変更後は make all で必ず確認する。
origin: ECC (adapted for C-portfolio)
---

# TDD Workflow (C-portfolio)

## 使う場面

- 新機能追加
- バグ修正
- `render.c` / `data.c` / `main.c` の変更

## 基本フロー

1. 仕様確認: `docs/DD.md` と `docs/_complete.md` を読む
2. 期待動作を箇条書きで明確化する（表示、構造、エスケープ）
3. 実装する
4. `make all` を実行して生成物を確認する

## C-portfolio用チェック項目

- `snprintf` のみを使用し、バッファサイズを厳守している
- 出力文字列は必要に応じて `html_escape` を経由している
- `dist/index.html` と `dist/main.css` が生成される

