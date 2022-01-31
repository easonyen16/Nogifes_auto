@echo off
chcp 65001
set /p StartNum=   輸入開始序號:
set /p EndNum=  輸入結束序號:
set /a cnt=%StartNum%
:loop
echo %cnt%
::下載影片
if %cnt% LSS 1000 (aria2c "https://v2static.nogifes.jp/resource/Movie/Reward/reward_movie_00%cnt%.usme")^
else (aria2c "https://v2static.nogifes.jp/resource/Movie/Reward/reward_movie_0%cnt%.usme")
::影片解碼
if %cnt% LSS 1000 (crid.exe -a C5510101 -b 0013F11B -o "auto" "reward_movie_00%cnt%.usme")^
else (crid.exe -a C5510101 -b 0013F11B -o "auto" "reward_movie_0%cnt%.usme")
::影片合併
if %cnt% LSS 1000 (ffmpeg -i "%~dp0\auto\reward_movie_00%cnt%.264_med.m2v" -i "%~dp0\auto\reward_movie_00%cnt%.avi.wav"  -ab 320k  -y -c:v copy -c:a:0 libmp3lame "%~dp0\auto\reward_movie_00%cnt%.mp4")^
else (ffmpeg -i "%~dp0\auto\reward_movie_0%cnt%.264_med.m2v" -i "%~dp0\auto\reward_movie_0%cnt%.avi.wav"  -ab 320k  -y -c:v copy -c:a:0 libmp3lame "%~dp0\auto\reward_movie_0%cnt%.mp4")
::清除部必要檔案
del %~dp0\auto\*.m2v
del %~dp0\auto\*.wav
del %~dp0\auto\*.ini
del *.usme
set /a cnt=%cnt%+1
if %cnt% lss %EndNum%+1 goto loop
pause.
