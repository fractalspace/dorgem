@echo off

REM -- First use the Resource.hm as Popups include file
copy Resource.hm HtmlHelp\Popups.h > nul


REM -- Make HTML Help for Dorgem
if exist "HtmlHelp\Dorgem.chm" del "HtmlHelp\Dorgem.chm"
start /wait c:\progra~1\htmlhe~1\hhc HtmlHelp\Dorgem.hhp > nul
if not exist "HtmlHelp\Dorgem.chm" goto Error


REM -- Copy HtmlHelp for Dorgem
if exist Debug\nul copy "HtmlHelp\Dorgem.chm" Debug > nul
if exist Release\nul copy "HtmlHelp\Dorgem.chm" Release > nul
goto done


:Error
echo HtmlHelp\Dorgem.hhp(1) : error: Problem encountered creating help file

:done
