# Intelligent Motion Recorder

# Implementation

溝通架構: STM32l476 nucleo + IKS01A2 → STM32 disco → PC server
1. STM32l476 nucleo + IKS01A2
   IKS01A2有各種sensor, 我們用到的是LSM6DSL加速度計跟LPS22HB壓力計。IKS01A2負責感測器數據採集，nucleo負責處理數據、計    算分類使用者狀態
2. STM32l475 disco
   使用UART4的port接收nucleo的訊息，將訊息解碼
3. PC server
   disco開發板用socket傳送完整字串給PC server。PC server 會記錄下收到訊息的時間以及訊息內容，儲存至一個log file, 並將    結果display到使用者介面的螢幕上(using SDL library)。
   
   
# Algorithm flow
1. Day mode
   Update motion detection, with 8 motions in total (stationary, standing, sitting, lying, walking,          running, jogging, biking)      
2. Night mode
   If not sleeping, detect motion (same with day mode)
   If the user is asleep, detect turn over
   
 # How to reproduce
 1. clone all the repo
 2. go to SDL official website to download SDL library
 3. compile display.cpp (g++ display.cpp -lSDL2 -o display)
 4. In terminal, execute ./display, and click start to start listening

# demo video link
https://drive.google.com/open?id=1mKJFM953Cb-rhk2k8j25r6vZ5kqS9A22
