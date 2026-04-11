#include <stdio.h>
#include <string.h>
#include "render.h"

static int failures = 0;

static void check(const char *label, int condition) {
  if (condition) {
    printf("PASS: %s\n", label);
  } else {
    printf("FAIL: %s\n", label);
    failures++;
  }
}

static void check_escape(const char *label,
                         const char *input, const char *expected) {
  char buf[512];
  html_escape(input, buf, sizeof(buf));
  if (strcmp(buf, expected) != 0) {
    printf("FAIL: %s  got=[%s]  want=[%s]\n", label, buf, expected);
    failures++;
  } else {
    printf("PASS: %s\n", label);
  }
}

int main(void) {
  /* ---- 正常ケース ---- */
  check_escape("アンパサンド",     "A&B",        "A&amp;B");
  check_escape("小なり記号",       "<tag>",      "&lt;tag&gt;");
  check_escape("ダブルクォート",   "say \"hi\"", "say &quot;hi&quot;");
  check_escape("シングルクォート", "it's",       "it&#39;s");
  check_escape("通常文字列",       "hello",      "hello");
  check_escape("空文字列",         "",           "");
  check_escape("全種混在",         "<>&\"'",     "&lt;&gt;&amp;&quot;&#39;");

  /* ---- NULL ポインタ: クラッシュしないこと + dest[0] == '\0' ---- */
  {
    char buf[16];
    buf[0] = 'X'; /* sentinel */
    html_escape(NULL, buf, sizeof(buf));
    check("NULL-クラッシュなし",  1);  /* ここに到達 = PASS */
    check("NULL-ヌル終端",        buf[0] == '\0');
  }

  /* ---- バッファ境界: 書き込み後にヌル終端されること ---- */
  {
    char buf[5]; /* "hell" + '\0' */
    html_escape("hello world", buf, sizeof(buf));
    check("バッファ境界-ヌル終端", buf[4] == '\0');
  }

  printf("\n%s\n",
         failures == 0 ? "All tests passed." : "Some tests FAILED.");
  return failures == 0 ? 0 : 1;
}
