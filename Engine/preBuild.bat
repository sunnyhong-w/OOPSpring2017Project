@echo off
setlocal 
set file=Source\_GameScripts.h
echo // auto generated .h file > %file%
echo #pragma once >> %file%
forfiles /p Assest /s /m *.h /c "cmd /c echo #include @relpath" >> %file%

for /r . %%g in (*.tmx) do (
cd %%~pg
cmd /c tiled --export-map %%~ng.tmx %%~ng.json)