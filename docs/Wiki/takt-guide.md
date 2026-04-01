# TAKT ガイド（C-portfolio）

このドキュメントは、`C-portfolio` で TAKT を使って開発タスクを回すための最小運用手順をまとめたものです。

---

## 目的

- タスクを `plan -> implement -> review` で機械的に進める
- 実装とレビューを分離し、品質を安定させる
- `make all` を中心に検証を標準化する

---

## インストール

```bash
npm install -g takt
```

初回起動で `~/.takt/config.yaml` を作成し、言語は `ja` を推奨します。

---

## このリポジトリでの基本フロー

1. タスク投入

```bash
takt
```

2. 実行

```bash
takt run
```

3. 結果確認

```bash
takt list
```

---

## C-portfolio 向け検証コマンド

TAKT の実行タスク内で、最終検証は次を基準にします。

```bash
make all
```

確認対象:

- `dist/index.html` が生成される
- `dist/main.css` がコピーされる
- コンパイル警告・エラーが発生しない

---

## 推奨ピース方針

- 小規模修正: `backend-mini` 相当の短サイクル
- 複数ファイル変更: `default` 相当の `plan -> implement -> review`
- 設計変更を含む作業: `review-fix-default` 相当でレビュー往復を有効化

※ このリポジトリには専用ピースを同梱せず、既存の標準ピース利用を前提にします。

---

## 運用上の注意

- 生成物 `dist/` はコミット対象にしない（`.gitignore` 準拠）
- ドキュメント変更時は `docs/` だけを更新し、不要なツール設定を混入させない
- ブランチ命名は `feature/<description>` / `bugfix/<description>` を使用する

