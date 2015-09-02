ACLOCAL_AMFLAGS = -I  m4
DATE = @DOLLAR_SIGN(shell date +%Y%m%d)

SUBDIRS = \
	addons \
	src

DIST_SUBDIRS = \
	$(SUBDIRS)


dist-zip: all
	cp -f changelog.txt addons/pvr.lanettv-isengard/.
	cp -r addons/pvr.lanettv-isengard .
	$(STRIP) -x pvr.lanettv-isengard/$(ADDONNAME)
	zip -9 -r ./pvr.lanettv-isengard.$(VERSION)_$(OS).zip pvr.lanettv-isengard --exclude \*.am \*.in \*Makefile \*.gitignore
	rm -Rf pvr.lanettv-isengard

