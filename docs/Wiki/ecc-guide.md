# ECC ガイド（C-portfolio）

このドキュメントは、Everything Claude Code（ECC）由来の運用思想を `C-portfolio` に適用するための手順です。

---

## 位置づけ

ECC は「スキル駆動で作業品質を均一化する」ための仕組みです。  
このリポジトリでは、ECC 本体の大量ファイルを持ち込まず、必要な運用ルールだけを採用します。

---

## Skills 管理方針（最小構成）

このリポジトリでは以下の2段階管理を推奨します。

- `.agents/skills/` : 有効化中のスキル
- `.agents/skills_pool/` : 必要時に有効化する候補スキル

推奨スキル（C-portfolio向け）:

- `tdd-workflow`
- `security-review`
- `verification-loop`
- `strategic-compact`

---

## スキル運用ルール

1. 変更前に設計資料（`docs/DD.md`, `docs/_complete.md`）を確認する  
2. 実装時は `snprintf` と `html_escape` の制約を守る  
3. 最終確認は `make all` を必ず実行する  
4. レビュー観点は「安全性・仕様整合・差分最小」を優先する

---

## 導入時の注意

- ECC の全量コピー（大量 skill / agent / rule）は行わない
- このリポジトリに不要な言語向けルールは追加しない
- 運用ルールは `docs/Wiki/` に文書化し、再現性を持たせる

