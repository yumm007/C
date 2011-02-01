#OBJS 	:= $(notdir $(obj-y))  去除obj-j中非目录的部分
#SUBDIRS := $(subst ./,,$(dir $(obj-y))) 将obj-y中的 ./ 替换成空
#SUBLIBS	:= $(addsuffix built-in.o,$(SUBDIRS)) 添加后缀

#.PHONY: all
#ifeq ($(obj-y),)  如果为空
#
#all:
#	@rm -f built-in.o
#	@$(AR) rcs built-in.o  作一个假的.o文件
#else
#
#all: $(SUBDIRS) $(OBJS)
#	$(LD) -r -o built-in.o $(OBJS) $(SUBLIBS)  生成.o文件
#
#endif
#
#.PHONY: $(SUBDIRS)
#$(SUBDIRS):
#	@$(MAKE) -C $@ all

