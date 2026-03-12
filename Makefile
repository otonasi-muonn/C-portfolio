CC      = gcc
CFLAGS  = -Wall -Wextra -Werror -pedantic -std=c11
SRC_DIR = src
DIST    = dist
STYLES  = styles
TARGET  = generator

SRCS = $(SRC_DIR)/main.c $(SRC_DIR)/render.c $(SRC_DIR)/data.c

.PHONY: all build generate clean

all: build generate
	@echo "完了: $(DIST)/index.html を生成しました"

build: $(TARGET)

$(TARGET): $(SRCS) $(SRC_DIR)/models.h $(SRC_DIR)/render.h
	$(CC) $(CFLAGS) -o $(TARGET) $(SRCS)

generate: $(TARGET)
	@mkdir -p $(DIST)
	./$(TARGET)
	@cp $(STYLES)/main.css $(DIST)/main.css

clean:
	@rm -f $(TARGET)
	@rm -rf $(DIST)
