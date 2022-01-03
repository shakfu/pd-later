INCLUDE = -I./include
# LIBS=$(wildcard lib/*)
MACOS_VER=10.15


cflags += -ftree-vectorize -mmacosx-version-min=$(MACOS_VER) $(INCLUDE)/qdbm
ldflags += -lm lib/libqdbm.a

lib.name = dbm

class.sources := dbm.c

datafiles = help-dbm.pd

include Makefile.pdlibbuilder


render:
	@scripts/render.py && make


hash:
	@gcc -o ./scripts/hash scripts/hash-functions.c


test_tokyo: test_tokyo.c
	@gcc -Iinclude/tokyocabinet -mmacosx-version-min=$(MACOS_VER) -o test_tokyo \
		 test_tokyo.c lib/libtokyocabinet.a -lz -lbz2 -lpthread -lm -lc

test_qdbm: test_qdbm.c
	@gcc -Iinclude/qdbm -mmacosx-version-min=$(MACOS_VER) -o test_qdbm \
		test_qdbm.c lib/libqdbm.a