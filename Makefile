include config.mk

OBJ = fjinit.o
BIN = fjinit

all: $(BIN)

$(BIN): $(OBJ)
	$(CC) $(LDFLAGS) -o $@ $(OBJ) $(LDLIBS)

install: all
	mkdir -p $(DESTDIR)$(PREFIX)/bin
	cp -f $(BIN) $(DESTDIR)$(PREFIX)/bin
	mkdir -p $(DESTDIR)$(MANPREFIX)/man8
	sed "s/VERSION/$(VERSION)/g" < $(BIN).8 > $(DESTDIR)$(MANPREFIX)/man8/$(BIN).8

uninstall:
	rm -f $(DESTDIR)$(PREFIX)/bin/$(BIN)
	rm -f $(DESTDIR)$(MANPREFIX)/man8/$(BIN).8

dist: clean
	mkdir -p fjinit-$(VERSION)
	cp LICENSE Makefile README config.mk fjinit.8 fjinit.c fjinit-$(VERSION)
	tar -cf fjinit-$(VERSION).tar fjinit-$(VERSION)
	gzip fjinit-$(VERSION).tar
	rm -rf fjinit-$(VERSION)

clean:
	rm -f $(BIN) $(OBJ) fjinit-$(VERSION).tar.gz

.SUFFIXES: .def.h

.def.h.h:
	cp $< $@

.PHONY:
	all install uninstall dist clean
