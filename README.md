# Win05 期末專題報告

**組別** : 第5組  
**組員** :  
- 41243101 伍翊瑄  
- 41243103 林采儀  
- 41243108 鄭又瑄  

---

## 一、主題與動機

ppt檔案:
https://nfuedu-my.sharepoint.com/:p:/g/personal/41243101_nfu_edu_tw/ERTxmQvtgTJAtlYT8ThSQRQB6dkK3pG_ytWH8VbiJSuYjg?e=RcJRR2

• 主題: 基於 Qt 的俄羅斯方塊遊戲——「晃方世界」。​

• 動機: 高中時，經常和同學一起玩 TETRIS 的線上俄羅斯方塊 PK，但總是最早被淘汰，心有不甘。為了提升自己的遊戲技巧，決定開發一款俄羅斯方塊遊戲，這樣可以調整遊戲內部數據（例如掉落速度），並進行針對性的練習。同時這也是一個學習如何應用 Qt 框架進行遊戲設計與開發的好機會。

---

## 二、系統需求

• 硬體需求:開發環境(筆電)。

• 軟體需求:Qt開發環境版本(6.7.2)。

---

## 三、系統設計與實作

❑功能規劃

•核心功能: 俄羅斯方塊遊戲基本玩法。

•衍生功能: 背景音樂、難度調節等。

❑Qt 框架應用

•遊戲主視窗和界面佈局: 使用 QWidget 或 QMainWindow 類來創建。

•遊戲面板繪製:使用QPainter 類，它允許在窗口上繪製方塊、分數等動態內容。

•音效控制:通過 QCheckBox 和 QSoundEffect ，管理音樂開關。

❑信號與槽機制應用

•按鈕點擊: 如開始、暫停、開關音樂等操作，當玩家點擊按鈕時，會發送 clicked 信號，
並觸發對應的槽函數。

•鍵盤事件:透過keyPressEvent 來捕獲鍵盤按鍵，控制方塊的移動和旋轉。

❑遊戲邏輯設計

•方塊定義: generateNewPiece()函式，七種基本方塊形狀( O, I, L, J, Z, S, T )，每個形狀由
四個Qpoint 定義其相對位置。

•方塊生成: 使用std::rand()隨機生成一個索引（範圍為0到6），以此來實現隨機生成七
種形狀。

•下一個方塊的預覽: 使用QPixmap 創建預覽區域，調用generateNewPiece() 繪製下一
個方塊的圖像，並透過mainWindow的方法更新顯示。

•方塊消除: 檢查當行是否填滿的變數 isFull ，若變為 true，則消除該行並將上方的方塊下
移。

•方塊移動(← ↓ →、Space): 使用"左、下、右方向鍵"和"空白鍵"，分別觸發左移函式 moveLeft() 
和下移函式 dropStep() 以及右移函式 moveRight() 、快速下移函式 moveToBottom()。

•方塊旋轉(↑): 使用"上方向鍵"觸發旋轉函數 rotate()。通過矩陣的轉置和反轉實現 90 度旋轉，
且避免超界與碰撞。(先轉置，將其行列對調;後反轉，將每列的元素左右顛倒)

---

❑遊戲邏輯設計

•得分和等級: 隨等級提高，遊戲加速並引入視窗晃動和浮動等視覺效果，提升遊戲難度。

#### 第二級(加入視窗左右晃動)
![第二級](https://github.com/user-attachments/assets/5ee6f870-94e9-451b-98eb-588a298f854d)

#### 第三級(加入視窗上下晃動)
![第三級](https://github.com/user-attachments/assets/eee86f0d-bf3f-4471-9b4f-adec1c569348)

#### 第四級(加入當前方塊左右移動)​
![第四級](https://github.com/user-attachments/assets/bd309f88-7c05-47b6-b4d2-11ec0da78af6)

• 暫停機制的觸發條件

• 暫停時，應停止所有計時器、動畫或遊戲邏輯
的更新。

• 暫停狀態的保存與恢復

• 在恢復時，確保遊戲邏輯和動畫能從中斷點繼
續執行。

• 所有暫停前的動作（例如方塊的下落、計分
等）應保存以便恢復。

#### 暫停繼續功能
![暫停繼續功能](https://github.com/user-attachments/assets/306f9e9b-29a3-48d3-8873-6b55ac0248a0)

• 遊戲中止條件與結束畫面: 無法合法放置新方塊則觸發遊戲結束，並跳出結束視窗。

#### 結束畫面
![結束畫面](https://github.com/user-attachments/assets/b37b8b60-08df-4066-aed2-d413654c5dd1)


---

## 四、測試與結果

❑測試案例

•生成的方塊形狀是否隨機，並覆蓋所有可能的形狀。

•測試方塊生成、旋轉和消除及音效同步功能。

•檢查視窗晃動是否影響遊戲操作。

❑測試結果

•方塊的隨機生成均符合預期，且每種形狀的出現概率平均分布。

•核心功能運行穩定且同步。

•功能表現穩定，音效開關能準確被控制，視窗晃動在多次測試中未出現卡頓現象。


---

## 五、結論

❑優勢

•簡單直觀的操作方式，易於玩家上手。

•遊戲的難度設計循序漸進，既適合初學者，也能滿足進階玩家的挑戰需求。

•核心功能表現穩定且同步。

❑學習收穫

•團隊合作能力。

•熟悉了界面設計和信號與槽機制的運用。

•提升了 Qt 框架實踐能力。

---
## 六、附錄

### 影片


[專題影片](https://github.com/user-attachments/assets/5c253320-b5ca-40b8-a032-5ed820f789d1)

1. **使用者體驗**：設計直觀、流暢的操作介面，確保不同年齡層（如老人或小孩）都能輕鬆上手。
2. **程式實作**：雖然程式實作方式與參考資料的介面有些相似，但僅在部分功能實現邏輯上有所參考。整體程式實作部分與參考資料的程式有明顯的區別。
---

### 參考資料

- [QtTetris 參考資料](https://github.com/tashaxing/QtTetris)
- [參考資料的試玩影片(存在一些bug)](https://github.com/user-attachments/assets/6d8c9369-c1b9-4377-80ae-f57b5db92372)
