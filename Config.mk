# setup directory paths
OBJDIR_DEBUG_WIN32 = debug/win32
OBJDIR_RELEASE_WIN32 = release/win32
OBJDIR_DEBUG_LINUX = debug/linux
OBJDIR_RELEASE_LINUX = release/linux

ifeq ($(OS),Windows_NT)
 SDKDIR = /u/netgames/install/hl/sdk/multiplayer
 JOEBOTDIR = /u/netgames/install/hl/bots/joebot/src
 MMDIR = /u/netgames/install/hl/server/metamod/src/1.17
 OBJDIR_DEBUG = $(OBJDIR_DEBUG_WIN32)
 OBJDIR_RELEASE = $(OBJDIR_RELEASE_WIN32)
else
 SDKDIR = /home/netgames/install/hl/sdk/multiplayer
 JOEBOTDIR = /home/netgames/install/hl/bots/joebot/src
 MMDIR = /home/netgames/install/hl/server/metamod/src/1.17
 OBJDIR_DEBUG = $(OBJDIR_DEBUG_LINUX)
 OBJDIR_RELEASE = $(OBJDIR_RELEASE_LINUX)
endif

OBJDIR_DEBUG_STD = $(OBJDIR_DEBUG)/std
OBJDIR_DEBUG_MM = $(OBJDIR_DEBUG)/mm
OBJDIR_RELEASE_STD = $(OBJDIR_RELEASE)/std
OBJDIR_RELEASE_MM = $(OBJDIR_RELEASE)/mm

EXTRA_CFLAGS =

ifneq ($(DLLTYPE),std)
 EXTRA_CFLAGS += -DUSE_METAMOD
endif

# uncomment to compile debug code
#EXTRA_CFLAGS += -DDEBUGENGINE
#EXTRA_CFLAGS += -DDEBUGMESSAGES
#EXTRA_CFLAGS += -D_DEBUG
