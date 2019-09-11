SHELL=/bin/sh

# To compile project as debug version, set DEBUG=1
# To compile project with clang, set CLANG=1
TARGET = kombig
EXE = $(TARGET)

# Output and intermediate (for object files) directory, shared and static lib
OBJSDIR = ./obj
OBJDIRTREE = $(OBJSDIR)


# Set of used tools (compiler may be selected during execution)
ifdef CLANG
	export CC = clang
else
	export CC = gcc
endif
LD = $(CC)
MKDIR = mkdir -p
RM = rm -f


# List of every important source or generated files
CXXSRC = $(wildcard *.cpp)
HEADERS = $(wildcard *.h)
OBJS = $(CXXSRC:%.cpp=$(OBJSDIR)/%.o)
PRECOMP_HEADERS = $(OBJSDIR)/Globals.h.gch
RESOURCE_FILES = $(OBJSDIR)/$(TARGET).res
LDLIBS = -lstdc++


# All compiler flags (general and compiler specific too)
CCFLAGS = -std=c++03 -Wall -W -Wextra -Wuninitialized -pedantic \
	-Wno-unknown-pragmas
	# -fpic

ifeq ($(CC),gcc)
	CCFLAGS += -Wunused-parameter -Wmissing-declarations -Wredundant-decls \
		-Wshadow -Wlogical-op -Wswitch-default -Wswitch-enum \
		-Wstrict-overflow=5 -Wdate-time -Winit-self -Wwrite-strings \
		-Wnarrowing -Wstack-protector -Wunsafe-loop-optimizations \
		-Wno-aggressive-loop-optimizations -Wdisabled-optimization \
		-Wsuggest-attribute=noreturn -Wsuggest-attribute=format \
		-Wformat=2 -Wfloat-equal -Wcast-qual -Wno-multichar -Wsynth \
		-Wcast-align -Wconversion -Wold-style-cast -Wctor-dtor-privacy \
		-Wdelete-non-virtual-dtor -Woverloaded-virtual -Wsign-promo \
		-Wconditionally-supported -Wuseless-cast -Wc++11-compat \
		-Weffc++ -Wsuggest-attribute=const -Wsuggest-attribute=pure \
		-Wno-variadic-macros
		# -Waggregate-return
endif
ifeq ($(CC),clang)
	CCFLAGS += -Wno-disabled-macro-expansion -Wno-c++11-long-long \
		-Wno-four-char-constants -Wno-variadic-macros -Dinline=__inline
endif


# Set of flags that depend on compilation configuration
LDFLAGS = -fvisibility=hidden -fvisibility-inlines-hidden -fno-rtti
ifdef DEBUG
	CCFLAGS += -D_DEBUG
	LDFLAGS += -g3 -ftrapv -fstack-protector-all
else
	CCFLAGS += -DNDEBUG
	LDFLAGS += -Ofast -fno-stack-protector
	LDLIBS += -lssp
endif


# Specific optimization flags for linker according to compiler and configuration
ifeq ($(CC),gcc)
ifdef DEBUG
	LDFLAGS += -fstack-check -fbounds-check
		# -fvtable-verify=std -fvtv-debug -flto-odr-type-merging
else
	LDFLAGS += -flto -fivopts -funsafe-loop-optimizations -fipa-pta \
		-fno-branch-count-reg -fgcse-sm \
		-fgcse-las -fmodulo-sched -fmodulo-sched-allow-regmoves \
		-fuse-linker-plugin
		# -flto -floop-parallelize-all -fwhole-program
endif
endif


# Definition of commands
COMPILE = $(CC) -x c++ $(CCFLAGS) $(LDFLAGS) -c $< -o $@
LINK = $(LD) $(CCFLAGS) $(LDFLAGS) $^ $(LDLIBS) -o $@
CREATE_OBJDIRTREE = $(MKDIR) $@
CLEAN = $(RM) $(OBJS) $(OBJSDIR)/*.h.gch *.plist


.PHONY: release clean clang-analyze cppcheck

all: objdirtree $(EXE)

$(EXE): $(OBJS)
	$(LINK)

$(OBJSDIR)/%.o: %.cpp
	$(COMPILE)

$(OBJS): $(HEADERS)

objdirtree: $(OBJDIRTREE)

$(OBJDIRTREE):
	$(CREATE_OBJDIRTREE)

clang-analyze: $(CXXSRC)
	clang $^ --analyze -std=c++03 -Wall -W -Wextra -Wuninitialized -pedantic

cppcheck: $(CXXSRC)
	cppcheck $^ -x c++ --std=c++03 --enable=all -f --inconclusive

release: objdirtree $(EXE)
	$(CLEAN)

clean:
	$(CLEAN) $(EXE)
