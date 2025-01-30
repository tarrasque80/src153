
execdirs :=   cnet/logservice/ cnet/glinkd/ cnet/gdeliveryd/ cnet/gamedbd/ cnet/uniquenamed/ cnet/gfaction/ cskill/skill/

all: libshare subs libs gs
clean: clean-libshare clean-subs clean-libs clean-gs

configure: setrules configure-softlink
clean-configure: clean-softlink

libs: libgs libcm
clean-libs: clean-libgs clean-libcm

install:
	cp -f ./cgame/gs/gs /root/pwserver/gamed/gs; \
	chmod 777 /root/pwserver/gamed/gs; \
	cp -f ./cgame/gs/libtask.so /root/pwserver/gamed/libtask.so; \
	chmod 777 /root/pwserver/gamed/libtask.so; \
	cp -f ./cnet/gfaction/gfactiond /root/pwserver/gfactiond/gfactiond; \
	chmod 777 /root/pwserver/gfactiond/gfactiond; \
	cp -f ./cnet/uniquenamed/uniquenamed /root/pwserver/uniquenamed/uniquenamed; \
	chmod 777 /root/pwserver/uniquenamed/uniquenamed; \
	cp -f ./cnet/gamedbd/gamedbd /root/pwserver/gamedbd/gamedbd; \
	chmod 777 /root/pwserver/gamedbd/gamedbd; \
	cp -f ./cnet/gdeliveryd/gdeliveryd /root/pwserver/gdeliveryd/gdeliveryd; \
	chmod 777 /root/pwserver/gdeliveryd/gdeliveryd; \
	cp -f ./cnet/glinkd/glinkd /root/pwserver/glinkd/glinkd; \
	chmod 777 /root/pwserver/glinkd/glinkd; \
	cp -f ./cnet/gacd/gacd /root/pwserver/gacd/gacd; \
	chmod 777 /root/pwserver/gacd/gacd; \
	cp -f ./cnet/logservice/logservice /root/pwserver/logservice/logservice; \
	chmod 777 /root/pwserver/logservice/logservice;

libshare:
	cd share; \
	make;  \
	cd io; \
	make lib; \
	cd ../../

clean-libshare:
	cd share; \
	make clean;  \
	cd ../

libcm:
	cd cgame/libcm; \
	make -j8;  \
	cd ../../

clean-libcm:
	cd cgame/libcm; \
	make clean;  \
	cd ../../

libgs: libgsio libLogClient libgsPro2 libdbCli
	cd cgame/libgs; \
	mkdir -p io; \
	mkdir -p gs; \
	mkdir -p db; \
	mkdir -p sk; \
	mkdir -p log; \
	make
	cd ../../

clean-libgs: clean-libgsio clean-libLogClient clean-libgsPro2 clean-libdbCli
	cd cgame/libgs; \
	make clean; \
	cd ../../

gs:
	cd cgame; \
	make -j8; \
	cd ..;

clean-gs:
	cd cgame; \
	make clean; \
	cd ..;

.PHONY: rpcgen

setrules:
	./setrules.sh;

configure-softlink: clean-softlink
	cd cnet/; \
	ln -s ../share/common/; \
	ln -s ../share/io/; \
	ln -s ../share/mk/; \
	ln -s ../share/rpc/; \
	ln -s ../share/rpcgen; \
	ln -s ../share/storage/; \
	cd ..;
	mkdir -p iolib; \
	cd iolib; \
	mkdir -p inc; \
	cd inc; \
	ln -s ../../cnet/gamed/auctionsyslib.h; \
	ln -s ../../cnet/gamed/sysauctionlib.h; \
	ln -s ../../cnet/gdbclient/db_if.h; \
	ln -s ../../cnet/gamed/factionlib.h; \
	ln -s ../../cnet/common/glog.h; \
	ln -s ../../cnet/gamed/gsp_if.h; \
	ln -s ../../cnet/gamed/mailsyslib.h; \
	ln -s ../../cnet/gamed/privilege.hxx; \
	ln -s ../../cnet/gamed/sellpointlib.h; \
	ln -s ../../cnet/gamed/stocklib.h; \
	ln -s ../../cnet/gamed/webtradesyslib.h; \
	ln -s ../../cnet/gamed/kingelectionsyslib.h; \
	ln -s ../../cnet/gamed/pshopsyslib.h; \
	cd .. ; \
	ln -s ../cnet/io/libgsio.a; \
	ln -s ../cnet/gdbclient/libdbCli.a; \
	ln -s ../cnet/gamed/libgsPro2.a; \
	ln -s ../cnet/logclient/liblogCli.a; \
	ln -s ../cskill/skill/libskill.a; \
	cd ../cnet/gamed/header/; \
	ln -s ../../../cgame/gs/attack.h; \
	ln -s ../../../cgame/gs/dbgprt.h; \
	ln -s ../../../cgame/gs/filter.h; \
	ln -s ../../../cgame/libcm/hashtab.h; \
	ln -s ../../../cgame/gs/obj_interface.h; \
	ln -s ../../../cgame/gs/property.h; \
	ln -s ../../../cgame/gs/substance.h; \
	cd ../../../cskill/; \
	ln -s ../share/common; \
	ln -s ../share/io; \
	ln -s ../share/mk; \
	ln -s ../share/rpc; \
	ln -s ../share/storage/; \
	cd header/; \
	ln -s ../../cgame/gs/attack.h; \
	ln -s ../../cgame/gs/dbgprt.h; \
	ln -s ../../cgame/gs/filter.h; \
	ln -s ../../cgame/libcm/hashtab.h; \
	ln -s ../../cgame/gs/obj_interface.h; \
	ln -s ../../cgame/gs/property.h; \
	ln -s ../../cgame/gs/substance.h; \
	cd ../skill; \
	ln -s ../../cgame/gs/sfilterdef.h; \
	ln -s ../../cgame/gs/skillwrapper.h; \
	ln -s ../../cgame/gs/statedef.h; \
	cd ../../; \

rpcgen:
	cd cnet; \
	./rpcgen rpcalls.xml; \
	cd gfaction/operations; \
	./opgen.pl; \
	cd ../../..;

subs:
	for dir in $(execdirs); do \
        $(MAKE) -j8 -C $$dir; \
    done

clean-subs:
	for dir in $(execdirs); do \
        $(MAKE) -C $$dir clean; \
    done

libgsio:
	cd cnet/io; \
	make clean; \
	make; \
	make lib; \
	cd ../..;

clean-libgsio:
	cd cnet/io; \
	rm -rf libgsio.a; \
	cd ../..;

libLogClient:
	cd cnet/logclient; \
	make clean; \
	make -f Makefile.gs -j8; \
	cd ../..;

clean-libLogClient:
	cd cnet/logclient; \
	rm -rf liblogCli.a; \
	cd ../..;

libgsPro2:
	cd cnet/gamed; \
	make clean; \
	make lib -j8; \
	cd ../..;

clean-libgsPro2:
	cd cnet/gamed; \
	make clean; \
	cd ../..;

libdbCli:
	cd cnet/gdbclient; \
	make clean; \
	make lib -j8; \
	cd ../..;

clean-libdbCli:
	cd cnet/gdbclient; \
	make clean; \
	cd ../..;

clean-softlink:
	# 删除 cnet 目录中的软链接
	rm -f cnet/common
	rm -f cnet/io
	rm -f cnet/mk
	rm -f cnet/rpc
	rm -f cnet/rpcgen
	rm -f cnet/storage
	# 删除 iolib 目录及其内容
	rm -rf iolib
	# 删除 cnet/gamed/header 中的软链接
	rm -f cnet/gamed/header/attack.h
	rm -f cnet/gamed/header/dbgprt.h
	rm -f cnet/gamed/header/filter.h
	rm -f cnet/gamed/header/hashtab.h
	rm -f cnet/gamed/header/obj_interface.h
	rm -f cnet/gamed/header/property.h
	rm -f cnet/gamed/header/substance.h
	# 删除 cskill 中的软链接
	rm -f cskill/common
	rm -f cskill/io
	rm -f cskill/mk
	rm -f cskill/rpc
	rm -f cskill/storage
	# 删除 cskill/header 中的软链接
	rm -f cskill/header/attack.h
	rm -f cskill/header/dbgprt.h
	rm -f cskill/header/filter.h
	rm -f cskill/header/hashtab.h
	rm -f cskill/header/obj_interface.h
	rm -f cskill/header/property.h
	rm -f cskill/header/substance.h
	# 删除 cskill/skill 中的软链接
	rm -f cskill/skill/sfilterdef.h
	rm -f cskill/skill/skillwrapper.h
	rm -f cskill/skill/statedef.h
	# 删除 cnet 目录中的协议数据
	rm -f cnet/inl/*
	rm -f cnet/rpcdata/*