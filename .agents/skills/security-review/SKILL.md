---
name: security-review
description: C-portfolio向けセキュリティ確認。HTML出力の安全性と危険API混入を重点確認する。
origin: ECC (adapted for C-portfolio)
---

# Security Review (C-portfolio)

## 使う場面

- HTML出力ロジックの変更時
- CGI/外部通信まわりを扱う変更時
- ユーザー入力相当データの扱いを変える時

## 重点チェック

- `sprintf` ではなく `snprintf` を使っている
- `system` / `popen` など危険な実行経路を追加していない
- HTML出力に `html_escape` が適用されている
- サイレント失敗ではなく、エラーを明示的に扱っている

## 最終確認

- `make all` が成功する
- 生成HTMLが壊れていない

