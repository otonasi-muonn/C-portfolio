# C-portfolio

C言語で静的HTMLを生成し、CSSで動的UIを実現するポートフォリオサイトです。  
JavaScriptに依存せず、`make` ベースでローカル開発とGitHub Pages公開を行います。

---

## ドキュメント

主要な設計資料は `docs/` を参照してください。

| ドキュメント | 内容 |
|------------|------|
| [`docs/_complete.md`](docs/_complete.md) | 企画全体の背景・技術スタック・機能一覧 |
| [`docs/DD.md`](docs/DD.md) | 実装詳細（データモデル、描画ルール、CSS仕様） |
| [`docs/20260401_ccss_v2_proposal.md`](docs/20260401_ccss_v2_proposal.md) | Cフロント / CSSバック方針の拡張提案 |

---

## 開発運用ガイド（TAKT / ECC）

`.Myrepository` と同じく、運用ガイドを `docs/Wiki/` に集約しています。

| ドキュメント | 内容 |
|------------|------|
| [TAKT ガイド](docs/Wiki/takt-guide.md) | TAKT のインストール・使い方・ピース運用（C-portfolio向け） |
| [ECC ガイド](docs/Wiki/ecc-guide.md) | Everything Claude Code と Skills 管理の運用手順 |
| [ベストプラクティスガイド](docs/Wiki/best-practice-guide.md) | C-portfolio で守る実装・検証・レビューの実践ルール |

### TAKT 最短運用（3コマンド）

```bash
takt
takt run
takt list
```

最後は必ず `make all` でローカル検証してください。

---

## 開発環境の構築

### 前提ツール

- `gcc`（C11対応）
- `make`
- `git`
- `gh`（任意: ラベル同期などで利用）

### セットアップ手順

1. リポジトリを取得します。

```bash
git clone <YOUR_REPOSITORY_URL>
cd C-portfolio
```

2. ビルドとHTML生成を実行します。

```bash
make all
```

3. 生成物を確認します。

```text
dist/index.html
dist/main.css
```

---

## よく使うコマンド

| コマンド | 内容 |
|---------|------|
| `make build` | `generator` バイナリをコンパイル |
| `make generate` | `dist/index.html` を生成し、CSSをコピー |
| `make all` | `build` + `generate` をまとめて実行 |
| `make clean` | `generator` と `dist/` を削除 |

---

## ファイル・ディレクトリ説明

| パス | 説明 |
|-----|------|
| `src/` | C言語のジェネレーター本体（`main.c`, `render.c`, `data.c` など） |
| `styles/main.css` | UIスタイルとCSS状態管理ロジック |
| `dist/` | ビルド生成物（Git管理対象外） |
| `.agents/` | ECC運用のための最小Skills構成（有効スキルと候補プール） |
| `.takt/` | TAKTの最小運用設定（ログ類はコミットしない） |
| `.github/workflows/deploy.yml` | GitHub Actions による Pages デプロイ |
| `docs/` | 設計・提案・改善記録 |

---

## コントリビューション

貢献時は [CONTRIBUTING.md](CONTRIBUTING.md) を確認してください。  
コミュニティ参加時は [CODE_OF_CONDUCT.md](CODE_OF_CONDUCT.md) の遵守をお願いします。

## ライセンス

このプロジェクトは [MIT License](LICENSE) のもとで公開されています。
