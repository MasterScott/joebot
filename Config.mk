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

# uncomment to compile metamod plugin
#EXTRA_CFLAGS += -DUSE_METAMOD

# uncomment to compile for counter-strike 1.5
EXTRA_CFLAGS += -DCSTRIKE15

# uncomment to compile debug code
#EXTRA_CFLAGS += -g -D_DEBUG -DDEBUGENGINE
EXTRA_CFLAGS += -g -DDEBUGENGINE
