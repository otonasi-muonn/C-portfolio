# TAKT ガイド（C-portfolio）

このドキュメントは、`C-portfolio` で TAKT を使って開発タスクを回すための実運用手順です。  
「導入しすぎない」ことを前提に、必要最小の運用だけを定義します。

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

推奨の最小設定例:

```yaml
provider: claude
model: sonnet
language: ja
concurrency: 1
```

---

## C-portfolioでの運用境界

このリポジトリでは、TAKTの全機能を使い切ることは目的にしません。  
以下を「使う」、それ以外は必要時のみ使う方針です。

**常用**

- `takt`（タスク投入）
- `takt run`（実行）
- `takt list`（結果確認）

**必要時のみ**

- `takt watch`（常駐実行）
- `takt clear`（セッション初期化）
- `takt --pipeline ...`（CI連携）

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

4. ローカル最終確認

```bash
make all
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

## 推奨タスク記述（例）

曖昧なタスク名だとレビューで戻りやすいため、目的・範囲・完了条件を明記します。

```yaml
task: >
  projectカードのHTML出力にカテゴリ属性を追加し、
  CSSフィルタリング仕様（docs/DD.md）に合わせる。
  完了条件は make all 成功と dist/index.html 出力確認。
piece: default
branch: feature/add-project-category-attr
worktree: true
```

---

## 推奨ピース方針

- 小規模修正: `backend-mini` 相当の短サイクル
- 複数ファイル変更: `default` 相当の `plan -> implement -> review`
- 設計変更を含む作業: `review-fix-default` 相当でレビュー往復を有効化

※ このリポジトリには専用ピースを同梱せず、既存の標準ピース利用を前提にします。

---

## 運用上の注意

- `.takt/` にはランタイム生成物をコミットしない（`.takt/.gitignore` 準拠）
- 生成物 `dist/` はコミット対象にしない（`.gitignore` 準拠）
- ドキュメント変更時は `docs/` だけを更新し、不要なツール設定を混入させない
- ブランチ命名は `feature/<description>` / `bugfix/<description>` を使用する

---

## トラブルシュート（最小）

### `takt run` が失敗する

1. `takt list` で失敗タスクを確認
2. 失敗理由が環境依存か実装修正かを切り分ける
3. 実装修正後に再実行し、最後は `make all` で確定する

### 差分が広がりすぎる

- タスクを分割して再投入する
- `review-fix-default` 相当の往復を使って段階的に収束させる
- 無関係ファイルの変更が混ざったら除外してやり直す

