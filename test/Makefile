NAME			= tester
SO				= ../malloc/bin/libmalloc.so

SRCEXT      	= c
SRCDIR      	= src
OBJEXT      	= o
HEADEREXT		= h
HEADERDIR		= include
BUILDDIR    	= obj
LIBDIR			= lib
BINDIR			= bin
SRC				= $(wildcard $(SRCDIR)/*.$(SRCEXT))
OBJS			= $(foreach src,$(SRC),$(BUILDDIR)/$(notdir $(src:.$(SRCEXT)=.$(OBJEXT))))
HEADERS 		= $(shell find $(HEADERDIR) -name '*.$(HEADEREXT)')

FLAGS			= -Wall -Wextra -Werror

ifdef PRODUCTION
FLAGS			+= -O3 -D PRODUCTION
else
FLAGS 			+= -g3 -Ofast
endif

LIBS			= -L../malloc/bin -lmalloc -lm

UNAME_S			= $(shell uname -s)
ifeq ($(UNAME_S), Linux)
NCPU 			= $(shell nproc --all)
else ifeq ($(UNAME_S), Darwin)
NCPU 			= $(shell sysctl -n hw.ncpu)
else
$(error Windows is, and should never be supported)
endif
MAKEFLAGS		+="j $(NCPU) $(NAME)"

VPATH = $(shell find $(SRCDIR) -type d | tr '\n' ':' | sed -E 's/(.*):/\1/')

.SUFFIXES:

##---------------------------------------------------------------------
## BUILD RULES
##---------------------------------------------------------------------

all: $(BINDIR) $(NAME)
	$(MAKE) -C ../malloc
	@cp ../malloc/bin/libmalloc.so bin/libmalloc.so
	@echo Build complete
	@echo

$(NAME): $(SO) $(BUILDDIR) $(OBJS) $(HEADERS)
	$(CC) -o $@ $(OBJS) $(FLAGS) $(LIBS)

$(BUILDDIR)/%.o: $(SRCDIR)/%.$(SRCEXT) $(HEADERS)
	$(CC) -I$(HEADERDIR) -c -o $(BUILDDIR)/$(notdir $@) $< $(LIBS)

$(BUILDDIR):
	mkdir -p $(BUILDDIR)

test:
	@$(MAKE) all 2>&1 | grep -v "make\[2\]\:" | grep -v "make\[1\]\:"
	LD_LIBRARY_PATH=$$PWD/../malloc/bin ./$(NAME)

test-valgrind:
	@$(MAKE) all 2>&1 | grep -v "make\[2\]\:" | grep -v "make\[1\]\:"
	LD_LIBRARY_PATH=$$PWD/../malloc/bin valgrind --leak-check=full --show-leak-kinds=definite,indirect,possible --track-origins=yes ./$(NAME)

start-valgrind-container:
	docker run -it --rm -v $$PWD/..:/malloc nloomans/codam

clean:
	make -C ../malloc clean
ifneq ($(BUILDDIR),.)
	/bin/rm -rf $(BUILDDIR)/
endif
	rm -f $(NAME)
	rm -rf $(BINDIR)

re:
	$(MAKE) clean
	rm -f $(NAME)
	$(MAKE)

$(BINDIR):
	mkdir -p $(BINDIR)
$(SO):
	$(MAKE) -C ../malloc
# Dirty hack below
	cp ../malloc/bin/libmalloc.so $(BINDIR)
