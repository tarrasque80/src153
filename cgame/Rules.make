IOPATH=/root/src153/iolib
BASEPATH=/root/src153/cgame

INC=-I$(BASEPATH)/include -I$(BASEPATH) -I$(IOPATH)/inc -I$(BASEPATH)/libcm
IOLIB_OBJ=$(BASEPATH)/libgs/gs/*.o $(BASEPATH)/libgs/io/*.o $(BASEPATH)/libgs/db/*.o $(BASEPATH)/libgs/log/*.o $(BASEPATH)/libgs/sk/*.o
CMLIB=$(BASEPATH)/libcm.a $(BASEPATH)/libonline.a $(IOLIB_OBJ) $(BASEPATH)/collision/libTrace.a
DEF = -DLINUX -D_DEBUG  -D__THREAD_SPIN_LOCK__ -DUSE_LOGCLIENT -mfpmath=387
DEF += -D__USER__=\"$(USER)\"

THREAD = -D_REENTRANT -D_THREAD_SAFE -D_PTHREADS -mfpmath=387
THREADLIB = -lpthread -mfpmath=387
PCRELIB = -lpcre -mfpmath=387

ALLLIB = $(THREADLIB) $(PCRELIB) -lssl -lcrypto -lstdc++ -ldl
CSTD = -w -std=c18 -mfpmath=387 -Wno-parentheses -fpermissive -Wno-write-strings -Wno-unused-local-typedefs -Wno-stringop-truncation -Wno-error=parentheses -Wno-class-memaccess
STD = -w -std=c++23 -mfpmath=387 -Wno-parentheses -fpermissive -Wno-write-strings -Wno-unused-local-typedefs -Wno-stringop-truncation -Wno-error=parentheses -Wno-class-memaccess
OPTIMIZE = -O0 -g -ggdb -mfpmath=387
CC=gcc $(CSTD) $(DEF) $(THREAD) $(OPTIMIZE) -g -ggdb
CPP=g++ $(STD) $(DEF) $(THREAD) $(OPTIMIZE)  -g -ggdb
LD=g++ $(STD) -g -L/usr/local/ssl/lib $(OPTIMIZE) $(THREADLIB) 
AR=ar crs 
ARX=ar x

ifneq ($(wildcard .depend),)
include .depend
endif

ifeq ($(TERM),cygwin)
THREADLIB = -lpthread
CMLIB += /usr/lib/libgmon.a
DEF += -D__CYGWIN__
endif

dep:
	$(CC) -MM $(INC)  -c *.c* > .depend

