# GitHub Copilot Instructions

## 🎯 Project Specific Rules (プロジェクト固有のルール)
<!-- 
新しくプロジェクトを開始する際、ここにこのプロジェクト独自のルールやコーディング規約を追記してください。
（例：状態管理にはZustandを使用すること、APIの呼び出しは常に src/lib/api.ts を経由すること、など）
-->
- 

---

## 🤖 AI Assistant Directives (AIへの基本指示)
コードを生成・提案する際は、以下のルールに必ず従ってください。

1. **言語設定**: 回答、コード内のコメント、コミットメッセージは必ず**日本語**で出力してください。
2. **設計駆動開発**: 実装を始める前に、必ず `docs/` ディレクトリ配下にある関連ドキュメント（機能一覧、DB設計、権限設計など）を参照し、その設計に準拠したコードを生成してください。
3. **コーディングスタイル**: 簡潔で可読性の高いコードを心がけ、DRY原則やYAGNI原則を意識してください。エラーハンドリングは適切に行い、握りつぶさないようにしてください。

---

## 📦 Repository Context (リポジトリの前提知識)

### Repository Purpose
This is a **Japanese-language GitHub repository template**. It is not a runnable application — it provides a starting structure for new projects. When used as a template, contributors run `setup.sh` to select a documentation tier and begin customizing.

### Initial Setup Flow
After creating a new repo from this template:
1. Run `chmod +x setup.sh && ./setup.sh` — interactively selects the documentation set and removes itself when done
2. Clone issue labels from the source repo: `gh label clone otonasi-muonn/template-repo --force`
3. The script removes one of the two doc tiers (`docs/small/` or `docs/standard/`) and promotes the chosen tier's contents to `docs/`

### Documentation Tiers
- **Small** (`docs/small/`): Single-page design doc (`01_api-design.md`, `02_db-design.md`, `03_security.md`, `04_ai-design.md`, `05_deployment.md`)
- **Standard** (`docs/standard/`): Full design doc set — feature list, tech stack, screen flow, permissions, ERD, directory structure, infrastructure, logging, schedule/issues
After `setup.sh` runs, these subdirectories are gone and the chosen docs live directly under `docs/`.

### Formatting Conventions (EditorConfig)
- Default: UTF-8, LF line endings, 2-space indent, trim trailing whitespace, final newline
- Python files: 4-space indent
- Go files and Makefiles: tab indent
- Markdown files: trailing whitespace is **preserved** (two trailing spaces = line break)

### Branch Naming
From CONTRIBUTING.md:
- Features: `feature/<description>` (e.g., `feature/add-login`)
- Bug fixes: `bugfix/<description>` (e.g., `bugfix/fix-header`)

### Pull Requests
Use the PR template at `.github/PULL_REQUEST_TEMPLATE.md`. Sections (in Japanese):
1. 背景・目的 — background/purpose; link related issues with `Close #N`
2. 変更内容 — bullet list of changes
3. 動作確認・テスト — verification steps; attach screenshots for UI changes
4. レビューポイント — design concerns or performance questions for reviewers
5. チェックリスト — build/tests pass, docs updated, no debug code left

### Issues
Five templates under `.github/ISSUE_TEMPLATE/`:
| File | Type | Label |
|------|------|-------|
| `01_task.yml` | Task | — |
| `02_question.yml` | Question | — |
| `03_epic.yml` | Epic | — |
| `04_bug_report.yml` | Bug | `bug` |
| `05_feature_request.yml` | Feature request | — |

### GitHub Actions
- **stale.yml**: Issues and PRs go stale after 60 days of inactivity, close after 7 more days
- **labeler.yml**: Auto-labels PRs — `documentation` for changes to `docs/**` or `README.md`; `bug` for paths matching `*bug*` or `*fix*`
- **greetings.yml**: Automated greeting on first issue/PR