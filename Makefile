CC = gcc
CFLAGS = -g -fPIC -Wall -Wextra -O3
SRCDIR = src
INCDIR = include
OBJDIR = obj
BINDIR = bin
EXDIR = samples
TESTDIR = tests
TESTFILE = $(TESTDIR)/cclap_test
INCLUDES = -I$(INCDIR)
LIBFLAGS = -cruvs
SRC=$(wildcard $(SRCDIR)/*.c)
OBJ=$(SRC:$(SRCDIR)/%.c=$(OBJDIR)/%.o)
TARGETS = lib test buildsamples

all: $(TARGETS)

lib: $(BINDIR)/libcclap.a

$(BINDIR)/libcclap.a: $(OBJ)
	@[ -d $(BINDIR) ] || mkdir $(BINDIR)
	@echo "Creating $@"
	@ar $(LIBFLAGS) $@ $^

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@[ -d $(OBJDIR) ] || mkdir $(OBJDIR)
	@echo "Compiling $<"
	@$(CC) $(CFLAGS) $(INCLUDES) -c -o $@ $<

build-tests: lib
	@echo "Compiling $(TESTFILE).c"
	@$(CC) $(CFLAGS) $(INCLUDES) $(TESTFILE).c -o $(TESTFILE) -L$(BINDIR) -lcclap -lcunit

test: build-tests
	@echo "Running $(TESTFILE)"
	@$(TESTFILE)

buildsamples:
	@echo "Building samples"
	@make -C $(EXDIR)

clean: clean_tmp
	@rm -vrf $(OBJDIR)
	@make clean -C $(EXDIR)

clean_tmp:
	@rm -vf *~ $(SRCDIR)/*~ $(INCDIR)/*~ $(INCDIR)/cclap/*~ $(TESTDIR)/*~
	@make clean_tmp -C $(EXDIR)

clean_tests:
	@rm -vf $(TESTFILE)

distclean mrproper veryclean: clean clean_tests
	@rm -vrf $(BINDIR)
	@make distclean -C $(EXDIR)
