CC = gcc
CFLAGS = -g -Wall -O3
LIB = -I../include -L../bin -lcclap
SRC = $(wildcard *.c)
EXEC = $(SRC:.c=) # Remove the extension ".c"
TARGETS = build

all: $(TARGETS)

build:
	@if [ ! -e ../bin/libcclap.a ]; then \
		echo "libcclap.a not detected. Compiling it."; \
		make -C ../; \
	fi;
	@$(foreach f, $(EXEC), \
		echo "Compiling $f.c"; \
		$(CC) $(CFLAGS) $(f).c -o $(f) $(LIB); \
	)

clean_tmp:
	@rm -vf *~

clean: clean_tmp

distclean mrproper veryclean:
	@$(foreach f, $(EXEC), \
		rm -vf $f; \
	)
