# OOPSpring2017Project
Game clone of HopSlide by Daniel Linssen from Ludum Dare 30, cloned by Sunny Hong &amp; John Yang for school project

# README

## 編譯須知
編譯時請選擇Release編譯，使用Debug版本編譯會因為沒有設定環境而造成錯誤。

GameSetting.h中有個遊戲視窗的名稱，根據視窗名稱不同會選擇啟動不同的Scene編譯。

```string GameSetting::WindowName = "Hop";```

編譯前請更改後面的內容。
請依序編譯兩次，第一次編譯「Hop」，第二次編譯「Slide」。

如果想要完整的處理，還需要去game.rc2改icon，但我想這你們應該不會去動吧

---

編譯後從Release資料夾中將編譯好的game.exe移出資料夾放到根目錄Engine中，並重新命名成對應的名稱。


檔案執行的條件是

1. 以exe為根找的到Assest
2. 以exe為根找的到fmod.dll

具體請參考Release.zip的擺法

---

要重新開始請刪除存檔0.sav

---

編譯或者處理上有任何問題請洽```sunnyhong.w@gmail.com```聯絡洪國翔
