all: add.plugin sub.plugin mul.plugin div.plugin mod.plugin

%.plugin: %.c
	gcc -shared -fPIC -o $@ $<

