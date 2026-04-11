/* test_snprintf.c
 * checked_snprintf は static 関数のため render.c を直接インクルードして
 * その翻訳単位にアクセスする。
 * コンパイル時に render.c を別翻訳単位として渡してはならない（二重定義になる）。
 */
#include "../src/render.c"

static int failures = 0;

static void check(const char *label, int condition) {
  if (condition) {
    printf("PASS: %s\n", label);
  } else {
    printf("FAIL: %s\n", label);
    failures++;
  }
}

int main(void) {
  char buf[16];

  /* ---- 正常な書き込み ---- */
  buf[0] = '\0';
  check("正常書き込み-戻り値",
        checked_snprintf(buf, sizeof(buf), "test", "hello") == 1);
  check("正常書き込み-内容",
        strcmp(buf, "hello") == 0);

  /* ---- buf = NULL ---- */
  check("buf=NULL", checked_snprintf(NULL, 16, "test", "hi") == 0);

  /* ---- buf_size = 0 ---- */
  check("buf_size=0", checked_snprintf(buf, 0, "test", "hi") == 0);

  /* ---- バッファ超過: 切り詰め + ヌル終端 ---- */
  {
    char small[5];
    int ret = checked_snprintf(small, sizeof(small),
                               "overflow_test", "1234567890");
    check("バッファ超過-戻り値",   ret == 0);
    check("バッファ超過-ヌル終端", small[sizeof(small) - 1] == '\0');
  }

  printf("\n%s\n",
         failures == 0 ? "All tests passed." : "Some tests FAILED.");
  return failures == 0 ? 0 : 1;
}
