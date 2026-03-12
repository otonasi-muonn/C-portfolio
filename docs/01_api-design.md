# 🔌 API設計

## 1. 自作エンドポイント一覧
アプリが提供する独自のAPIエンドポイントです。
| メソッド | エンドポイント | 役割 | 備考 |
| --- | --- | --- | --- |
| `POST` | `/api/auth/login` | ログイン処理 | JWTを返す |
| `GET` | `/api/items` | データ一覧の取得 | |
| `POST` | `/api/items` | データの新規作成 | |

## 2. 外部サービス連携 (3rd Party APIs)
アプリが利用する外部のAPIやサービスです。
| サービス名 | 用途 | 必要な環境変数キー |
| --- | --- | --- |
| OpenAI API | テキスト生成 | `OPENAI_API_KEY` |
| Supabase | 認証・DB | `NEXT_PUBLIC_SUPABASE_URL`, `SUPABASE_SERVICE_ROLE_KEY` |

## 3. Webhook設計
外部からアプリに飛んでくるWebhookの定義です。
* **エンドポイント**: `/api/webhooks/stripe`
* **受け取るイベント**: `checkout.session.completed`
* **処理内容**: ユーザーのステータスを「有料会員」に更新する