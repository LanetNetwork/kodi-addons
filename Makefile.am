ACLOCAL_AMFLAGS = -I  m4
DATE = @DOLLAR_SIGN(shell date +%Y%m%d)

SUBDIRS = \
	addons \
	src

DIST_SUBDIRS = \
	$(SUBDIRS)


dist-zip: all
	cp -f changelog.txt addons/pvr.lanettv/.
	cp -r addons/pvr.lanettv .
	$(STRIP) -x pvr.lanettv/$(ADDONNAME)
	zip -9 -r ./pvr.lanettv.$(VERSION)_$(OS).zip pvr.lanettv --exclude \*.am \*.in \*Makefile \*.gitignore
	rm -Rf pvr.lanettv

