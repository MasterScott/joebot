# setup directory paths
OBJDIR_WIN32 = obj.win32
OBJDIR_LINUX = obj.linux

ifeq ($(OS),Windows_NT)
 SDKDIR = /u/netgames/install/hl/sdk/multiplayer
 JOEBOTDIR = /u/netgames/install/hl/bots/joebot
 MMDIR = /u/netgames/install/hl/server/metamod-1.17
 OBJDIR = $(OBJDIR_WIN32)
else
 SDKDIR = /home/netgames/install/hl/sdk/multiplayer/
 JOEBOTDIR = /home/netgames/install/hl/bots/joebot
 MMDIR = /home/netgames/install/hl/server/metamod-1.17
 OBJDIR = $(OBJDIR_LINUX)
endif

EXTRA_CFLAGS =

ifneq ($(DLLTYPE),std)
 EXTRA_CFLAGS += -DUSE_METAMOD
endif

ifeq ($(CS),1_5)
 EXTRA_CFLAGS += -DCSTRIKE15
endif

# uncomment to compile debug code
#EXTRA_CFLAGS += -g -D_DEBUG -DDEBUGENGINE
