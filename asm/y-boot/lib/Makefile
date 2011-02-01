SUBDIRS	:=	string/
SUBDIRS	+=	math/

SUBLIBS  = $(addsuffix $(LIBNAME), $(SUBDIRS))

.PHONY:	all
all:	$(SUBDIRS)
	@$(LD) -r -o $(LIBNAME) $(SUBLIBS)

.PHONY: $(SUBDIRS)
$(SUBDIRS):
	@$(MAKE) -C $@ all

.PHONY: clean
clean:
	@for i in $(SUBDIRS);do $(MAKE) -C $$i clean;done
	@rm -f $(LIBNAME)



