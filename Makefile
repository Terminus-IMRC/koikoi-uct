all:

TARGET := main
SRCS := main.c randrange.c player.c score.c fudaset.c playout.c uct.c dbg.c
ALLDEPS = $(MAKEFILE_LIST_SANS_DEPS)
CFLAGS_LOCAL := -Wall -Wextra -pipe -g -O0
LDLIBS_LOCAL := -lm

CC := gcc
RM := rm -f
WC := wc -l -c
CTAGS := ctags

DEPS := $(patsubst %.c, %.c.d, $(filter %.c, $(SRCS)))
OBJS := $(patsubst %.c, %.c.o, $(filter %.c, $(SRCS)))

VALID_MAKECMDGOALS := all $(TARGET) $(DEPS) $(OBJS) clean line tags
NONEED_DEP_MAKECMDGOALS := clean line tags

EXTRA_MAKECMDGOALS := $(filter-out $(VALID_MAKECMDGOALS), $(MAKECMDGOALS))
ifneq '$(EXTRA_MAKECMDGOALS)' ''
 $(error No rule to make target `$(firstword $(EXTRA_MAKECMDGOALS))')
else
 ifeq '$(filter-out $(NONEED_DEP_MAKECMDGOALS), $(MAKECMDGOALS))' '$(MAKECMDGOALS)'
  sinclude $(DEPS)
 endif
endif

MAKEFILE_LIST_SANS_DEPS := $(filter-out %.c.d, $(MAKEFILE_LIST))

LINK.o = $(CC) $(CFLAGS) $(CFLAGS_LOCAL) $(EXTRACFLAGS) $(CPPFLAGS) $(CPPFLAGS_LOCAL) $(EXTRACPPFLAGS) $(TARGET_ARCH)
COMPILE.c = $(CC) $(CFLAGS) $(CFLAGS_LOCAL) $(EXTRACFLAGS) $(CPPFLAGS) $(CPPFLAGS_LOCAL) $(EXTRACPPFLAGS) $(TARGET_ARCH) -c
MKDEP.c = $(CC) $(CFLAGS) $(CFLAGS_LOCAL) $(EXTRACFLAGS) $(CPPFLAGS) $(CPPFLAGS_LOCAL) $(EXTRACPPFLAGS) $(TARGET_ARCH) -M -MP -MT $<.o -MF $@

all: $(TARGET)

$(TARGET): $(OBJS) $(ALLDEPS)
	$(LINK.o) $(OUTPUT_OPTION) $(OBJS) $(LOADLIBES) $(LOADLIBES_LOCAL) $(LDLIBS) $(LDLIBS_LOCAL)

%.c.o: %.c %.c.d $(ALLDEPS)
	$(COMPILE.c) $(OUTPUT_OPTION) $<

%.c.d: %.c $(ALLDEPS)
	$(MKDEP.c) $<

.PHONY: clean
clean:
	$(RM) $(TARGET)
	$(RM) $(OBJS)
	$(RM) $(DEPS)
	$(RM) tags

.PHONY: line
line:
	$(WC) $(SRCS) $(MAKEFILE_LIST_SANS_DEPS) test-*.c

.PHONY: tags
tags:
	$(CTAGS) -R .
