AR  = ar
CPP = g++ -w -std=gnu++23 -DPRAGMA_PACK=4 -fpermissive -Wno-deprecated 
CC  = g++ -w -std=gnu++23 -DPRAGMA_PACK=4 -fpermissive -Wno-write-strings -Wno-parentheses -Wno-deprecated 
LD  = g++ -w -std=gnu++23 -DPRAGMA_PACK=4 -Wl,--allow-multiple-definition 


IO_DIR = $(TOP_SRCDIR)/io
CO_DIR = $(TOP_SRCDIR)/common
LOG_DIR = $(TOP_SRCDIR)/logclient

INCLUDES = -I. -I$(TOP_SRCDIR) -I$(IO_DIR) -I$(CO_DIR) -I$(TOP_SRCDIR)/rpc -I$(TOP_SRCDIR)/inl -I$(TOP_SRCDIR)/rpcdata -I/usr/include/libxml2 -I../iolib/inc -I/usr/include/openssl

SHARESRC = $(IO_DIR)/pollio.cpp $(IO_DIR)/protocol.cpp $(IO_DIR)/security.cpp $(IO_DIR)/rpc.cpp $(IO_DIR)/proxyrpc.cpp $(IO_DIR)/base64.cpp $(IO_DIR)/utf.cpp $(IO_DIR)/beaktrace.cpp $(CO_DIR)/thread.cpp $(CO_DIR)/conf.cpp $(CO_DIR)/timer.cpp $(CO_DIR)/itimer.cpp $(CO_DIR)/octets.cpp $(IO_DIR)/crypt.cpp

LOGSRC  = $(LOG_DIR)/logclientclient.cpp $(LOG_DIR)/logclienttcpclient.cpp $(LOG_DIR)/log.cpp $(LOG_DIR)/glog.cpp
LOGSTUBSRC = $(LOG_DIR)/stubs.cxx $(LOG_DIR)/state.cxx

SHARE_SOBJ = -lssl -ldl

ifeq ($(SINGLE_THREAD),true)
	DEFINES = -w -D_GNU_SOURCE -D_FILE_OFFSET_BITS=64
	SHAREOBJ := $(SHARESRC:%.cpp=%.o)
	LOGOBJ := $(LOGSRC:%.cpp=%.o) 
	LOGSTUB := $(LOGSTUBSRC:%.cxx=%.o) 
else
	DEFINES = -w -D_GNU_SOURCE -pthread -D_REENTRANT_ -D_FILE_OFFSET_BITS=64 
	LDFLAGS = -pthread 
	SHAREOBJ := $(SHARESRC:%.cpp=%_m.o)
	LOGOBJ := $(LOGSRC:%.cpp=%_m.o)
	LOGSTUB := $(LOGSTUBSRC:%.cxx=%_m.o) 
endif

ifeq ($(DEBUG_VERSION),true)
	DEFINES += -D_DEBUGINFO -D_DEBUG -g -ggdb -O0
else
	DEFINES += 
endif

DEFINES += -DUSE_EPOLL
