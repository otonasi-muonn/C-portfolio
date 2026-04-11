#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include "render.h"

#define BUF_SIZE 8192
#define ESC_SIZE 1024

/* ---- HTMLエスケープ ---- */
void html_escape(const char *src, char *dest, size_t dest_size) {
  size_t di = 0;
  if (!dest || dest_size == 0) return;
  if (!src) {
    fprintf(stderr, "[WARN] html_escape に NULL ポインタが渡されました\n");
    dest[0] = '\0';
    return;
  }

  for (size_t si = 0; src[si] != '\0' && di < dest_size - 1; si++) {
    const char *rep = NULL;
    switch (src[si]) {
      case '&':  rep = "&amp;";  break;
      case '<':  rep = "&lt;";   break;
      case '>':  rep = "&gt;";   break;
      case '"':  rep = "&quot;"; break;
      case '\'': rep = "&#39;";  break;
      default: break;
    }
    if (rep) {
      size_t len = strlen(rep);
      if (di + len >= dest_size) break;
      memcpy(dest + di, rep, len);
      di += len;
    } else {
      dest[di++] = src[si];
    }
  }
  dest[di] = '\0';
}

/* ---- ヘルパー: エスケープ済み文字列を返す ---- */
static void esc(const char *src, char *buf, size_t buf_size) {
  html_escape(src ? src : "", buf, buf_size);
}

/* ---- ヘルパー: snprintf の戻り値を必ず検証する ---- */
static int checked_snprintf(char *buf, size_t buf_size,
                            const char *context, const char *fmt, ...) {
  int ret;
  va_list args;

  if (!buf || buf_size == 0 || !context || !fmt) {
    fprintf(stderr, "[ERROR] checked_snprintf の引数が不正です\n");
    return 0;
  }

  va_start(args, fmt);
  ret = vsnprintf(buf, buf_size, fmt, args);
  va_end(args);

  if (ret < 0) {
    fprintf(stderr, "[ERROR] %s の snprintf で失敗しました\n", context);
    buf[0] = '\0';
    return 0;
  }

  if ((size_t)ret >= buf_size) {
    fprintf(stderr,
            "[WARN] %s の snprintf がバッファ上限を超えました "
            "(buf=%zu, required=%d)\n",
            context, buf_size, ret);
    buf[buf_size - 1] = '\0';
    return 0;
  }

  return 1;
}

/* ---- ヘルパー: カテゴリの代表インデックスを取得 ---- */
static size_t category_group_index(const Project *projects, size_t current_index) {
  const char *current = projects[current_index].category ?
                        projects[current_index].category : "";
  for (size_t i = 0; i < current_index; i++) {
    const char *prev = projects[i].category ? projects[i].category : "";
    if (strcmp(prev, current) == 0) return i;
  }
  return current_index;
}

/* ---- <head> ---- */
void render_head(FILE *fp, const SiteConfig *config) {
  char title[ESC_SIZE], desc[ESC_SIZE], css[ESC_SIZE];
  char buf[BUF_SIZE];

  esc(config->title, title, sizeof(title));
  esc(config->description, desc, sizeof(desc));
  esc(config->css_path, css, sizeof(css));

  if (!checked_snprintf(buf, sizeof(buf), "render_head",
    "<head>\n"
    "  <meta charset=\"UTF-8\">\n"
    "  <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n"
    "  <meta name=\"description\" content=\"%s\">\n"
    "  <title>%s</title>\n"
    "  <link rel=\"stylesheet\" href=\"%s\">\n"
    "</head>\n",
    desc, title, css)) {
    return;
  }
  fputs(buf, fp);
}

/* ---- ヘッダー・ナビゲーション ---- */
void render_header(FILE *fp, const SiteConfig *config) {
  char title[ESC_SIZE];
  char buf[BUF_SIZE];

  esc(config->title, title, sizeof(title));

  if (!checked_snprintf(buf, sizeof(buf), "render_header",
    "<header class=\"site-header\">\n"
    "  <nav class=\"global-nav\">\n"
    "    <a href=\"#\" class=\"nav-logo\">%s</a>\n"
    "    <ul class=\"nav-links\">\n"
    "      <li><a href=\"#about\">About</a></li>\n"
    "      <li><a href=\"#skills\">Skills</a></li>\n"
    "      <li><a href=\"#works\">Works</a></li>\n"
    "    </ul>\n"
    "  </nav>\n"
    "</header>\n",
    title)) {
    return;
  }
  fputs(buf, fp);
}

/* ---- プロフィールセクション ---- */
void render_profile(FILE *fp, const Profile *prof,
                    const SocialLink *links, size_t links_count) {
  char name[ESC_SIZE], name_sub[ESC_SIZE], aff[ESC_SIZE], tag[ESC_SIZE], bio[ESC_SIZE];
  char buf[BUF_SIZE];

  esc(prof->name, name, sizeof(name));
  esc(prof->name_sub ? prof->name_sub : "", name_sub, sizeof(name_sub));
  esc(prof->affiliation, aff, sizeof(aff));
  esc(prof->tagline, tag, sizeof(tag));
  esc(prof->bio, bio, sizeof(bio));

  if (!checked_snprintf(buf, sizeof(buf), "render_profile.header",
    "<section id=\"about\" class=\"section\">\n"
    "  <h2 class=\"section-title\">About</h2>\n"
    "  <div class=\"profile\">\n"
    "    <h3 class=\"profile-name\">%s</h3>\n"
    "    <p class=\"profile-name-sub\">%s</p>\n"
    "    <p class=\"profile-affiliation\">%s</p>\n"
    "    <p class=\"profile-tagline\">%s</p>\n"
    "    <p class=\"profile-bio\">%s</p>\n"
    "  </div>\n",
    name, name_sub, aff, tag, bio)) {
    return;
  }
  fputs(buf, fp);

  fputs("  <ul class=\"social-links\">\n", fp);
  for (size_t i = 0; i < links_count; i++) {
    char svc[ESC_SIZE], lbl[ESC_SIZE], url[ESC_SIZE];
    esc(links[i].service, svc, sizeof(svc));
    esc(links[i].label, lbl, sizeof(lbl));
    esc(links[i].url, url, sizeof(url));

    if (!checked_snprintf(buf, sizeof(buf), "render_profile.link",
      "    <li><a href=\"%s\" target=\"_blank\" rel=\"noopener noreferrer\""
      " class=\"social-link\">%s - %s</a></li>\n",
      url, svc, lbl)) {
      continue;
    }
    fputs(buf, fp);
  }
  fputs("  </ul>\n</section>\n", fp);
}

/* ---- スキル・ステータスセクション ---- */
void render_skills(FILE *fp, const SkillSection *skill) {
  char buf[BUF_SIZE];
  char desc[ESC_SIZE], stats[ESC_SIZE], langs[ESC_SIZE], prod[ESC_SIZE], gitty[ESC_SIZE];
  
  esc(skill->description, desc, sizeof(desc));
  esc(skill->github_stats_url, stats, sizeof(stats));
  esc(skill->github_langs_url, langs, sizeof(langs));
  esc(skill->github_productive_url, prod, sizeof(prod));
  esc(skill->gitty_url, gitty, sizeof(gitty));

  if (!checked_snprintf(buf, sizeof(buf), "render_skills",
    "<section id=\"skills\" class=\"section\">\n"
    "  <h2 class=\"section-title\">Skills & Status</h2>\n"
    "  <p class=\"skill-desc\" style=\"margin-bottom: var(--space-md);\">%s</p>\n"
    "  \n"
    "  <div style=\"display: flex; flex-wrap: wrap; gap: var(--space-md); margin-bottom: var(--space-lg); align-items: flex-start;\">\n"
    "    <img src=\"%s\" alt=\"GitHub Stats\" style=\"max-width: 100%%; border-radius: 8px;\">\n"
    "    <img src=\"%s\" alt=\"Top Languages\" style=\"max-width: 100%%; border-radius: 8px;\">\n"
    "    <img src=\"%s\" alt=\"Productive Time\" style=\"max-width: 100%%; border-radius: 8px;\">\n"
    "  </div>\n"
    "  \n"
    "  <div class=\"skill-card\" style=\"display: inline-block;\">\n"
    "    <h3 class=\"skill-name\">Gitty Profile</h3>\n"
    "    <p class=\"skill-desc\" style=\"margin-bottom: var(--space-sm);\">詳細なコーディング分析はこちら</p>\n"
    "    <a href=\"%s\" target=\"_blank\" rel=\"noopener noreferrer\" class=\"social-link\">Gitty で開く</a>\n"
    "  </div>\n"
    "</section>\n",
    desc, stats, langs, prod, gitty)) {
    return;
  }
  fputs(buf, fp);
}

/* ---- キャリアタイムライン ---- */
void render_career(FILE *fp, const CareerEvent *events, size_t count) {
  char buf[BUF_SIZE];

  fputs("<section class=\"career-section\">\n"
        "  <h2 class=\"section-title\">Career</h2>\n"
        "  <div class=\"timeline\">\n", fp);

  for (size_t i = 0; i < count; i++) {
    char date[ESC_SIZE], title[ESC_SIZE], desc[ESC_SIZE];
    esc(events[i].date, date, sizeof(date));
    esc(events[i].title, title, sizeof(title));
    esc(events[i].description, desc, sizeof(desc));

    if (!checked_snprintf(buf, sizeof(buf), "render_career.item",
      "    <div class=\"timeline-item\">\n"
      "      <span class=\"timeline-date\">%s</span>\n"
      "      <h3 class=\"timeline-title\">%s</h3>\n"
      "      <p class=\"timeline-desc\">%s</p>\n"
      "    </div>\n",
      date, title, desc)) {
      continue;
    }
    fputs(buf, fp);
  }
  fputs("  </div>\n</section>\n", fp);
}

/* ---- プロジェクト一覧 ---- */
void render_projects(FILE *fp, const Project *projects, size_t count) {
  char buf[BUF_SIZE];

  fputs("<section class=\"projects-section\">\n"
        "  <h2 class=\"section-title\">Projects</h2>\n"
        "  <div class=\"filter-container\">\n", fp);

  fputs("    <div class=\"project-filters\" role=\"radiogroup\""
        " aria-label=\"プロジェクトカテゴリフィルター\">\n"
        "      <input class=\"filter-radio\" type=\"radio\""
        " name=\"project-filter\" id=\"filter-all\" checked>\n"
        "      <label class=\"filter-label\" for=\"filter-all\">すべて</label>\n", fp);

  for (size_t i = 0; i < count; i++) {
    size_t group = category_group_index(projects, i);
    char cat[ESC_SIZE], filter_id[ESC_SIZE];
    if (group != i) continue;

    esc(projects[i].category, cat, sizeof(cat));
    if (!checked_snprintf(filter_id, sizeof(filter_id),
                          "render_projects.filter_id",
                          "filter-cat-%zu", group)) {
      continue;
    }
    if (!checked_snprintf(buf, sizeof(buf), "render_projects.filter_control",
      "      <input class=\"filter-radio\" type=\"radio\""
      " name=\"project-filter\" id=\"%s\">\n"
      "      <label class=\"filter-label\" for=\"%s\">%s</label>\n",
      filter_id, filter_id, cat)) {
      continue;
    }
    fputs(buf, fp);
  }
  fputs("    </div>\n", fp);

  fputs("    <style>\n", fp);
  for (size_t i = 0; i < count; i++) {
    size_t group = category_group_index(projects, i);
    char filter_id[ESC_SIZE];
    if (group != i) continue;

    if (!checked_snprintf(filter_id, sizeof(filter_id),
                          "render_projects.filter_rule_id",
                          "filter-cat-%zu", group)) {
      continue;
    }
    if (!checked_snprintf(buf, sizeof(buf), "render_projects.filter_rule",
      ".filter-container:has(#%s:checked) .project-card:not(.category-%zu) {"
      " display: none; }\n",
      filter_id, group)) {
      continue;
    }
    fputs(buf, fp);
  }
  fputs("    </style>\n", fp);

  fputs("    <div class=\"projects-grid\">\n", fp);
  for (size_t i = 0; i < count; i++) {
    size_t group = category_group_index(projects, i);
    char title[ESC_SIZE], url[ESC_SIZE], period[ESC_SIZE];
    char cat[ESC_SIZE], desc[ESC_SIZE], detail_id[ESC_SIZE];

    esc(projects[i].title, title, sizeof(title));
    esc(projects[i].url, url, sizeof(url));
    esc(projects[i].period, period, sizeof(period));
    esc(projects[i].category, cat, sizeof(cat));
    esc(projects[i].description, desc, sizeof(desc));

    if (!checked_snprintf(detail_id, sizeof(detail_id),
                          "render_projects.detail_id",
                          "detail-toggle-%zu", i)) {
      continue;
    }

    if (!checked_snprintf(buf, sizeof(buf), "render_projects.card",
      "      <article class=\"project-card category-%zu\" data-category=\"%s\">\n"
      "        <h3 class=\"project-title\">\n"
      "          <a href=\"%s\" target=\"_blank\" rel=\"noopener noreferrer\">%s</a>\n"
      "        </h3>\n"
      "        <div class=\"project-meta\">\n"
      "          <span class=\"project-period\">%s</span>\n"
      "          <span class=\"project-category\">%s</span>\n"
      "        </div>\n"
      "        <input type=\"checkbox\" class=\"detail-toggle\" id=\"%s\">\n"
      "        <label class=\"detail-toggle-label\" for=\"%s\">\n"
      "          <span class=\"detail-open\">詳細を見る</span>\n"
      "          <span class=\"detail-close\">閉じる</span>\n"
      "        </label>\n"
      "        <div class=\"project-detail-wrapper\">\n"
      "          <div class=\"project-detail-inner\">\n"
      "            <p class=\"project-desc\">%s</p>\n"
      "          </div>\n"
      "        </div>\n"
      "      </article>\n",
      group, cat, url, title, period, cat, detail_id, detail_id, desc)) {
      continue;
    }
    fputs(buf, fp);
  }
  fputs("    </div>\n"
        "  </div>\n"
        "</section>\n", fp);
}

/* ---- フッター ---- */
void render_footer(FILE *fp) {
  char buf[BUF_SIZE];
  if (!checked_snprintf(buf, sizeof(buf), "render_footer",
    "<footer class=\"site-footer\">\n"
    "  <p class=\"footer-generated\">Generated by pure C</p>\n"
    "  <p class=\"footer-build\">Build: %s %s</p>\n"
    "</footer>\n",
    __DATE__, __TIME__)) {
    return;
  }
  fputs(buf, fp);
}
