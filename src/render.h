#ifndef RENDER_H
#define RENDER_H

#include <stdio.h>
#include "models.h"

/* HTMLエスケープ: src を安全な文字列に変換し dest へ書き込む */
void html_escape(const char *src, char *dest, size_t dest_size);

/* セクション別レンダリング関数 */
void render_head(FILE *fp, const SiteConfig *config);
void render_header(FILE *fp, const SiteConfig *config);
void render_profile(FILE *fp, const Profile *prof,
                    const SocialLink *links, size_t links_count);
void render_skills(FILE *fp, const Skill *skills, size_t count);
void render_career(FILE *fp, const CareerEvent *events, size_t count);
void render_projects(FILE *fp, const Project *projects, size_t count);
void render_footer(FILE *fp);

#endif /* RENDER_H */
