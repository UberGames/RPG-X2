default: so
so: build_so

# compiler to use for building shared objects
CC = gcc

# determine arch and platform
ARCH=$(shell uname -m | sed -e s/i.86/i386/)
PLATFORM=$(shell uname|sed -e s/_.*//|tr '[:upper:]' '[:lower:]')

# cflags for the compiler
ifeq ($(PLATFORM), mingw32)
SOCFLAGS = $(CFLAGS)
else
SOCFLAGS = $(CFLAGS) -fPIC
endif

# set extension
ifeq ($(PLATFORM), mingw32)
EXT=dll
ARCH=x86
else
EXT=so
endif

# cgame objects
OBJ = \
  fx_transporter.o \
  fx_tetrion.o \
  fx_stasis.o \
  fx_scavenger.o \
  fx_quantum.o \
  fx_phaser.o \
  fx_misc.o \
  fx_lib.o \
  fx_item.o \
  fx_imod.o \
  fx_grenade.o \
  fx_dreadnought.o \
  fx_compression.o \
  fx_borg.o \
  cg_weapons.o \
  cg_view.o \
  cg_snapshot.o \
  cg_servercmds.o \
  cg_screenfx.o \
  cg_scoreboard.o \
  cg_predict.o \
  cg_playerstate.o \
  cg_players.o \
  cg_marks.o \
  cg_main.o \
  cg_localents.o \
  cg_info.o \
  cg_event.o \
  cg_env.o \
  cg_ents.o \
  cg_effects.o \
  cg_drawtools.o \
  cg_draw.o \
  cg_consolecmds.o 

# depency objects from game
OBJDEP = \
  q_shared.o \
  q_math.o \
  bg_misc.o \
  bg_pmove.o \
  bg_slidemove.o 
  
 # object for syscalls to the engine
SOOBJ = \
  cg_syscalls.o 

# do cc for shared library
DO_SOCC = $(CC) $(SOCFLAGS) -Wall -o $@ -c $<

build_so: DO_CC=$(DO_SOCC)

# cgame
cg_consolecmds.o : cg_consolecmds.c; $(DO_CC)
cg_draw.o : cg_draw.c; $(DO_CC)
cg_drawtools.o : cg_drawtools.c; $(DO_CC)
cg_effects.o : cg_effects.c; $(DO_CC)
cg_ents.o : cg_ents.c; $(DO_CC)
cg_env.o : cg_env.c; $(DO_CC)
cg_event.o : cg_event.c; $(DO_CC)
cg_info.o : cg_info.c; $(DO_CC)
cg_localents.o : cg_localents.c; $(DO_CC)
cg_main.o : cg_main.c; $(DO_CC)
cg_marks.o : cg_marks.c; $(DO_CC)
cg_players.o : cg_players.c; $(DO_CC)
cg_playerstate.o : cg_playerstate.c; $(DO_CC)
cg_predict.o : cg_predict.c; $(DO_CC)
cg_scoreboard.o : cg_scoreboard.c; $(DO_CC)
cg_screenfx.o : cg_screenfx.c; $(DO_CC)
cg_servercmds.o : cg_servercmds.c; $(DO_CC)
cg_snapshot.o : cg_snapshot.c; $(DO_CC)
cg_view.o : cg_view.c; $(DO_CC)
cg_weapons.o : cg_weapons.c; $(DO_CC)
fx_borg.o : fx_borg.c; $(DO_CC)
fx_compression.o : fx_compression.c; $(DO_CC)
fx_dreadnought.o : fx_dreadnought.c; $(DO_CC)
fx_grenade.o : fx_grenade.c; $(DO_CC)
fx_imod.o : fx_imod.c; $(DO_CC)
fx_item.o : fx_item.c; $(DO_CC)
fx_lib.o : fx_lib.c; $(DO_CC)
fx_misc.o : fx_misc.c; $(DO_CC)
fx_phaser.o : fx_phaser.c; $(DO_CC)
fx_quantum.o : fx_quantum.c; $(DO_CC)
fx_scavenger.o : fx_scavenger.c; $(DO_CC)
fx_stasis.o : fx_stasis.c; $(DO_CC)
fx_tetrion.o : fx_tetrion.c; $(DO_CC)
fx_transporter.o : fx_transporter.c; $(DO_CC)

# dependencies from game
q_shared.o: ../game/q_shared.c; $(DO_CC)
q_math.o: ../game/q_math.c; $(DO_CC)
bg_misc.o:  ../game/bg_misc.c; $(DO_CC)
bg_pmove.o: ../game/bg_pmove.c; $(DO_CC)
bg_slidemove.o: ../game/bg_slidemove.c; $(DO_CC)

# cgame syscalls
cg_syscalls.o : cg_syscalls.c; $(DO_CC)

build_so: $(OBJDEP) $(OBJ) $(SOOBJ)
ifeq ($(PLATFORM), mingw32)
	$(CC) -shared -Wl,--export-all-symbols,-soname,cgame$(ARCH).$(EXT) -o cgame$(ARCH).$(EXT) $(OBJ) $(OBJDEP) $(SOOBJ)
else
	$(CC) -shared -Wl,--export-dynamic,-soname,cgame$(ARCH).$(EXT) -o cgame$(ARCH).$(EXT) $(OBJ) $(OBJDEP) $(SOOBJ)
endif
	
clean:
	rm -f *.o *.$(EXT)
