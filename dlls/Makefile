# ---------------------------------------------------------------------------
# Definitions
# ---------------------------------------------------------------------------
include ../Config.mk

ifeq ($(OPT),debug)
 OBJDIR = $(OBJDIR_DEBUG)
else
 OBJDIR = $(OBJDIR_RELEASE)
endif

# build external source list
include ../NNSim/som/Config.mk
SRC_SOM := $(SRC:%.cpp=../NNSim/som/%.cpp)
OBJ_SOM := $(SRC:%.cpp=../NNSim/som/$(OBJDIR)/%.o)
include ../NNSim/Config.mk
SRC_NNSIM := $(SRC:%.cpp=../NNSim/%.cpp)
OBJ_NNSIM := $(SRC:%.cpp=../NNSim/$(OBJDIR)/%.o)

ifeq ($(DLLTYPE),std)
 DLLNAME = joebot
 ifeq ($(OPT),debug)
  OBJDIR = $(OBJDIR_DEBUG_STD)
 else
  OBJDIR = $(OBJDIR_RELEASE_STD)
 endif
else
 DLLNAME = joebot_mm
 ifeq ($(OPT),debug)
  OBJDIR = $(OBJDIR_DEBUG_MM)
 else
  OBJDIR = $(OBJDIR_RELEASE_MM)
 endif
endif

# build source list
include Config.mk
SRC_DLLS := $(SRC:%.cpp=%.cpp)
OBJ_DLLS := $(SRC:%.cpp=$(OBJDIR)/%.o)

SRC = $(SRC_DLLS) $(SRC_NNSIM) $(SRC_SOM)
OBJ = $(OBJ_DLLS) $(OBJ_NNSIM) $(OBJ_SOM)

CC = gcc
RM = rm -fv

# includes
BASECFLAGS = -I$(JOEBOTDIR)/engine -I$(JOEBOTDIR)/common\
 -I$(JOEBOTDIR)/pm_shared -I$(MMDIR)/metamod -I$(JOEBOTDIR)/NNSim/som\
 -I$(JOEBOTDIR)/NNSim -I$(JOEBOTDIR)/dlls

BASECFLAGS += $(EXTRA_CFLAGS)

ifeq ($(OPT),debug)
 CFLAGS = $(BASECFLAGS) -g
else
 CFLAGS = $(BASECFLAGS) -O2 -march=i586 -O2 -ffast-math -funroll-loops\
  -fomit-frame-pointer -fexpensive-optimizations
 ifeq ($(OS),Windows_NT)
  CFLAGS += -falign-loops=2 -falign-jumps=2 -falign-functions=2
 else
  CFLAGS += -malign-loops=2 -malign-jumps=2 -malign-functions=2
 endif
endif

ifeq ($(OS),Windows_NT)
 DLL = $(DLLNAME).dll
 LD = dllwrap
 LDFLAGS = -mwindows --add-stdcall-alias --output-def $(DLLNAME)_mingw.def
 LDLIBS = -lstdc++
else
 DLL = $(DLLNAME)_i386.so
 LD = gcc
 LDFLAGS = -fPIC -shared
 LDLIBS = -lstdc++ -ldl
endif

# ---------------------------------------------------------------------------
# Targets
# ---------------------------------------------------------------------------
ifeq ($(OPT),debug)
 EXTERNAL_BUILDS = debug_som debug_NNSim
else
 EXTERNAL_BUILDS = release_som release_NNSim
endif

all: $(OBJDIR) $(EXTERNAL_BUILDS) $(OBJDIR)/$(DLL)

debug_som:
	$(MAKE) -C ../NNSim/som OPT=debug

release_som:
	$(MAKE) -C ../NNSim/som

debug_NNSim:
	$(MAKE) -C ../NNSim OPT=debug

release_NNSim:
	$(MAKE) -C ../NNSim

debug_mm:
	$(MAKE) OPT=debug

debug_std:
	$(MAKE) OPT=debug DLLTYPE=std

release_mm:
	$(MAKE)

release_std:
	$(MAKE) DLLTYPE=std

$(OBJDIR):
	mkdir -p $(OBJDIR)
	
clean:
	-$(RM) $(OBJDIR)/*.o

spotless:
	-$(RM) -r $(OBJDIR)

rebuild: clean all

# ---------------------------------------------------------------------------
# Rules
# ---------------------------------------------------------------------------
$(OBJDIR)/$(DLL): $(OBJ)
	$(LD) $(LDFLAGS) -o $@ $(OBJ) $(LDLIBS)

$(OBJDIR)/%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@
