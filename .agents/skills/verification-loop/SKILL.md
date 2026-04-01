---
name: verification-loop
description: C-portfolio向け検証ループ。変更後に必ず make all と生成物確認を行う。
origin: ECC (adapted for C-portfolio)
---

# Verification Loop (C-portfolio)

## 実行タイミング

- 機能実装後
- PR作成前
- ドキュメントと実装を同時変更した後

## 検証手順

1. ビルド・生成

```bash
make all
```

2. 生成物確認

- `dist/index.html` が存在する
- `dist/main.css` が存在する

3. 差分確認

- 意図しないファイルを変更していない
- 無関係な設定ファイルを混入していない

