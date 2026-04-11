#ifndef MODELS_H
#define MODELS_H

#include <stddef.h>

/* サイト全体の設定 */
typedef struct {
  const char *title;
  const char *lang;
  const char *description;
  const char *css_path;
} SiteConfig;

/* プロフィール情報 */
typedef struct {
  const char *name;      /* メイン表示名（ニックネーム等） */
  const char *name_sub;  /* サブ表示名（本名等） */
  const char *affiliation;
  const char *tagline;
  const char *bio;
} Profile;

/* 外部アカウントリンク */
typedef struct {
  const char *service;
  const char *url;
  const char *label;
} SocialLink;

/* 技術スタック・ステータス（ウィジェット＆外部連携） */
typedef struct {
  const char *description;
  const char *github_stats_url;
  const char *github_langs_url;
  const char *github_productive_url; /* 先輩が使っていた活動時間グラフ */
  const char *gitty_url;
} SkillSection;

/* キャリア・学習歴イベント */
typedef struct {
  const char *date;
  const char *title;
  const char *description;
} CareerEvent;

/* プロジェクト */
typedef struct {
  const char *title;
  const char *url;
  const char *period;
  const char *category;
  const char *description;
} Project;

/* データ配列の外部参照宣言 */
extern const SiteConfig site_config;
extern const Profile profile;
extern const SocialLink social_links[];
extern const size_t social_links_count;
extern const SkillSection skill_section;
extern const CareerEvent career_events[];
extern const size_t career_events_count;
extern const Project projects[];
extern const size_t projects_count;

#endif /* MODELS_H */
