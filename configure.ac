AC_CONFIG_MACRO_DIR([m4])

dnl Process this file with autoconf to produce a configure script.
AC_INIT([kodi-pvr-lanettv], [0.0.4], [Lanet Network])

AC_CONFIG_AUX_DIR(autotools)
PREFIX_DEFAULT="/usr"
AC_PREFIX_DEFAULT($PREFIX_DEFAULT)

AC_CANONICAL_HOST
AC_CANONICAL_TARGET

dnl Setup for automake
AM_INIT_AUTOMAKE([foreign])

dnl Check for tools


LT_INIT
AC_PROG_CXX
AC_LANG_PUSH([C++])
CXXFLAGS="$CXXFLAGS -std=c++0x"
AC_PROG_MAKE_SET
AC_PROG_CPP
AC_PROG_INSTALL
AC_EXEEXT

AC_SUBST(VERSIONEXT)
OS="linux"
ISMINGW32=false
ADDONNAME=Kodi_LanetTV_nix64.so
SOEXT=so
ISAPPLE=false

case $host in
  *-*-cygwin* | *-*-mingw32*)
$    AC_LIBTOOL_WIN32_DLL
    LDFLAGS+="-no-undefined -no-cygwin"
    OS="wingl windx"
    ISMINGW32=true
    ADDONNAME=Kodi_LanetTV_win32.dll
    SOEXT=dll
    ;;
  arm*-*-linux-gnu*)
    AC_SUBST(ARCH_DEFINES, "-D_ARMEL")
    ;;
  powerpc-*-linux-gnu*)
    AC_SUBST(ARCH_DEFINES, "-D_POWERPC")
    ;;
  powerpc64-*-linux-gnu*)
    AC_SUBST(ARCH_DEFINES, "-D_POWERPC64")
    ;;
  *-apple-darwin*)
    OS="osx32 ios osx64"
    SOEXT=dylib
    ADDONNAME=Kodi_LanetTV_macOS.so
    ISAPPLE=true
    ;;
  *-androideabi)
    OS="android"
    AC_SUBST(ARCH_DEFINES, "-DANDROID")
    ADDONNAME=Kodi_LanetTV_droid.so
    ;;
esac

if test "x${libdir}" != 'x${exec_prefix}/lib'; then
  LIBDIR=${libdir}
elif test "$prefix" = "NONE"; then
  LIBDIR=$PREFIX_DEFAULT/lib/xbmc/addons
else
  LIBDIR=$prefix/lib/xbmc/addons
fi
AC_SUBST(LIBDIR)

if test "x${datadir}" != 'x${datarootdir}' && test "x${datarootdir}" = 'x${prefix}/share'; then
  DATADIR=${datadir}
elif test "$prefix" = "NONE"; then
  DATADIR=$PREFIX_DEFAULT/share/xbmc/addons
else
  DATADIR=$prefix/share/xbmc/addons
fi

AC_SUBST(DATADIR)
AM_CONDITIONAL([MINGW32], $ISMINGW32)
AM_CONDITIONAL([APPLE], $ISAPPLE)
AC_SUBST(OS)
AC_SUBST(ADDONNAME)
AC_SUBST(SOEXT)

# Finally create all the generated files
AC_OUTPUT([Makefile
          addons/Makefile
          addons/pvr.lanettv-isengard/Makefile
          addons/pvr.lanettv-isengard/addon.xml
          src/Makefile
          src/lib_json/Makefile
          src/lanettv/Makefile])

