set LIBRARY=
set INCLUDE=

@echo off
if not exist vm\nul mkdir vm
cd vm
if not exist ..\..\lcc\bin\lcc.exe goto nolcc
echo Creating the GAME ASM files...
echo. 
..\..\..\bin_nt\lcc -DQ3_VM -DQAGAME -DBUILDING_QVM -S -Wf-target=bytecode -Wf-g -I..\cgame -I..\game -I..\ui %1 ../g_main.c
@if errorlevel 1 goto quit

..\..\..\bin_nt\lcc -DQ3_VM -DQAGAME -DBUILDING_QVM -S -Wf-target=bytecode -Wf-g -I..\..\cgame -I..\..\game -I..\..\ui %1 ../g_syscalls.c
@if errorlevel 1 goto quit

..\..\..\bin_nt\lcc -DQ3_VM -DQAGAME -DBUILDING_QVM -S -Wf-target=bytecode -Wf-g -I..\..\cgame -I..\..\game -I..\..\ui %1 ../bg_misc.c
@if errorlevel 1 goto quit
..\..\..\bin_nt\lcc -DQ3_VM -DQAGAME -DBUILDING_QVM -S -Wf-target=bytecode -Wf-g -I..\..\cgame -I..\..\game -I..\..\ui %1 ../bg_lib.c
@if errorlevel 1 goto quit
..\..\..\bin_nt\lcc -DQ3_VM -DQAGAME -DBUILDING_QVM -S -Wf-target=bytecode -Wf-g -I..\..\cgame -I..\..\game -I..\..\ui %1 ../bg_pmove.c
@if errorlevel 1 goto quit
..\..\..\bin_nt\lcc -DQ3_VM -DQAGAME -DBUILDING_QVM -S -Wf-target=bytecode -Wf-g -I..\..\cgame -I..\..\game -I..\..\ui %1 ../bg_slidemove.c
@if errorlevel 1 goto quit
..\..\..\bin_nt\lcc -DQ3_VM -DQAGAME -DBUILDING_QVM -S -Wf-target=bytecode -Wf-g -I..\..\cgame -I..\..\game -I..\..\ui %1 ../q_math.c
@if errorlevel 1 goto quit
..\..\..\bin_nt\lcc -DQ3_VM -DQAGAME -DBUILDING_QVM -S -Wf-target=bytecode -Wf-g -I..\..\cgame -I..\..\game -I..\..\ui %1 ../q_shared.c
@if errorlevel 1 goto quit

..\..\..\bin_nt\lcc -DQ3_VM -DQAGAME -DBUILDING_QVM -S -Wf-target=bytecode -Wf-g -I..\..\cgame -I..\..\game -I..\..\ui %1 ../ai_dmnet.c
@if errorlevel 1 goto quit
..\..\..\bin_nt\lcc -DQ3_VM -DQAGAME -DBUILDING_QVM -S -Wf-target=bytecode -Wf-g -I..\..\cgame -I..\..\game -I..\..\ui %1 ../ai_dmq3.c
@if errorlevel 1 goto quit
..\..\..\bin_nt\lcc -DQ3_VM -DQAGAME -DBUILDING_QVM -S -Wf-target=bytecode -Wf-g -I..\..\cgame -I..\..\game -I..\..\ui %1 ../ai_main.c
@if errorlevel 1 goto quit
..\..\..\bin_nt\lcc -DQ3_VM -DQAGAME -DBUILDING_QVM -S -Wf-target=bytecode -Wf-g -I..\..\cgame -I..\..\game -I..\..\ui %1 ../ai_chat.c
@if errorlevel 1 goto quit
..\..\..\bin_nt\lcc -DQ3_VM -DQAGAME -DBUILDING_QVM -S -Wf-target=bytecode -Wf-g -I..\..\cgame -I..\..\game -I..\..\ui %1 ../ai_cmd.c
@if errorlevel 1 goto quit
..\..\..\bin_nt\lcc -DQ3_VM -DQAGAME -DBUILDING_QVM -S -Wf-target=bytecode -Wf-g -I..\..\cgame -I..\..\game -I..\..\ui %1 ../ai_team.c
@if errorlevel 1 goto quit

..\..\..\bin_nt\lcc -DQ3_VM -DQAGAME -DBUILDING_QVM -S -Wf-target=bytecode -Wf-g -I..\..\cgame -I..\..\game -I..\..\ui %1 ../g_active.c
@if errorlevel 1 goto quit
..\..\..\bin_nt\lcc -DQ3_VM -DQAGAME -DBUILDING_QVM -S -Wf-target=bytecode -Wf-g -I..\..\cgame -I..\..\game -I..\..\ui %1 ../g_arenas.c
@if errorlevel 1 goto quit
..\..\..\bin_nt\lcc -DQ3_VM -DQAGAME -DBUILDING_QVM -S -Wf-target=bytecode -Wf-g -I..\..\cgame -I..\..\game -I..\..\ui %1 ../g_bot.c
@if errorlevel 1 goto quit
..\..\..\bin_nt\lcc -DQ3_VM -DQAGAME -DBUILDING_QVM -S -Wf-target=bytecode -Wf-g -I..\..\cgame -I..\..\game -I..\..\ui %1 ../g_client.c
@if errorlevel 1 goto quit
..\..\..\bin_nt\lcc -DQ3_VM -DQAGAME -DBUILDING_QVM -S -Wf-target=bytecode -Wf-g -I..\..\cgame -I..\..\game -I..\..\ui %1 ../g_combat.c
@if errorlevel 1 goto quit
..\..\..\bin_nt\lcc -DQ3_VM -DQAGAME -DBUILDING_QVM -S -Wf-target=bytecode -Wf-g -I..\..\cgame -I..\..\game -I..\..\ui %1 ../g_fx.c
@if errorlevel 1 goto quit
..\..\..\bin_nt\lcc -DQ3_VM -DQAGAME -DBUILDING_QVM -S -Wf-target=bytecode -Wf-g -I..\..\cgame -I..\..\game -I..\..\ui %1 ../g_items.c
@if errorlevel 1 goto quit
..\..\..\bin_nt\lcc -DQ3_VM -DQAGAME -DBUILDING_QVM -S -Wf-target=bytecode -Wf-g -I..\..\cgame -I..\..\game -I..\..\ui %1 ../g_log.c
@if errorlevel 1 goto quit
..\..\..\bin_nt\lcc -DQ3_VM -DQAGAME -DBUILDING_QVM -S -Wf-target=bytecode -Wf-g -I..\..\cgame -I..\..\game -I..\..\ui %1 ../g_mem.c
@if errorlevel 1 goto quit
..\..\..\bin_nt\lcc -DQ3_VM -DQAGAME -DBUILDING_QVM -S -Wf-target=bytecode -Wf-g -I..\..\cgame -I..\..\game -I..\..\ui %1 ../g_misc.c
@if errorlevel 1 goto quit
..\..\..\bin_nt\lcc -DQ3_VM -DQAGAME -DBUILDING_QVM -S -Wf-target=bytecode -Wf-g -I..\..\cgame -I..\..\game -I..\..\ui %1 ../g_missile.c
@if errorlevel 1 goto quit
..\..\..\bin_nt\lcc -DQ3_VM -DQAGAME -DBUILDING_QVM -S -Wf-target=bytecode -Wf-g -I..\..\cgame -I..\..\game -I..\..\ui %1 ../g_mover.c
@if errorlevel 1 goto quit
..\..\..\bin_nt\lcc -DQ3_VM -DQAGAME -DBUILDING_QVM -S -Wf-target=bytecode -Wf-g -I..\..\cgame -I..\..\game -I..\..\ui %1 ../g_session.c
@if errorlevel 1 goto quit
..\..\..\bin_nt\lcc -DQ3_VM -DQAGAME -DBUILDING_QVM -S -Wf-target=bytecode -Wf-g -I..\..\cgame -I..\..\game -I..\..\ui %1 ../g_spawn.c
@if errorlevel 1 goto quit
..\..\..\bin_nt\lcc -DQ3_VM -DQAGAME -DBUILDING_QVM -S -Wf-target=bytecode -Wf-g -I..\..\cgame -I..\..\game -I..\..\ui %1 ../g_svcmds.c
@if errorlevel 1 goto quit
..\..\..\bin_nt\lcc -DQ3_VM -DQAGAME -DBUILDING_QVM -S -Wf-target=bytecode -Wf-g -I..\..\cgame -I..\..\game -I..\..\ui %1 ../g_target.c
@if errorlevel 1 goto quit
..\..\..\bin_nt\lcc -DQ3_VM -DQAGAME -DBUILDING_QVM -S -Wf-target=bytecode -Wf-g -I..\..\cgame -I..\..\game -I..\..\ui %1 ../g_team.c
@if errorlevel 1 goto quit
..\..\..\bin_nt\lcc -DQ3_VM -DQAGAME -DBUILDING_QVM -S -Wf-target=bytecode -Wf-g -I..\..\cgame -I..\..\game -I..\..\ui %1 ../g_trigger.c
@if errorlevel 1 goto quit
..\..\..\bin_nt\lcc -DQ3_VM -DQAGAME -DBUILDING_QVM -S -Wf-target=bytecode -Wf-g -I..\..\cgame -I..\..\game -I..\..\ui %1 ../g_utils.c
@if errorlevel 1 goto quit
..\..\..\bin_nt\lcc -DQ3_VM -DQAGAME -DBUILDING_QVM -S -Wf-target=bytecode -Wf-g -I..\..\cgame -I..\..\game -I..\..\ui %1 ../g_weapon.c
@if errorlevel 1 goto quit
..\..\..\bin_nt\lcc -DQ3_VM -DQAGAME -DBUILDING_QVM -S -Wf-target=bytecode -Wf-g -I..\..\cgame -I..\..\game -I..\..\ui %1 ../g_usable.c
@if errorlevel 1 goto quit
..\..\..\bin_nt\lcc -DQ3_VM -DQAGAME -DBUILDING_QVM -S -Wf-target=bytecode -Wf-g -I..\..\cgame -I..\..\game -I..\..\ui %1 ../g_breakable.c
@if errorlevel 1 goto quit
..\..\..\bin_nt\lcc -DQ3_VM -DQAGAME -DBUILDING_QVM -S -Wf-target=bytecode -Wf-g -I..\..\cgame -I..\..\game -I..\..\ui %1 ../g_turrets.c
@if errorlevel 1 goto quit
..\..\..\bin_nt\lcc -DQ3_VM -DQAGAME -DBUILDING_QVM -S -Wf-target=bytecode -Wf-g -I..\..\cgame -I..\..\game -I..\..\ui %1 ../g_ui.c
@if errorlevel 1 goto quit
..\..\..\bin_nt\lcc -DQ3_VM -DQAGAME -DBUILDING_QVM -S -Wf-target=bytecode -Wf-g -I..\..\cgame -I..\..\game -I..\..\ui %1 ../g_cmds.c
@if errorlevel 1 goto quit
..\..\..\bin_nt\lcc -DQ3_VM -DQAGAME -DBUILDING_QVM -S -Wf-target=bytecode -Wf-g -I..\..\cgame -I..\..\game -I..\..\ui %1 ../bg_oums.c
@if errorlevel 1 goto quit

if not exist ..\..\..\bin_nt\q3asm.exe goto noq3asm
echo Creating the GAME.QVM file...
echo.
..\..\..\bin_nt\q3asm -f ../game
goto done
:noq3asm
echo ERROR:  Q3ASM.EXE was not found in the ..\..\..\bin_nt subdirectory.
pause
goto done
:nolcc
echo ERROR:  LCC.EXE was not found in the ..\..\..\bin_nt subdirectory.
pause
goto done
:quit
echo Error building the GAME ASM files with LCC.EXE!
pause
:done
echo.
cd ..
