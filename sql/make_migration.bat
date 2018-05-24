
@ECHO Off
SETLOCAL ENABLEDELAYEDEXPANSION

SET UTC=YYYYMMDDHHmmss

REM Get the UTC date-time string to use
CALL :GetFormattedCurrentUTCDate UTC
ECHO "Formatted UTC Date time : %UTC%"

SET "output=%UTC%_world.sql"

ECHO DROP PROCEDURE IF EXISTS add_migration;>> migrations/%output%
ECHO delimiter ??>> migrations/%output%
ECHO CREATE PROCEDURE `add_migration`()>> migrations/%output%
ECHO BEGIN>> migrations/%output%
ECHO DECLARE v INT DEFAULT 1;>> migrations/%output%
ECHO SET v = (SELECT COUNT(*) FROM `migrations` WHERE `id`='%UTC%');>> migrations/%output%
ECHO IF v=0 THEN>> migrations/%output%
ECHO INSERT INTO `migrations` VALUES ('%UTC%');>> migrations/%output%
ECHO -- Add your query below.>> migrations/%output%
ECHO.>> migrations/%output%
ECHO.>> migrations/%output%
ECHO.>> migrations/%output%
ECHO -- End of migration.>> migrations/%output%
ECHO END IF;>> migrations/%output%
ECHO END??>> migrations/%output%
ECHO delimiter ; >> migrations/%output%
ECHO CALL add_migration();>> migrations/%output%
ECHO DROP PROCEDURE IF EXISTS add_migration;>> migrations/%output%

REM End of the script Body
:EndOfScriptBody
ECHO "Exiting ..."
GOTO EndOfScript

REM Sub routine to get the current UTC date as formatted string YYYYMMDD_HHmmss
:GetFormattedCurrentUTCDate outString
 FOR /F "tokens=* DELIMS=^=" %%a IN ('WMIC Path Win32_UTCTime Get Year^,Month^,Day^,Hour^,Minute^,Second /Value') DO (
  SET LINE=%%a
  FOR /f "tokens=1-2 delims=^=" %%i IN ("!LINE!") DO (
   IF "%%i" == "Year" ( SET year=%%j)
   IF "%%i" == "Month" ( SET month=%%j)
   IF "%%i" == "Day" ( SET day=%%j)
   IF "%%i" == "Hour" ( SET hour=%%j)
   IF "%%i" == "Minute" ( SET minute=%%j)
   IF "%%i" == "Second" ( SET second=%%j)
  )
 )
 
REM Prepend Zero to the number if less than Ten
 IF %month% LSS 10 SET month=0%month%
 IF %day% LSS 10 SET day=0%day%
 IF %hour% LSS 10 SET hour=0%hour%
 IF %minute% LSS 10 SET minute=0%minute%
 IF %second% LSS 10 SET second=0%second%

 SET %1=%year%%month%%day%%hour%%minute%%second%
REM END of :GetFormattedCurrentUTCDate
GOTO EndOfScript

:EndOfScript