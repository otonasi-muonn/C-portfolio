#include <stdio.h>
#include <stdlib.h>
#include "models.h"
#include "render.h"

#define OUTPUT_PATH "dist/index.html"

int main(void) {
  FILE *fp = fopen(OUTPUT_PATH, "w");
  if (!fp) {
    fprintf(stderr, "[ERROR] %s を開けません\n", OUTPUT_PATH);
    return EXIT_FAILURE;
  }
  fprintf(stdout, "[INFO] 出力先を開きました: %s\n", OUTPUT_PATH);

  /* HTML ドキュメント開始 */
  fputs("<!DOCTYPE html>\n", fp);
  fprintf(fp, "<html lang=\"%s\">\n", site_config.lang);

  render_head(fp, &site_config);

  fputs("<body>\n", fp);
  render_header(fp, &site_config);

  fputs("<main>\n", fp);
  render_profile(fp, &profile, social_links, social_links_count);
  render_skills(fp, &skill_section);

  /* #works セクション（キャリア + プロジェクト） */
  fputs("<section id=\"works\" class=\"section\">\n"
        "  <h2 class=\"section-title\">Works</h2>\n", fp);
  render_career(fp, career_events, career_events_count);
  render_projects(fp, projects, projects_count);
  fputs("</section>\n", fp);

  fputs("</main>\n", fp);
  render_footer(fp);

  fputs("</body>\n</html>\n", fp);

  fclose(fp);
  printf("[INFO] 生成完了: %s\n", OUTPUT_PATH);
  return EXIT_SUCCESS;
}
