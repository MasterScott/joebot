# setup directory paths
OBJDIR_WIN32 = obj.win32
OBJDIR_LINUX = obj.linux

ifeq ($(OS),Windows_NT)
 SDKDIR = /u/netgames/install/hl/sdk/multiplayer
 JOEBOTDIR = /u/netgames/install/hl/bots/joebot/src
 MMDIR = /u/netgames/install/hl/server/metamod/src/1.17
 OBJDIR = $(OBJDIR_WIN32)
else
 SDKDIR = /home/netgames/install/hl/sdk/multiplayer
 JOEBOTDIR = /home/netgames/install/hl/bots/joebot/src
 MMDIR = /home/netgames/install/hl/server/metamod/src/1.17
 OBJDIR = $(OBJDIR_LINUX)
endif

EXTRA_CFLAGS =

ifneq ($(DLLTYPE),std)
 EXTRA_CFLAGS += -DUSE_METAMOD
endif

# uncomment to compile debug code
#EXTRA_CFLAGS += -DDEBUGENGINE
#EXTRA_CFLAGS += -DDEBUGMESSAGES
#EXTRA_CFLAGS += -D_DEBUG
