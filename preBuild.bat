@echo off
setlocal 
set file=Source\_GameScripts.h
echo // auto generated .h file - Last Build : %DATE% %TIME% > %file%
echo #pragma once >> %file%
forfiles /p Assest /s /m *.h /c "cmd /c echo #include @relpath" >> %file%