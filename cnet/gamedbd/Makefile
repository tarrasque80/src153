
TOP_SRCDIR = ..

SINGLE_THREAD = false
DEBUG_VERSION = true

include ../mk/gcc.defs.mk

INCLUDES += -I$(TOP_SRCDIR) -I$(TOP_SRCDIR)/storage -I$(TOP_SRCDIR)/include
#LDFLAGS  += -static 
DEFINES  += -DUSE_DB -DUSE_WDB -DUSE_LOGCLIENT -DMPPC_4WAY -DUSE_TRANSACTION -D_FILE_OFFSET_BITS=64
DEFINES += -D__USER__=\"$(USER)\"
CFLAGS += -lpthread -g -ggdb 

OBJS = $(LOGOBJS) ../storage/storage.o ../storage/storagetool.o gamedbserver.o state.o stubs.o gamedbd.o gamedbmanager.o accessdb.o mergedb.o mergedb2.o selldataimage.o tranlog.o transman.o utilfunction.o forbidpolicy.o abstractplayers.o tinyxml2.o
CLEAN += *.o gamedbd

all : gamedbd

gamedbd : $(OBJS) $(SHAREOBJ) $(SHARE_SOBJ) $(LOGSTUB) $(LOGOBJ)
	$(LD) $(LDFLAGS) -o $@ $(OBJS) $(SHAREOBJ) $(SHARE_SOBJ)  $(LOGSTUB) $(LOGOBJ) -lcrypto -ldl

#cashstat: $(SHAREOBJ) $(SHARE_SOBJ) ../storage/storage.o statistic.cpp
#	$(LD) $(LDFLAGS) -D_REENTRANT -o $@ statistic.cpp $(SHAREOBJ) $(SHARE_SOBJ) ../storage/storage.o -I$(INCLUDES) -DUSE_WDB
cashstat: $(SHAREOBJ) $(SHARE_SOBJ) ../storage/storage.o statistic.o $(LOGOBJ) $(LOGSTUB)
	$(LD) $(LDFLAGS) -static -o $@ statistic.o $(SHAREOBJ) $(SHARE_SOBJ) $(LOGOBJ) $(LOGSTUB) ../storage/storage.o

include ../mk/gcc.rules.mk

