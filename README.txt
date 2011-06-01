The Star Trek Voyager: Elite Force codebase project

This project is meant to be a stable codebase with obvious bugs that
Ravensoft left in there removed and a few new features added.

This project was originally started because my modification of the Quake3
engine to run EF turned out to be a bit problematic. The SnapVector macro
yields slightly different results in the virtual machine interpreter of the
newer quake3 compared to the older quake3 eliteforce is based off.
As a result, you cannot jump as high as you could if you set com_maxfps to
some magic values like 74, 85, 125 etc..
To get the same movement as with the original EliteForce in these new engine
releases the multiplayer VM code must be modified to emulate the old
behaviour.
So my goal is that every active player and every server will install these
modifications to allow for smooth movement on both, servers using the new
engine release and for good motion prediction on the client side. I realize
it may well be that this never happens as getting people to install new pak
files would be a major undertaking. I thought, if I attempt to do so in the
first place, I can as well try to get in a few improvements as long as they
don't change gameplay. I am not trying to insert new weapons, models,
gametypes etc...

A few todos:
 - Ignoring text messages from players that are a nuisance
 - Unlagged code (can be switched on/off with a cvar)

If you have made changes that do *not* change gameplay as mentioned above
and you think it really improves EliteForce, feel free to send the patches
to: arny@ats.s.bawue.de
If your code is reasonably clean, I will definitely add it to the
repository!

Now for the using of this code:
I have left all non-C files from Ravensoft intact. This means, you should
probably be able to build this release under windows like you build the
original EF source released by raven.
For users on unixoid environments, I have included a few Makefiles that make
the job of building easier. The Makefiles are designed to work under Linux
so if you use another OS you may need to edit the three Makefiles included
in the game, cgame and ui directories to make this build properly.

There are two different types of builds, namely the shared library and the
building of QVMs.
Building the shared objects only requires typing in "make so"
in the main directory where this README resides. If you have a working gcc
installation, this is all you need to do. The rest is being handled by the
Makefiles and in the end you should have three resulting files in the baseef
directory.
Building QVMs is easy, too. You need a few special bins, though, namely
q3lcc, q3rcc, q3cpp and q3asm. They can be found when building Quake3 from
icculus.org: http://icculus.org/quake3/
Get the source code, compile it (you may want to only build the dedicated
server if you have no OpenGL support... consult the README in their project
on how this works). When compilation is done, the four required binaries can
be found in code/tools/. Copy the files to the bin/ directory that is in the
same dir with this README.
Now just do: "make qvm" in this dir and all QVMs should be in baseef/vm/.

Careful when compiling both, QVMs and shared objects. gcc and the q3lcc
compiler produce incompatible output.
Make sure to run "make clean" before you change from QVM to shared objects
and vice versa.

And last but not least: I already installed a mechanism to produce a pak
file that only works if you have zip installed.
make pak
will result in all three QVMs being packed into pak4.pk3 in baseef :)
Rename it as you see fit.

 - Thilo Schulz, 16.03.2006
The Star Trek Voyager: Elite Force codebase project

This project is meant to be a stable codebase with obvious bugs that
Ravensoft left in there removed and a few new features added.

This project was originally started because my modification of the Quake3
engine to run EF turned out to be a bit problematic. The SnapVector macro
yields slightly different results in the virtual machine interpreter of the
newer quake3 compared to the older quake3 eliteforce is based off.
As a result, you cannot jump as high as you could if you set com_maxfps to
some magic values like 74, 85, 125 etc..
To get the same movement as with the original EliteForce in these new engine
releases the multiplayer VM code must be modified to emulate the old
behaviour.
So my goal is that every active player and every server will install these
modifications to allow for smooth movement on both, servers using the new
engine release and for good motion prediction on the client side. I realize
it may well be that this never happens as getting people to install new pak
files would be a major undertaking. I thought, if I attempt to do so in the
first place, I can as well try to get in a few improvements as long as they
don't change gameplay. I am not trying to insert new weapons, models,
gametypes etc...

A few todos:
 - Ignoring text messages from players that are a nuisance
 - Unlagged code (can be switched on/off with a cvar)

If you have made changes that do *not* change gameplay as mentioned above
and you think it really improves EliteForce, feel free to send the patches
to: arny@ats.s.bawue.de
If your code is reasonably clean, I will definitely add it to the
repository!

Now for the using of this code:
I have left all non-C files from Ravensoft intact. This means, you should
probably be able to build this release under windows like you build the
original EF source released by raven.
For users on unixoid environments, I have included a few Makefiles that make
the job of building easier. The Makefiles are designed to work under Linux
so if you use another OS you may need to edit the three Makefiles included
in the game, cgame and ui directories to make this build properly.

There are two different types of builds, namely the shared library and the
building of QVMs.
Building the shared objects only requires typing in "make so"
in the main directory where this README resides. If you have a working gcc
installation, this is all you need to do. The rest is being handled by the
Makefiles and in the end you should have three resulting files in the baseef
directory.
Building QVMs is easy, too. You need a few special bins, though, namely
q3lcc, q3rcc, q3cpp and q3asm. They can be found when building Quake3 from
icculus.org: http://icculus.org/quake3/
Get the source code, compile it (you may want to only build the dedicated
server if you have no OpenGL support... consult the README in their project
on how this works). When compilation is done, the four required binaries can
be found in code/tools/. Copy the files to the bin/ directory that is in the
same dir with this README.
Now just do: "make qvm" in this dir and all QVMs should be in baseef/vm/.

Careful when compiling both, QVMs and shared objects. gcc and the q3lcc
compiler produce incompatible output.
Make sure to run "make clean" before you change from QVM to shared objects
and vice versa.

And last but not least: I already installed a mechanism to produce a pak
file that only works if you have zip installed.
make pak
will result in all three QVMs being packed into pak4.pk3 in baseef :)
Rename it as you see fit.

 - Thilo Schulz, 16.03.2006
