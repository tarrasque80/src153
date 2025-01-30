LIBSUBDIR=io
LIBOBJ = io/pollio.o
SUBDIR=gs 
CLEAN_SUBDIR = collision common gs io libgs libcm

all: lib collision solib libcm $(SUBDIR)

$(SUBDIR): FORCE
	cd $@; make

lib:	$(LIBSUBDIR) gslib
	ar crs libonline.a $(LIBOBJ) 

collision: FORCE
	cd collision; make
	
solib: FORCE
	cd gs; make solib

gslib:	FORCE
	cd libgs; make

libcm: FORCE
	cd libcm; make
	
$(LIBSUBDIR): FORCE
	cd $@; make 
FORCE:

clean: 	FORCE 
	rm -f *.o libonline.a libcm.a;
	-($(foreach dir,$(CLEAN_SUBDIR),$(MAKE) -C $(dir) clean;))


include Rules.make
