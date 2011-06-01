@echo off
echo.
cd game
pause
if exist game.bat call game
pause
cd ..\cgame
if exist cgame.bat call cgame
pause
cd ..\ui
if exist ui.bat call ui
pause
cd ..
echo Finished.
