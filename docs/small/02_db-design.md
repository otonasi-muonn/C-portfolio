# 🗄️ データベース設計

## テーブル定義

### `users` (ユーザー情報)
| カラム名 | 型 | 制約 | 説明 |
| --- | --- | --- | --- |
| id | UUID | PK | |
| email | String | UNIQUE | |
| created_at | DateTime | | |

### `items` (メインデータ)
| カラム名 | 型 | 制約 | 説明 |
| --- | --- | --- | --- |
| id | UUID | PK | |
| user_id | UUID | FK | 作成者 |
| title | String | | |
| content | Text | | |