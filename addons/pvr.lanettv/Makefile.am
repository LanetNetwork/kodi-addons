addondir = $(prefix)/addons/pvr.lanettv
addon_DATA = \
	addon.xml \
	$(ADDONNAME) \
	icon.png
	fanart.jpg

$(ADDONNAME):
	make -C ../../src

EXTRA_DIST = \
	fanart.jpg \
	addon.xml \
	icon.png
