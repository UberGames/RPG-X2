STEF Game Source. Copyright (C) 1999-2001 Raven Softare

NOTE: The source MUST BE INSTALLED into the \stvoy\ directory.  (q3asm will not work otherwise, and you cannot make the vms!)

The Game Source is broken out into 3 areas.

game - governs the game, runs on the server side.
cgame - governs the client side of the game, runs on the client side.
ui - handles the ui on the client side.

Making a quick mod is very straightforward. This document assumes Microsoft Visual C++ v6.xx. It covers making a slight mod to the game source, recompiling for debugging and rebuilding the vm's for distribution.

Slow Stasis Projectiles (Rockets) - TestMod
----------------------
1. Open up the StefGame.dsw in Microsoft Visual C++.
2. Set the "game" project as the active project.
3. Open the "g_local.h" file and change the GAMEVERSION define from "baseef" to "TestMod"
4. Save "g_local.h"
5. Open the "g_missile.c" file.
6. Go to line 557 and change the 900 to 300. The old line reads:
	
	VectorScale( dir, 900, bolt->s.pos.trDelta );

The new line should read

	VectorScale( dir, 300, bolt->s.pos.trDelta );

7. Save "g_missile.c"
8. Perform a Build All command and it should build a DLL for the game. 

At this point you have two options. You can run the debugger, choosing 'stvoyHM.exe' as the executable host which will load your DLL for debugging or you can build the vm's for distribution. When you release mods, you will want to build new vm's.

Building the vm's requires two things.

1. The files contained in the bin_nt path must be available for execution ( lcc.exe and q3asm.exe )
2. There must be environment variables set for proper lib and include paths. Microsoft Visual C++ installs a batch file that does this for you called "VCVARS32.bat"

To build the sample vm for the slow rocket test, do the following:

1. Open a DOS window.
2. Make sure lcc.exe and q3asm.exe are available on the path.
3. Run VCVARS32.bat
4. Go to your mods game directory and run the 'game.bat' file.

This ultimately produces a 'qagame.qvm' in the \baseef\vm\ path. 

5. Make a "TestMod" path under your STEF directory. This will be a sibling to 'baseef'
6. Move 'qagame.qvm' to "\YourSTEFPath\TestMod\vm\"
7. Run STEF with the following command line "stvoyhm +set fs_game TestMod"
8. "TestMod" should be the referenced game and you should be able to catch up with and outrun your rockets.


Each of the areas contain a batch file "game.bat", "cgame.bat", and "ui.bat" which will build the appropriate vm files.

-----------------------------------------------
Using Visual Studio to Build and Debug your Mod

1. Create a directory to hold your Mod in the STEF directory
       \YourSTEFPath\TestMod\
2. In VC, open Project->Settings
3. On the Debug tab Category General: 
   -set the "Executable for debug session" to your stvoyHM.exe
   -Set the "Working directory" to the same directory where stvoyHM.exe is
   -Set the "Program arguments" to:  +set r_fullscreen 0 +set viewlog 1 +set fs_game TestMod
   (note, if you don't have the "set fs_game TestMod", you will need to create
    a pak0.pk3 file for the mod to show up in the menu - see below)
4. On the link tab, change path of the "Output file name" to your TestMod directory

Do this for each of projects you are making modifications to (cgame, game, ui)

-------------------------------------------------
Making my Mod show up on the Mod list in the game

You need to have a pak0.pk3 file in your mod directory
before it will show up on the in-game menu. Create a "description.txt"
file with some information about your game mod. Use WinZIP to create 
the pak0.pk3 file. Add the "description.txt" file without compression
to the pak0.pk3 file and store it in your TestMod directory.  You can 
now put the .dll files in this directory, or create a "vm" directory 
and place the .qvm files in the vm directory.


	
