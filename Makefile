# ---------------------------------------------------------------------------
# Definitions
# ---------------------------------------------------------------------------
include Config.mk

ifeq ($(DLLTYPE),std)
 DLLNAME = joebot
else
 DLLNAME = joebot_mm
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

# build source list
include dlls/Config.mk
SRC_DLLS := $(SRC:%.cpp=dlls/%.cpp)
OBJ_DLLS := $(SRC:%.cpp=dlls/$(OBJDIR)/%.o)
include NNSim/Config.mk
SRC_NNSIM := $(SRC:%.cpp=NNSim/%.cpp)
OBJ_NNSIM := $(SRC:%.cpp=NNSim/$(OBJDIR)/%.o)
include NNSim/som/Config.mk
SRC_SOM := $(SRC:%.cpp=NNSim/som/%.cpp)
OBJ_SOM := $(SRC:%.cpp=NNSim/som/$(OBJDIR)/%.o)

SRC = $(SRC_DLLS) $(SRC_NNSIM) $(SRC_SOM)
OBJ = $(OBJ_DLLS) $(OBJ_NNSIM) $(OBJ_SOM)

# ---------------------------------------------------------------------------
# Targets
# ---------------------------------------------------------------------------
all: $(DLL)

mmdll:
	$(MAKE) clean
	$(MAKE)

stddll:
	$(MAKE) clean DLLTYPE=std
	$(MAKE) DLLTYPE=std

clean:
	$(MAKE) -C dlls $@
	$(MAKE) -C NNSim $@
	$(MAKE) -C NNSim/som $@
	-$(RM) $(DLL)
	-$(RM) $(DLLNAME)_mingw.def

spotless: clean
	$(MAKE) -C dlls $@
	$(MAKE) -C NNSim $@
	$(MAKE) -C NNSim/som $@
	-$(RM) *.so
	-$(RM) *.dll

rebuild: clean all

# ---------------------------------------------------------------------------
# Rules
# ---------------------------------------------------------------------------
$(DLL): $(OBJ)
	$(LD) $(LDFLAGS) -o $@ $(OBJ) $(LDLIBS)

dlls/$(OBJDIR)/%.o: dlls/%.cpp
	$(MAKE) -C dlls

NNSim/$(OBJDIR)/%.o: NNSim/%.cpp
	$(MAKE) -C NNSim

NNSim/som/$(OBJDIR)/%.o: NNSim/som/%.cpp
	$(MAKE) -C NNSim/som
