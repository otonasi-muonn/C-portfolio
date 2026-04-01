# 改善タスクリスト (Task List)

## 🛠 最優先 (Critical): 信頼性と SEO
- [ ] `models.h`: `SiteConfig` に OGP・ファビコン用の項目を追加する
- [ ] `render.c`: `render_head` で OGP タグと Favicon タグを出力する
- [ ] `data.c`: 自身の OGP 画像 URL とファビコンパスを設定する

## ♿ 優先 (High): アクセシビリティと表現力
- [ ] `render.c`: プロジェクトのアコーディオンに ARIA 属性を追加する
- [ ] `models.h`: `Project` 構造体に `image_url` と技術タグ配列を追加する
- [ ] `render.c`: プロジェクトカードに画像と複数の技術タグを表示する
- [ ] `models.h`: `Profile` に `avatar_url` を追加し、`render.c` で表示する

## ✨ 推奨 (Normal): 機能拡張とこだわり
- [ ] `styles/main.css`: `:has()` を活用した JS なしのテーマ切り替え（Dark/Light）を実装する
- [ ] `Makefile`: 画像アセットのコピー (`dist/images`) 処理を追加する
- [ ] `main.c` / `render.c`: 404 エラーページの生成ロジックを追加する

## 🧹 その他
- [ ] `README.md`: プロジェクトの説明をテンプレートから実情に合わせて書き換える
