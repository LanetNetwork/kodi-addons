ACLOCAL_AMFLAGS = -I  m4
DATE = @DOLLAR_SIGN(shell date +%Y%m%d)

SUBDIRS = \
	addons \
	src

DIST_SUBDIRS = \
	$(SUBDIRS)


dist-zip: all
	cp -f changelog.txt addons/pvr.lanet.tv-isengard/.
	cp -r addons/pvr.lanet.tv-isengard .
	$(STRIP) -x pvr.lanet.tv-isengard/$(ADDONNAME)
	zip -9 -r ./pvr.lanet.tv-isengard.$(VERSION)_$(OS).zip pvr.lanet.tv-isengard --exclude \*.am \*.in \*Makefile \*.gitignore
	rm -Rf pvr.lanet.tv-isengard

