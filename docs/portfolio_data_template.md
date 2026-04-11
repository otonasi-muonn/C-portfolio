# ポートフォリオデータテンプレート

このファイルは `src/data.c` に記入するすべてのデータ項目を網羅したテンプレートです。
空欄を埋めるだけでポートフォリオが完成する形式になっています。

**使い方:**
1. 各セクションの「記入欄」に値を書き込む
2. 完成した値を `src/data.c` の対応箇所に貼り付ける
3. `make all` を実行して `dist/index.html` を確認する

デモ値は `/* デモ値: ... */` コメントとして残してあります。置き換える際の参考にしてください。

---

## 1. サイト設定（SiteConfig）

> サイト全体のメタ情報です。検索エンジンやブラウザタブに影響します。

| フィールド | 説明 | なぜ必要か |
|-----------|------|-----------|
| `title` | サイトのタイトル。`<title>` タグとナビロゴに使われる | ブラウザタブ・検索結果に表示される第一印象 |
| `lang` | HTMLの言語コード（例: `ja`, `en`） | スクリーンリーダーや検索エンジンの言語判定に必要 |
| `description` | サイトの説明文。`<meta name="description">` に使われる | 検索結果のスニペットに表示される。100〜150文字が目安 |
| `css_path` | CSSファイルのパス（通常は `main.css` のまま） | レイアウト・スタイルの読み込み先 |

```c
const SiteConfig site_config = {
  .title       = "無音ポートフォリオ",
  .lang        = "ja",
  .description = "CCSSポートフォリオ！",
  .css_path    = "main.css"
};
```

---

## 2. プロフィール（Profile）

> 「About」セクションに表示されます。採用担当者が最初に読む部分です。

| フィールド | 説明 | 記入のヒント |
|-----------|------|------------|
| `name` | 氏名（フルネーム） | 本名でも活動名でも可。一貫性を持たせること |
| `affiliation` | 所属（大学・学年・専攻、または企業・役職） | 最新の所属を書く。卒業後は「○○大学 卒業」でも可 |
| `tagline` | 自分を一言で表すキャッチフレーズ | 技術的な特徴や個性を15〜30文字程度で表現する |
| `bio` | 自己紹介文（数文〜数段落） | 経歴・得意分野・情熱を具体的に書く。150〜300文字が読みやすい |

```c
const Profile profile = {
  /* name: メイン表示名（ニックネーム） / name_sub: サブ表示名（本名） */
  .name        = "無音",
  .name_sub    = "田中 那於斗",
  /* デモ値: .affiliation = "○○大学 情報工学科 2年" */
  .affiliation = "福岡工業大学 情報工学部 情報工学科 2年 / 情報技術研究部(じょぎ)",
  /* 座右の銘「その場にとどまるためには全力で走り続けなければならない」を短縮 */
  .tagline     = "止まらず走り続けることが\"普通\"でいること",
  .bio         = "福岡工業大学でWebからシステム・モバイルまで幅広く開発を手がけています。"
                 "TypeScript/React/Next.jsによるモダンWebアプリ、Node.jsによる非同期処理、"
                 "VS Code拡張「Code Grimoire」、C言語(WebAssembly)・Rust・Dart/Flutterなど"
                 "多様な技術スタックで実践的なプロダクトを継続的に制作しています。"
                 "Web Speed Hackathon 2026にも参加するなど、常に新しい技術領域に挑戦中です。"
};
```

---

## 3. ソーシャルリンク（SocialLink[]）

> プロフィールセクション下部に表示されるリンク一覧です。

| フィールド | 説明 | 記入のヒント |
|-----------|------|------------|
| `service` | サービス名（例: GitHub, X, LinkedIn） | 表示名として使われる |
| `url` | プロフィールページのURL | 必ず `https://` から始める |
| `label` | リンクの表示テキスト（例: @ユーザー名） | `@ユーザー名` または `表示名` |

```c
const SocialLink social_links[] = {
  { .service = "GitHub", .url = "https://github.com/otonasi-muonn",          .label = "@otonasi-muonn" },
  { .service = "X",      .url = "https://x.com/otonasi_muonn",               .label = "@otonasi_muonn" },
  { .service = "Gitty",  .url = "https://gitty-code.com/user/otonasi-muonn", .label = "Gitty Profile" },
};
```

---

## 4. スキル・ステータス（SkillSection）

> 外部サービス連携による客観的なコーディング実績の表示セクションです。

| フィールド | 説明 | 記入のヒント |
|-----------|------|------------|
| `description` | セクション冒頭の説明文 | 何が表示されているかを一文で説明する |
| `github_stats_url` | GitHub Readme Stats の総合ステータス画像URL | `username=` の部分をあなたのGitHubユーザー名に変える |
| `github_langs_url` | GitHub Readme Stats の言語割合画像URL | 同上 |
| `github_productive_url` | Profile Summary Cards の活動時間グラフURL | `username=` を変更、`utcOffset=9` はJSTのまま |
| `gitty_url` | Gittyプロフィールページへのリンク | あなたのGittyアカウントURLを指定する |

```c
const SkillSection skill_section = {
  .description = "GitHubの統計情報や言語使用率、活動時間帯グラフを表示しています。",
  .github_stats_url      = "https://github-readme-stats.vercel.app/api?username=otonasi-muonn&show_icons=true&theme=github_dark&rank_icon=github&hide_border=true&bg_color=161b22",
  .github_langs_url      = "https://github-readme-stats.vercel.app/api/top-langs/?username=otonasi-muonn&layout=compact&theme=github_dark&hide_border=true&bg_color=161b22",
  .github_productive_url = "https://github-profile-summary-cards.vercel.app/api/cards/productive-time?username=otonasi-muonn&theme=github_dark&utcOffset=9",
  .gitty_url             = "https://gitty-code.com/user/otonasi-muonn"
};
```

---

## 5. キャリア・学習歴（CareerEvent[]）

> 「Career」タイムラインに時系列で表示されます。

| フィールド | 説明 | 記入のヒント |
|-----------|------|------------|
| `date` | 年月（`YYYY-MM` 形式） | 表示にそのまま使われる。`2024-04` など |
| `title` | イベントのタイトル | 入学、インターン開始、資格取得、受賞など |
| `description` | イベントの詳細説明（1〜2文） | 何を学んだか・何をしたかを具体的に書く |

```c
/* キャリア・学習歴はユーザーが後で記入してください */
const CareerEvent career_events[] = {
  { .date = "2025-04-02",
    .title = "入学",
    .description = "福岡工業大学入学" },
};
  { .date = "2025-04-08",
    .title = "入部",
    .description = "情報技術研究部に入部" },
};
  { .date = "2025-05-24/25",
    .title = "初ハッカソン参加",
    .description = "初めてのハッカソン参加！初めては部内で行う部内ハッカソンでした" },
};
  { .date = "2025-06-07",
    .title = "長期イベント初日",
    .description = "北九州で行われる[チャレンジキャラバン](https://challecara.org/)というイベントに参加！" },
};
  { .date = "2025-07-30",
    .title = "母校でお手伝い",
    .description = "私の母校、博多青松高校に行き、体験入学のお手伝いをしました。　またそれから私が高大連携という制度を利用したいため、その宣伝？お話を中学生や親御さんの前で行いました" },
};
  { .date = "2025-08-15/16/17",
    .title = "ブラッシュアップソン",
    .description = "部内で過去先輩たちが行った作品をブラッシュアップするというブラッシュアップソンを行いました。　チームとして優秀賞が取れ、初めて賞を取れたのでうれしかったです！" },
};
  { .date = "2025-08-20",
    .title = "StepByCodeさんのもくもく会参加",
    .description = "じょぎとも関わりのある、StepByCodeさんのもくもく会に参加させていただきました！" },
};
  { .date = "2025-08-26",
    .title = "ネクストエンジニア・カタパルト 2025　入構式",
    .description = "ネクストエンジニア・カタパルト 2025に参加しました！" },
};
  { .date = "2025-08-27/28",
    .title = "福岡学生ゲームジャム運営",
    .description = "じょぎや他団体が運営する、福岡学生ゲームジャムの運営のお手伝いをさせていただきました！" },
};
  { .date = "2025-08-28/29",
    .title = "Progate ハッカソン",
    .description = "運営のお手伝いと同時にハッカソンにも参加していました！　オンラインでのハッカソンは初めてただっため面白かったです！　先輩方のおかげで最優秀賞をいただきました" },
};
  { .date = "2025-09-03",
    .title = "大学の夏季講座参加",
    .description = "クラウドAIアプリ開発入門という夏季講座に参加しました！Flaskを学ぶことができました" },
};
  { .date = "2025-09-16/17/18",
    .title = "ハックツハッカソン ~イクチオカップ~参加",
    .description = "3日間の泊まり込みのハッカソンでした！　3日間で4時間ほどしか寝ないという徹夜ハッカソンを行っていました..." },
};
  { .date = "2025-10-11",
    .title = "誕生日",
    .description = "19歳になりました！！！" },
};
  { .date = "2025-11-01",
    .title = "成果報告会",
    .description = "夏期講座のクラウドAIアプリ開発入門で制作したアプリを発表する成果報告会を行いました！" },
};
  { .date = "2025-11-02",
    .title = "立花祭",
    .description = "大学のイベント！立花祭を行いました、作品の展示や屋台などを行っていました。" },
};
  { .date = "2025-11-08",
    .title = "PHPカンファレンス参加",
    .description = "初カンファレンス参加です！　様々なお話を聞けて面白かったです。" },
};
  { .date = "2025-11-22",
    .title = "OSSカンファレンス",
    .description = "PHPカンファレンスが楽しかったため、さらに参加してきました！　いろいろなOSSについてのお話などを聞かせていただきました。" },
};
  { .date = "2025-12-20/21",
    .title = "ハックツハッカソン ~プテラカップ~",
    .description = "二日間のハッカソンでした！　会場が地元だったためうれしかったです。" },
};
  { .date = "2026-01-17/18",
    .title = "技育CAMP参加",
    .description = "タスクの割り振りなどを先輩から知ることができました" },
};
  { .date = "2026-01-17/18",
    .title = "技育CAMP参加",
    .description = "タスクの割り振りなどを先輩から知ることができました" },
};
  { .date = "2026-02-16/17/18",
    .title = "TRI HACKATHON",
    .description = "じょぎ・ピアプロ・FIG.comの三団体でハッカソンを行いました！　個人で参加したのですが自分の代表作的作品CodeGrimoireをこのハッカソンで作ることができました。　また特殊なハッカソンであったため面白かったです。" },
};
  { .date = "2026-03-17/18/19",
    .title = "ハックツハッカソン ~メガロカップ~",
    .description = "独自のアーキテクチャCCSSでコンパイラやトランスパイラを制作しました！　楽しかったです。" },
};
  { .date = "2026-03-20/21",
    .title = "Web Speed Hackathon 2026",
    .description = "こういったチューニング系のハッカソンは初めて参加したので楽しかったです！　レギュレーション違反をしてしまいましたが、様々なことを得ることができました！！！" },
};
  { .date = "2026-03-28",
    .title = "内政ハッカソン発表",
    .description = "部内の内政を整えるハッカソンの発表がありました。　テストやログ設計などをしっかりと意識したいと感じ、勉強になるイベントでした。　製作途中の発表にはなってしまったがGitHubの使い方、進め方などを評価されチームでマネジメント賞をいただくことができました。" },
};
  { .date = "2026-04-01",
    .title = "2年生 & 幹部就任",
    .description = "2年生に上がると同時に、じょぎの幹部になりました" },
};
```

---

## 6. プロジェクト（Project[]）

> 「Projects」セクションにカード形式で表示されます。カテゴリフィルターが自動生成されます。

| フィールド | 説明 | 記入のヒント |
|-----------|------|------------|
| `title` | プロジェクト名 | リポジトリ名でも、わかりやすい名称でも可 |
| `url` | GitHubリポジトリや公開URLへのリンク | `https://` から始める |
| `period` | 制作時期（`YYYY-MM` 形式） | 開始月、または主要な開発期間の開始月 |
| `category` | カテゴリ名（フィルタリングに使われる） | 同じ文字列のものがひとつのグループになる。例: `Web`, `System`, `Tool` |
| `description` | プロジェクトの説明（1〜3文） | 何を作ったか・どんな技術を使ったかを書く |

```c
/* GitHub API (otonasi-muonn) pushed_at 降順 上位8件・フォーク除外 */
const Project projects[] = {
  { .title    = "C-portfolio",
    .url      = "https://github.com/otonasi-muonn/C-portfolio",
    .period   = "2026-03",
    .category = "C",
    .description = "C言語でHTMLを生成するポートフォリオサイト。makeベースでローカル開発・GitHub Pages公開に対応し、JavaScriptに依存しないCSSオンリーのUIを実現。" },

  { .title    = "megalo2026",
    .url      = "https://github.com/otonasi-muonn/megalo2026",
    .period   = "2026-03",
    .category = "TypeScript",
    .description = "スワイプで風を起こしキャラクターを運ぶ物理演算アクション＋ステージ作成・共有ゲーム。React＋Viteのフロントとホノバックエンドを持つモノレポ構成。" },

  { .title    = "Code-Grimoire",
    .url      = "https://github.com/otonasi-muonn/Code-Grimoire",
    .period   = "2026-02",
    .category = "TypeScript",
    .description = "コードを魔方陣に！！！" },

  { .title    = "Code-Grimoire-Core",
    .url      = "https://github.com/otonasi-muonn/Code-Grimoire-Core",
    .period   = "2026-03",
    .category = "Other",
    .description = "コード依存関係を魔方陣として可視化するVS Code拡張のコアエンジン。PixiJS＋d3-forceで数万ファイル規模でも60fps描画を実現するプラグイン方式の基盤。" },

  { .title    = "otonasi-muonn",
    .url      = "https://github.com/otonasi-muonn/otonasi-muonn",
    .period   = "2025-08",
    .category = "Other",
    .description = "GitHubプロフィールリポジトリ。GitHub統計・言語割合・Streak・トロフィーなどを表示するREADMEを管理。" },

  { .title    = "phase4_collelis",
    .url      = "https://github.com/otonasi-muonn/phase4_collelis",
    .period   = "2025-12",
    .category = "TypeScript",
    .description = "Next.js（App Router）・TypeScript・Tailwind CSSを用いて構築したWebアプリケーション。" },

  { .title    = "dameninngenF",
    .url      = "https://github.com/otonasi-muonn/dameninngenF",
    .period   = "2025-10",
    .category = "TypeScript",
    .description = "自身の「ダメ人間エピソード」を投稿し、いいね数でランキングされるSNSアプリ。TypeScript・Next.js・Reactで実装。" },

  { .title    = "laratter",
    .url      = "https://github.com/otonasi-muonn/laratter",
    .period   = "2025-08",
    .category = "Blade",
    .description = "Laravelを用いたTwitterクローンWebアプリ。ブックマーク機能（多対多リレーション）の設計・実装を中心に取り組んだ学習課題。" },
};
```

---

## 推奨項目（現在未実装・将来追加を検討）

以下の項目は現在の `models.h` には存在しませんが、ポートフォリオとして効果的な情報です。
将来的にデータモデルを拡張する際の参考にしてください。

### プロフィール拡張

| 推奨項目 | 効果 | 実装イメージ |
|---------|------|------------|
| プロフィール画像URL | 視覚的な第一印象を大幅に改善する | `Profile.avatar_url` フィールドを追加 |
| メールアドレス（任意） | 直接コンタクトの窓口になる | `Profile.contact_email` |
| 居住地・タイムゾーン | 採用担当者が働き方をイメージしやすくなる | `Profile.location` |

### プロジェクト拡張

| 推奨項目 | 効果 | 実装イメージ |
|---------|------|------------|
| 使用技術リスト | 複数技術スタックをアピールできる | `Project.tech_stack` (カンマ区切りの文字列) |
| スクリーンショットURL | 視覚的に成果物を示せる | `Project.image_url` |
| デモURL | 実際に動くものを見せられる | `Project.demo_url` |
| 定量的な実績 | 「〇〇%改善」「〇〇件処理」など数字で成果を示せる | `Project.metrics` |
| 制作背景・課題 | なぜ作ったかが伝わり共感を得やすくなる | `Project.motivation` |

### 新規セクション

| 推奨セクション | 効果 |
|--------------|------|
| 資格・認定（Certification） | TOEIC・情報処理技術者試験・ベンダー資格などを客観的に示せる |
| OSS貢献（Contributions） | 他プロジェクトへのPull Requestや Issue報告の実績 |
| 受賞・表彰（Awards） | コンテスト入賞や奨学金受給など |
| 技術ブログ・発表（Publications） | Zenn/Qiita記事・勉強会発表のリンク |
| 連絡フォーム情報（Contact） | 直接連絡できる手段を明示する |

---

## チェックリスト（公開前確認）

- [ ] `data.c` のデモ値をすべて実際の値に置き換えた
- [ ] `social_links` の URL がすべて正しいプロフィールを指している
- [ ] GitHub Stats の `username=` パラメーターがあなたのアカウント名になっている
- [ ] `career_events` が時系列順（古い順）に並んでいる
- [ ] `projects` の `category` 値に表記ゆれがない（大文字・小文字・全半角を統一）
- [ ] `make all` を実行して `dist/index.html` が正常に生成されることを確認した
- [ ] `dist/index.html` をブラウザで開いて全セクションが正しく表示されることを確認した
