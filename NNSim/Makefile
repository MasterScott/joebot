# ---------------------------------------------------------------------------
# Definitions
# ---------------------------------------------------------------------------
include ../Config.mk

CC = gcc
RM = rm -fv

# includes
BASECFLAGS = -I$(JOEBOTDIR)/engine -I$(JOEBOTDIR)/common\
 -I$(JOEBOTDIR)/pm_shared -I$(MMDIR)/metamod -I$(JOEBOTDIR)/NNSim/som\
 -I$(JOEBOTDIR)/NNSim -I$(JOEBOTDIR)/dlls

include Config.mk

BASECFLAGS += $(EXTRA_CFLAGS)

CFLAGS = $(BASECFLAGS) -w -O2 -march=i586 -O2 -ffast-math -funroll-loops\
 -fomit-frame-pointer -fexpensive-optimizations -malign-loops=2\
 -malign-jumps=2 -malign-functions=2
CFLAGS = $(BASECFLAGS) -w

OBJ := $(SRC:%.cpp=$(OBJDIR)/%.o)

# ---------------------------------------------------------------------------
# Targets
# ---------------------------------------------------------------------------
all: $(OBJDIR) $(OBJ)

$(OBJDIR):
	mkdir $(OBJDIR)
	
clean:
	-$(RM) $(OBJDIR)/*.o

rebuild: clean all

# ---------------------------------------------------------------------------
# Rules
# ---------------------------------------------------------------------------
$(OBJDIR)/%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@
