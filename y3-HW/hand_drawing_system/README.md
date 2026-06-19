# 多人空中繪圖系統 (Multi-User Air Drawing System)

這是一個基於 **MediaPipe** 手部偵測、**IoU + 外觀特徵追蹤** 與 **PySide6** GUI 的即時協作繪圖系統。  
使用者只需一個普通網路攝影機（720p），即可讓 **2~3 人** 同時在共享畫布上繪圖，無需觸控螢幕或特殊硬體。

---

## 系統特色

- **即時手部偵測**：利用 MediaPipe 提取 21 個手部特徵點，支援最多 3 隻手。
- **多手追蹤與重識別**：結合 IoU 與外觀嵌入（MobileNetV3）進行短暫關聯與長時遮擋恢復。
- **捏合控制筆觸**：以拇指與食指的距離（歸一化到手部尺寸）判斷筆尖起落。
- **多使用者顏色分離**：自動分配紅、綠、藍三色，分別對應不同使用者。
- **1€ 濾波器**：平滑指尖抖動，繪圖流暢自然。
- **簡潔 GUI**：基於 PySide6，顯示畫布、使用者數量與 FPS。

---

## 系統架構與模組說明
```txt
hand_drawing_system/
├── config/
│ └── default.yaml # 所有可調參數（攝影機、偵測、追蹤、繪圖）
├── src/
│ ├── main.py # 程式入口，載入設定並啟動 GUI
│ ├── core/
│ │ ├── system.py # 主控類別，整合各模組，處理每幀影像
│ │ ├── detector.py # 以 MediaPipe 偵測手部並回傳邊界框與特徵點
│ │ ├── tracker.py # 多手追蹤（IoU + 外觀特徵比對）
│ │ ├── reid_model.py # 外觀編碼器（MobileNetV3）
│ │ ├── user_manager.py # 使用者綁定與顏色分配（最多 3 人）
│ │ └── draw_engine.py # 畫布管理、濾波器、筆觸繪製
│ ├── gui/
│ │ └── main_window.py # PySide6 主視窗（顯示畫布、按鈕、狀態列）
│ └── utils/
│ └── filter.py # 1€ 濾波器實作
├── requirements.txt # Python 套件依賴
└── README.md # 本文件
```
### 核心流程

1. **攝影機擷取** → 2. **MediaPipe 手部偵測**（邊界框 + 21 關鍵點）
2. **多手追蹤**（關聯上一幀與目前檢測，並利用外觀特徵恢復遺失身分）
3. **使用者綁定**（將追蹤 ID 對應至使用者 ID 與顏色）
4. **捏合判斷**（計算拇指與食指距離 / 手部尺寸，低於閾值則筆尖按下）
5. **繪圖**（經 1€ 濾波器平滑後，在畫布上繪製對應顏色的筆觸）
6. **GUI 顯示**（疊加在攝影機畫面上，同時顯示 FPS 與筆狀態）

---

## 安裝與執行指南

### 前置需求

- **作業系統**：macOS / Linux / Windows (建議使用 bash 終端機)
- **Python**：3.8 以上
- **攝影機**：內建或外接 USB 攝影機（720p 以上）

### 步驟 1：複製專案

```bash
cd hand_drawing_system
```

### 步驟 2：建立虛擬環境並安裝依賴

```bash
# 建立虛擬環境
python3 -m venv venv

# 啟用虛擬環境（macOS / Linux）
source venv/bin/activate
# Windows 請使用：venv\Scripts\activate

# 升級 pip 並安裝套件
pip install --upgrade pip
pip install -r requirements.txt
```

### 步驟 3：執行程式

```bash
python src/main.py
```
