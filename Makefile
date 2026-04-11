CC       = gcc
CFLAGS   = -Wall -Wextra -Werror -pedantic -std=c11
SRC_DIR  = src
DIST     = dist
STYLES   = styles
TARGET   = generator
TEST_DIR = tests

SRCS = $(SRC_DIR)/main.c $(SRC_DIR)/render.c $(SRC_DIR)/data.c

TEST_BINS = $(TEST_DIR)/test_html_escape $(TEST_DIR)/test_snprintf

.PHONY: all build generate clean test

all: build generate
	@echo "完了: $(DIST)/index.html を生成しました"

build: $(TARGET)

$(TARGET): $(SRCS) $(SRC_DIR)/models.h $(SRC_DIR)/render.h
	$(CC) $(CFLAGS) -o $(TARGET) $(SRCS)

generate: $(TARGET)
	@mkdir -p $(DIST)
	./$(TARGET)
	@cp $(STYLES)/main.css $(DIST)/main.css

# --- テスト ---
test: $(TEST_BINS) all
	@for t in $(TEST_BINS); do echo "--- $$t ---"; ./$$t || exit 1; done
	@test -f $(DIST)/index.html   || (echo "FAIL: $(DIST)/index.html が見つかりません" && exit 1)
	@test -s $(DIST)/index.html   || (echo "FAIL: $(DIST)/index.html が空です" && exit 1)
	@grep -q '<html'   $(DIST)/index.html || (echo "FAIL: <html> タグなし" && exit 1)
	@! grep -q '<script' $(DIST)/index.html || (echo "FAIL: <script> タグが存在します" && exit 1)
	@echo "PASS: 全テスト完了"

# test_html_escape: render.c を別翻訳単位としてリンク
$(TEST_DIR)/test_html_escape: $(TEST_DIR)/test_html_escape.c $(SRC_DIR)/render.c $(SRC_DIR)/data.c
	$(CC) $(CFLAGS) -o $@ $^ -I$(SRC_DIR)

# test_snprintf: render.c をインクルードするため別翻訳単位として渡さない
$(TEST_DIR)/test_snprintf: $(TEST_DIR)/test_snprintf.c $(SRC_DIR)/data.c
	$(CC) $(CFLAGS) -o $@ $^ -I$(SRC_DIR)

clean:
	@rm -f $(TARGET)
	@rm -rf $(DIST)
	@rm -f $(TEST_BINS)
