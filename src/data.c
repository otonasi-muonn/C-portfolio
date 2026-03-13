#include "models.h"

/* サイト設定 */
const SiteConfig site_config = {
  .title       = "Portfolio - C-Powered",
  .lang        = "ja",
  .description = "C言語で静的生成されたポートフォリオサイト",
  .css_path    = "main.css"
};

/* プロフィール */
const Profile profile = {
  .name        = "山田 太郎",
  .affiliation = "○○大学 情報工学科 2年",
  .tagline     = "低レイヤーから Web まで — C言語で世界を組み立てる",
  .bio         = "大学1年次よりC言語を中心にシステムプログラミングを学習。"
                 "組込み開発からWebの静的サイト生成まで、低レイヤーの知見を"
                 "幅広い領域に応用することに情熱を注いでいます。"
};

/* ソーシャルリンク */
const SocialLink social_links[] = {
  { .service = "GitHub",  .url = "https://github.com/example",  .label = "@example" },
  { .service = "X",       .url = "https://x.com/example",       .label = "@example" },
  { .service = "Gitty",   .url = "https://gitty.example.com/u", .label = "Gitty Profile" },
};
const size_t social_links_count = sizeof(social_links) / sizeof(social_links[0]);
/* スキル・ステータス情報 */
const SkillSection skill_section = {
  .description = "私が使用している言語の割合や活動時間、客観的なコーディング分析については、以下のステータスとGittyプロフィールをご覧ください。",
  /* GitHub Readme Stats (総合ステータス) */
  .github_stats_url = "https://github-readme-stats.vercel.app/api?username=otonasi-muonn&show_icons=true&theme=github_dark&rank_icon=github&hide_border=true&bg_color=161b22",
  /* Top Languages (言語割合) */
  .github_langs_url = "https://github-readme-stats.vercel.app/api/top-langs/?username=otonasi-muonn&layout=compact&theme=github_dark&hide_border=true&bg_color=161b22",
  /* Profile Summary Cards (活動時間帯グラフ) */
  .github_productive_url = "https://github-profile-summary-cards.vercel.app/api/cards/productive-time?username=otonasi-muonn&theme=github_dark&utcOffset=9",
  /* Gittyへのリンク */
  .gitty_url = "https://gitty-code.com/user/otonasi-muonn"
};

/* キャリア・学習歴 */
const CareerEvent career_events[] = {
  { .date = "2024-04",
    .title = "○○大学 情報工学科 入学",
    .description = "コンピュータサイエンスの基礎を学び始める" },
  { .date = "2024-07",
    .title = "C言語プログラミングコンテスト参加",
    .description = "学内プログラミングコンテストにて C言語部門に出場" },
  { .date = "2024-10",
    .title = "GitHub での OSS 活動開始",
    .description = "個人プロジェクトを公開し、バージョン管理とCI/CDを実践" },
  { .date = "2025-03",
    .title = "C言語 SSG ポートフォリオ構築",
    .description = "本サイトの開発を開始。C言語による静的サイト生成に挑戦" },
};
const size_t career_events_count = sizeof(career_events) / sizeof(career_events[0]);

/* プロジェクト */
const Project projects[] = {
  { .title    = "C-Portfolio (本サイト)",
    .url      = "https://github.com/example/c-portfolio",
    .period   = "2025-03",
    .category = "Web",
    .description = "C言語で静的HTMLを生成するポートフォリオサイト。"
                   "snprintf による安全な文字列操作と HTML エスケープを実装。" },
  { .title    = "Mini Shell",
    .url      = "https://github.com/example/mini-shell",
    .period   = "2024-12",
    .category = "System",
    .description = "POSIX 準拠のミニマルなシェル実装。パイプ、リダイレクト、"
                   "ジョブ制御をサポート。" },
  { .title    = "Memory Allocator",
    .url      = "https://github.com/example/malloc",
    .period   = "2024-09",
    .category = "System",
    .description = "独自の動的メモリアロケータ。First-fit / Best-fit アルゴリズムを実装し"
                   "フラグメンテーション低減を検証。" },
  { .title    = "Task Tracker CLI",
    .url      = "https://github.com/example/task-cli",
    .period   = "2024-11",
    .category = "Tool",
    .description = "C言語製のコマンドラインタスク管理ツール。"
                   "ファイルベースの永続化とカラー出力に対応。" },
};
const size_t projects_count = sizeof(projects) / sizeof(projects[0]);
