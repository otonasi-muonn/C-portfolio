# 📁 ディレクトリ構成図

---

# 0️⃣ 設計前提

| 項目      | 内容                                 |
| ------- | ---------------------------------- |
| リポジトリ構成 | Monorepo / Polyrepo                |
| アーキテクチャ | Layered / Clean Architecture / DDD |
| デプロイ単位  | 単一サービス / マイクロサービス                  |
| 言語      | 任意（TypeScript / Go / Python 等）     |
| MVP方針   | P0に必要なディレクトリのみ                     |

---

# 1️⃣ 全体構成（Monorepo想定）

```id="o8d9si"
root/
├── apps/              # 実行可能アプリ
│   ├── web/
│   ├── api/
│   └── admin/
├── packages/          # 共有パッケージ
│   ├── ui/
│   ├── domain/
│   ├── config/
│   └── utils/
├── infra/             # IaC / Terraform / Docker
├── scripts/           # 補助スクリプト
├── docs/              # 設計書
└── README.md
```

---

# 2️⃣ フロントエンド構成テンプレ

```id="tq93md"
apps/web/
├── src/
│   ├── app/           # ルーティング層
│   ├── features/      # 機能単位モジュール
│   ├── components/    # 共通UI
│   ├── hooks/
│   ├── lib/           # APIクライアント等
│   ├── stores/        # 状態管理
│   └── types/
├── public/
└── tests/
```

---

## Featureベース構成（推奨）

```id="t6lm52"
features/
├── auth/
│   ├── components/
│   ├── api.ts
│   ├── hooks.ts
│   └── types.ts
├── entity/
│   ├── components/
│   ├── api.ts
│   ├── hooks.ts
│   └── types.ts
```

---

# 3️⃣ バックエンド構成テンプレ（Clean Architecture）

```id="9wh13c"
apps/api/
├── cmd/                # エントリポイント
├── internal/
│   ├── domain/         # エンティティ・ビジネスルール
│   ├── usecase/        # アプリケーションロジック
│   ├── repository/     # DB抽象
│   ├── handler/        # HTTP層
│   ├── middleware/
│   └── config/
├── migrations/
└── tests/
```

---

# 4️⃣ DDDベース構成テンプレ

```id="kl2m91"
src/
├── modules/
│   ├── user/
│   │   ├── domain/
│   │   ├── application/
│   │   ├── infrastructure/
│   │   └── presentation/
│   ├── organization/
│   └── core/
```

---

# 5️⃣ マイクロサービス構成

```id="0b5zvx"
services/
├── auth-service/
├── core-service/
├── notification-service/
└── gateway/
```

---

# 6️⃣ インフラ構成

```id="v9k0mz"
infra/
├── terraform/
│   ├── modules/
│   └── environments/
│       ├── dev/
│       ├── staging/
│       └── prod/
├── docker/
└── ci/
```

---

# 7️⃣ ドキュメント構成

```id="az1k93"
docs/
├── 01_feature-list.md
├── 02_db-design.md
├── 03_screen-flow.md
├── 04_permission-design.md
├── 05_api-spec.md
└── 06_directory.md
```

---

# 8️⃣ テスト構成テンプレ

```id="c32po1"
tests/
├── unit/
├── integration/
├── e2e/
└── fixtures/
```

---

# 9️⃣ ベクトルDB / AI機能がある場合

```id="q91dte"
packages/
├── embeddings/
│   ├── generator.ts
│   ├── repository.ts
│   └── vector-client.ts
├── rag/
│   ├── retriever.ts
│   └── prompt-builder.ts
```

---

# 🔟 状態管理分離パターン（FE）

```id="8t1k4d"
stores/
├── auth.store.ts
├── entity.store.ts
└── ui.store.ts
```

---

# 11️⃣ API設計分離パターン

```id="nb29df"
api/
├── client.ts
├── endpoints/
│   ├── auth.ts
│   ├── entities.ts
│   └── users.ts
```
