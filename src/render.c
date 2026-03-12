#include <stdio.h>
#include <string.h>
#include "render.h"

#define BUF_SIZE 8192
#define ESC_SIZE 1024

/* ---- HTMLエスケープ ---- */
void html_escape(const char *src, char *dest, size_t dest_size) {
  size_t di = 0;
  if (!src || !dest || dest_size == 0) return;

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

/* ---- <head> ---- */
void render_head(FILE *fp, const SiteConfig *config) {
  char title[ESC_SIZE], desc[ESC_SIZE];
  esc(config->title, title, sizeof(title));
  esc(config->description, desc, sizeof(desc));

  char buf[BUF_SIZE];
  snprintf(buf, sizeof(buf),
    "<head>\n"
    "  <meta charset=\"UTF-8\">\n"
    "  <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n"
    "  <meta name=\"description\" content=\"%s\">\n"
    "  <title>%s</title>\n"
    "  <link rel=\"stylesheet\" href=\"%s\">\n"
    "</head>\n",
    desc, title, config->css_path);
  fputs(buf, fp);
}

/* ---- ヘッダー・ナビゲーション ---- */
void render_header(FILE *fp, const SiteConfig *config) {
  char title[ESC_SIZE];
  esc(config->title, title, sizeof(title));

  char buf[BUF_SIZE];
  snprintf(buf, sizeof(buf),
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
    title);
  fputs(buf, fp);
}

/* ---- プロフィールセクション ---- */
void render_profile(FILE *fp, const Profile *prof,
                    const SocialLink *links, size_t links_count) {
  char name[ESC_SIZE], aff[ESC_SIZE], tag[ESC_SIZE], bio[ESC_SIZE];
  esc(prof->name, name, sizeof(name));
  esc(prof->affiliation, aff, sizeof(aff));
  esc(prof->tagline, tag, sizeof(tag));
  esc(prof->bio, bio, sizeof(bio));

  char buf[BUF_SIZE];
  snprintf(buf, sizeof(buf),
    "<section id=\"about\" class=\"section\">\n"
    "  <h2 class=\"section-title\">About</h2>\n"
    "  <div class=\"profile\">\n"
    "    <h3 class=\"profile-name\">%s</h3>\n"
    "    <p class=\"profile-affiliation\">%s</p>\n"
    "    <p class=\"profile-tagline\">%s</p>\n"
    "    <p class=\"profile-bio\">%s</p>\n"
    "  </div>\n",
    name, aff, tag, bio);
  fputs(buf, fp);

  /* ソーシャルリンク */
  fputs("  <ul class=\"social-links\">\n", fp);
  for (size_t i = 0; i < links_count; i++) {
    char svc[ESC_SIZE], lbl[ESC_SIZE], url[ESC_SIZE];
    esc(links[i].service, svc, sizeof(svc));
    esc(links[i].label, lbl, sizeof(lbl));
    esc(links[i].url, url, sizeof(url));
    snprintf(buf, sizeof(buf),
      "    <li><a href=\"%s\" target=\"_blank\" rel=\"noopener noreferrer\""
      " class=\"social-link\">%s - %s</a></li>\n",
      url, svc, lbl);
    fputs(buf, fp);
  }
  fputs("  </ul>\n</section>\n", fp);
}

/* ---- スキルセクション ---- */
void render_skills(FILE *fp, const Skill *skills, size_t count) {
  fputs("<section id=\"skills\" class=\"section\">\n"
        "  <h2 class=\"section-title\">Skills</h2>\n"
        "  <div class=\"skills-grid\">\n", fp);

  char buf[BUF_SIZE];
  for (size_t i = 0; i < count; i++) {
    char name[ESC_SIZE], lvl[ESC_SIZE], desc[ESC_SIZE];
    esc(skills[i].name, name, sizeof(name));
    esc(skills[i].level, lvl, sizeof(lvl));
    esc(skills[i].description, desc, sizeof(desc));
    snprintf(buf, sizeof(buf),
      "    <div class=\"skill-card\">\n"
      "      <h3 class=\"skill-name\">%s</h3>\n"
      "      <span class=\"skill-level\">%s</span>\n"
      "      <p class=\"skill-desc\">%s</p>\n"
      "    </div>\n",
      name, lvl, desc);
    fputs(buf, fp);
  }
  fputs("  </div>\n</section>\n", fp);
}

/* ---- キャリアタイムライン ---- */
void render_career(FILE *fp, const CareerEvent *events, size_t count) {
  fputs("<section class=\"career-section\">\n"
        "  <h2 class=\"section-title\">Career</h2>\n"
        "  <div class=\"timeline\">\n", fp);

  char buf[BUF_SIZE];
  for (size_t i = 0; i < count; i++) {
    char date[ESC_SIZE], title[ESC_SIZE], desc[ESC_SIZE];
    esc(events[i].date, date, sizeof(date));
    esc(events[i].title, title, sizeof(title));
    esc(events[i].description, desc, sizeof(desc));
    snprintf(buf, sizeof(buf),
      "    <div class=\"timeline-item\">\n"
      "      <span class=\"timeline-date\">%s</span>\n"
      "      <h3 class=\"timeline-title\">%s</h3>\n"
      "      <p class=\"timeline-desc\">%s</p>\n"
      "    </div>\n",
      date, title, desc);
    fputs(buf, fp);
  }
  fputs("  </div>\n</section>\n", fp);
}

/* ---- プロジェクト一覧 ---- */
void render_projects(FILE *fp, const Project *projects, size_t count) {
  fputs("<section class=\"projects-section\">\n"
        "  <h2 class=\"section-title\">Projects</h2>\n"
        "  <div class=\"projects-grid\">\n", fp);

  char buf[BUF_SIZE];
  for (size_t i = 0; i < count; i++) {
    char title[ESC_SIZE], url[ESC_SIZE], period[ESC_SIZE];
    char cat[ESC_SIZE], desc[ESC_SIZE];
    esc(projects[i].title, title, sizeof(title));
    esc(projects[i].url, url, sizeof(url));
    esc(projects[i].period, period, sizeof(period));
    esc(projects[i].category, cat, sizeof(cat));
    esc(projects[i].description, desc, sizeof(desc));
    snprintf(buf, sizeof(buf),
      "    <article class=\"project-card\" data-category=\"%s\">\n"
      "      <h3 class=\"project-title\">\n"
      "        <a href=\"%s\" target=\"_blank\" rel=\"noopener noreferrer\">%s</a>\n"
      "      </h3>\n"
      "      <div class=\"project-meta\">\n"
      "        <span class=\"project-period\">%s</span>\n"
      "        <span class=\"project-category\">%s</span>\n"
      "      </div>\n"
      "      <p class=\"project-desc\">%s</p>\n"
      "    </article>\n",
      cat, url, title, period, cat, desc);
    fputs(buf, fp);
  }
  fputs("  </div>\n</section>\n", fp);
}

/* ---- フッター ---- */
void render_footer(FILE *fp) {
  char buf[BUF_SIZE];
  snprintf(buf, sizeof(buf),
    "<footer class=\"site-footer\">\n"
    "  <p class=\"footer-generated\">Generated by pure C</p>\n"
    "  <p class=\"footer-build\">Build: %s %s</p>\n"
    "</footer>\n",
    __DATE__, __TIME__);
  fputs(buf, fp);
}
