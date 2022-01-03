MACOS_VER=10.15
LIB=../lib
INCLUDE=../include

cflags += -ftree-vectorize -mmacosx-version-min=$(MACOS_VER)

all: test_tokyo test_qdbm test_qdbm2

test_tokyo: test_tokyo.c
	@gcc $(cflags) -I$(INCLUDE)/tokyocabinet -o test_tokyo \
		 test_tokyo.c $(LIB)/libtokyocabinet.a -lz -lbz2 -lpthread -lm -lc
	./test_tokyo

test_qdbm: test_qdbm.c
	@gcc $(cflags) -I$(INCLUDE)/qdbm -o test_qdbm test_qdbm.c $(LIB)/libqdbm.a
	./test_qdbm


test_qdbm2: test_qdbm2.c
	@gcc $(cflags) -I$(INCLUDE)/qdbm -o test_qdbm2 test_qdbm2.c $(LIB)/libqdbm.a
	./test_qdbm2



clean:
	@rm -f test_tokyo test_qdbm test_qdbm2 book casket.tct

