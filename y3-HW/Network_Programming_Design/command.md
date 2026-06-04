# HW1 指令

## 內部指令
- `quit` - 結束 shell
- `setenv <var> <value>` - 設定環境變數
- `printenv [var]` - 印出環境變數值

## 外部指令
- `ls`, `cat`, `cp`, `echo`, `grep`, `wc`, `number` 等

## 特殊語法
- `|` - 普通管道，例：`ls | grep bin`
- `|N` - 編號管道，例：`ls |2`（N ≤ 128，| 後無空格）

## 錯誤訊息
- `Unknown command: [指令].`

---

# HW2 指令

## 外部指令（聊天室）
- `who` - 列出所有線上使用者
- `tell <user_id> <message>` - 傳送私人訊息
- `yell <message>` - 廣播訊息給所有使用者
- `name <new_name>` - 更改顯示名稱

## 伺服器
- `./server` - 啟動伺服器（埠 12345）
- `telnet 127.0.0.1 12345` - 客戶端連線

## 輸出格式
- `who` → `1	alice`
- `tell` 接收 → `<1 told you>: hello`
- `yell` 接收 → `<1 yelled>: hello`
- `name` 成功 → `name change accept!`
- `name` 失敗 → `User new name already exists!`

---

# HW3 指令

## MySQL 資料庫管理

- 清空資料：

```sql
USE chatdb;
TRUNCATE mails;
TRUNCATE group_members;
TRUNCATE `groups`;
```

- 確認資料表:

```sql
DELETE FROM users WHERE username IN ('alice','bob');
SELECT * FROM users;
SELECT * FROM mails;
SELECT * FROM groups;
SELECT * FROM group_members;
```

# HW3 指令與輸出格式

## 外部指令（郵件）
- `listMail` - 列出所有郵件
- `mailto <user> <message>` - 寄送郵件
- `mailto <user> < ls` - 重導向輸入寄信
- `delMail <mail_id>` - 刪除郵件

## 外部指令（群組）
- `createGroup <group_name>` - 建立群組
- `delGroup <group_name>` - 刪除群組（僅群主）
- `addTo <group> <user1> <user2> ...` - 邀請成員（僅群主）
- `leaveGroup <group_name>` - 離開群組
- `listGroup` - 列出所屬群組
- `remove <group> <user1> <user2> ...` - 移除成員（僅群主）
- `gyell <group> <message>` - 群組廣播

## 輸出格式（郵件）
- `listMail` → `ID:1 From:alice Content:hello Time:...`
- 無郵件 → `empty !`
- `mailto` 成功 → `send accept!`
- `mailto` 失敗 → `User not found !`
- `delMail` 成功 → `delete accept!`
- `delMail` 失敗 → `Mail id unexist !`

## 輸出格式（群組）
- `createGroup` 成功 → `Group created!`
- `createGroup` 失敗 → `Group already exist !`
- `addTo` 成功 → `bob add success !`
- `addTo` 使用者不存在 → `carol not found !`
- `addTo` 已在群組 → `bob already in group !`
- `addTo` 權限不足 → `You don't have permission !`
- `gyell` 群組不存在 → `Group not found !`
- `gyell` 接收 → `<family:alice>: hello`
- `leaveGroup` 成功 → `Leave success !`
- `leaveGroup` 失敗 → `Leave fault !`
- `listGroup` 無群組 → `Empty !`
- `remove` 成功 → `bob remove success !`
- `remove` 不在群組 → `bob is not in group !`
- `remove` 使用者不存在 → `carol not found !`
- `delGroup` 成功 → `Group delete success !`
- `delGroup` 群組不存在 → `Group not found !`
- `delGroup` 權限不足 → `You don't have permission !`

## 登入/登出
- `Login: <username>` / `Password: <password>` - 登入流程
- 註冊流程：`1` → 輸入帳號密碼
- `logout` - 登出

## 伺服器
- `./server` - 啟動伺服器（需 MySQL）
- `telnet 127.0.0.1 12345` - 客戶端連線