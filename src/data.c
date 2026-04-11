#include "models.h"

/* サイト設定 */
const SiteConfig site_config = {
  .title       = "無音ポートフォリオ",
  .lang        = "ja",
  .description = "CCSSポートフォリオ！",
  .css_path    = "main.css"
};

/* プロフィール */
const Profile profile = {
  .name        = "無音",
  .name_sub    = "田中 那於斗",
  .affiliation = "福岡工業大学 情報工学部 情報工学科 2年 / 情報技術研究部(じょぎ)",
  .tagline     = "止まらず走り続けることが\"普通\"でいること",
  .bio         = "福岡工業大学でWebからシステム・モバイルまで幅広く開発を手がけています。"
                 "TypeScript/React/Next.jsによるモダンWebアプリ、Node.jsによる非同期処理、"
                 "VS Code拡張「Code Grimoire」、C言語(WebAssembly)・Rust・Dart/Flutterなど"
                 "多様な技術スタックで実践的なプロダクトを継続的に制作しています。"
                 "Web Speed Hackathon 2026にも参加するなど、常に新しい技術領域に挑戦中です。"
};

/* ソーシャルリンク */
const SocialLink social_links[] = {
  { .service = "GitHub", .url = "https://github.com/otonasi-muonn",              .label = "@otonasi-muonn" },
  { .service = "X",      .url = "https://x.com/otonasi_muonn",                   .label = "@otonasi_muonn" },
  { .service = "Gitty",  .url = "https://gitty-code.com/user/otonasi-muonn",     .label = "Gitty Profile" },
};
const size_t social_links_count = sizeof(social_links) / sizeof(social_links[0]);

/* スキル・ステータス情報 */
const SkillSection skill_section = {
  .description = "GitHubの統計情報や言語使用率、活動時間帯グラフを表示しています。",
  .github_stats_url      = "https://github-readme-stats.vercel.app/api?username=otonasi-muonn&show_icons=true&theme=github_dark&rank_icon=github&hide_border=true&bg_color=161b22",
  .github_langs_url      = "https://github-readme-stats.vercel.app/api/top-langs/?username=otonasi-muonn&layout=compact&theme=github_dark&hide_border=true&bg_color=161b22",
  .github_productive_url = "https://github-profile-summary-cards.vercel.app/api/cards/productive-time?username=otonasi-muonn&theme=github_dark&utcOffset=9",
  .gitty_url             = "https://gitty-code.com/user/otonasi-muonn"
};

/* キャリア・学習歴 */
const CareerEvent career_events[] = {
  { .date = "2025-04-02",
    .title = "入学",
    .description = "福岡工業大学入学" },
  { .date = "2025-04-08",
    .title = "入部",
    .description = "情報技術研究部（じょぎ）に入部" },
  { .date = "2025-05-24/25",
    .title = "初ハッカソン参加",
    .description = "初めてのハッカソン参加！初めては部内で行う部内ハッカソンでした" },
  { .date = "2025-06-07",
    .title = "長期イベント初日",
    .description = "北九州で行われるチャレンジキャラバンというイベントに参加！" },
  { .date = "2025-07-30",
    .title = "母校でお手伝い",
    .description = "母校・博多青松高校で体験入学のお手伝い。高大連携制度の紹介を中学生・保護者の前で行いました" },
  { .date = "2025-08-15/16/17",
    .title = "ブラッシュアップソン",
    .description = "部内で先輩たちの過去作品をブラッシュアップするイベントを開催。チームとして優秀賞を受賞！" },
  { .date = "2025-08-20",
    .title = "StepByCodeさんのもくもく会参加",
    .description = "じょぎとも関わりのある、StepByCodeさんのもくもく会に参加させていただきました！" },
  { .date = "2025-08-26",
    .title = "ネクストエンジニア・カタパルト 2025 入構式",
    .description = "ネクストエンジニア・カタパルト 2025に参加しました！" },
  { .date = "2025-08-27/28",
    .title = "福岡学生ゲームジャム運営",
    .description = "じょぎや他団体が運営する福岡学生ゲームジャムの運営スタッフとして参加！" },
  { .date = "2025-08-28/29",
    .title = "Progateハッカソン",
    .description = "運営と並行してハッカソンにも参加。オンラインは初体験で新鮮でした！先輩方のおかげで最優秀賞をいただきました" },
  { .date = "2025-09-03",
    .title = "大学の夏季講座参加",
    .description = "クラウドAIアプリ開発入門という夏季講座に参加！Flaskを学ぶことができました" },
  { .date = "2025-09-16/17/18",
    .title = "ハックツハッカソン ~イクチオカップ~",
    .description = "3日間の泊まり込みハッカソン。3日間でわずか4時間睡眠という徹夜ハッカソンでした..." },
  { .date = "2025-10-11",
    .title = "誕生日",
    .description = "19歳になりました！！！" },
  { .date = "2025-11-01",
    .title = "成果報告会",
    .description = "夏季講座・クラウドAIアプリ開発入門で制作したアプリを発表する成果報告会を開催" },
  { .date = "2025-11-02",
    .title = "立花祭",
    .description = "大学祭！立花祭にて作品展示や屋台などを行いました" },
  { .date = "2025-11-08",
    .title = "PHPカンファレンス参加",
    .description = "初カンファレンス参加！様々なセッションを聴講してとても刺激になりました" },
  { .date = "2025-11-22",
    .title = "OSSカンファレンス参加",
    .description = "PHPカンファレンスが楽しかったため続けて参加！OSSに関する幅広い話を聴講しました" },
  { .date = "2025-12-20/21",
    .title = "ハックツハッカソン ~プテラカップ~",
    .description = "2日間のハッカソン。会場が地元だったためうれしかったです" },
  { .date = "2026-01-17/18",
    .title = "技育CAMP参加",
    .description = "タスクの割り振りなど、先輩からプロジェクト管理の知見を学ぶことができました" },
  { .date = "2026-02-16/17/18",
    .title = "TRI HACKATHON",
    .description = "じょぎ・ピアプロ・FIG.comの三団体ハッカソン。個人参加で代表作CodeGrimoireを開発できました" },
  { .date = "2026-03-17/18/19",
    .title = "ハックツハッカソン ~メガロカップ~",
    .description = "独自アーキテクチャCCSSでコンパイラ・トランスパイラを制作！とても楽しかったです" },
  { .date = "2026-03-20/21",
    .title = "Web Speed Hackathon 2026",
    .description = "チューニング系ハッカソン初参加。レギュレーション違反はありましたが多くの学びを得ました！" },
  { .date = "2026-03-28",
    .title = "内政ハッカソン発表",
    .description = "部内整備ハッカソン発表。テスト・ログ設計の重要性を実感。GitHubの進め方でチームがマネジメント賞を受賞" },
  { .date = "2026-04-01",
    .title = "2年生 & 幹部就任",
    .description = "2年生に進級すると同時に、じょぎの幹部に就任しました" },
};
const size_t career_events_count = sizeof(career_events) / sizeof(career_events[0]);

/* プロジェクト（otonasi-muonn GitHub 公開リポジトリ、pushed_at 降順 上位8件） */
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
const size_t projects_count = sizeof(projects) / sizeof(projects[0]);
