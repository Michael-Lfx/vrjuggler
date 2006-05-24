# ************** <auto-copyright.pl BEGIN do not edit this line> **************
#
# VR Juggler is (C) Copyright 1998-2006 by Iowa State University
#
# Original Authors:
#   Allen Bierbaum, Christopher Just,
#   Patrick Hartling, Kevin Meinert,
#   Carolina Cruz-Neira, Albert Baker
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Library General Public
# License as published by the Free Software Foundation; either
# version 2 of the License, or (at your option) any later version.
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Library General Public License for more details.
#
# You should have received a copy of the GNU Library General Public
# License along with this library; if not, write to the
# Free Software Foundation, Inc., 59 Temple Place - Suite 330,
# Boston, MA 02111-1307, USA.
#
# *************** <auto-copyright.pl END do not edit this line> ***************

# -----------------------------------------------------------------------------
# Base configure.in for Gadgeteer.
# -----------------------------------------------------------------------------
# This file is "compiled" by GNU autoconf to generate the configure script
# that is actually run.
# -----------------------------------------------------------------------------

AC_INIT([Gadgeteer Device Drivers], [1.1],
        [vrjuggler-devel@lists.sourceforge.net])
AC_PREREQ([2.53])
AC_COPYRIGHT([VR Juggler is (C) Copyright 1998-2006 by Iowa State University])
AC_CONFIG_SRCDIR([Makefile.in])
AC_REVISION($Revision$)
AC_CONFIG_AUX_DIR([../../../share/config])
DPP_PREREQ([1.5.1])
DPP_INIT

DPP_STD_CPP(yes)

# -----------------------------------------------------------------------------
# Command-line arguments (--enable-option, --with-pkg=package_name).
# -----------------------------------------------------------------------------

# ------------------------------------------------------ #
# --enable-feature[=arg] and --disable-feature arguments #
# ------------------------------------------------------ #
AC_ARG_ENABLE([versioning],
              [  --disable-versioning    Disable all versioning capabilities],
              [ENABLE_VERSIONING="$enableval"], [ENABLE_VERSIONING='yes'])

# -------------------------------------------- #
# --with-pkg[=arg] and --without-pkg arguments #
# -------------------------------------------- #

# Force the use of GCC as the compiler.
# NOTE: This is not fully implemented yet for any platform, but it is partially
# usable on Windows.
DPP_WITH_GCC(no)

AC_ARG_WITH(trackdapi,
            [  --with-trackdapi=<PATH> Trackd API installation root    [default=/usr/local]],
            [trackdapi_root="$withval"], [trackdapi_root='/usr/local'])

# Build the DTK wrapper using the DTK installation found at the given path.
# There is default path.
AC_ARG_WITH(dtk,
            [  --with-dtk=<PATH>       DTK installation],
            _with_dtk="$withval", _with_dtk='no')

#Build the VRPN wrapper using the VRPN installation found at the given path.
# There is default path.
AC_ARG_WITH(vrpn, [  --with-vrpn=<PATH>      VRPN installation],
            _with_vrpn="$withval", _with_vrpn='no')

# Define the binary format.
DPP_ABI_SETUP

# -----------------------------------------------------------------------------
# System-dependent stuff._CMA_NOWRAPPERS_
# -----------------------------------------------------------------------------
DPP_WIN32_SETUP
DPP_SYSTEM_SETUP

# Templates for Autoheader.
AH_TEMPLATE([_INCLUDE_TERMIO], [Define if building on HP-UX.])
AH_TEMPLATE([_CMA_NOWRAPPERS_], [Define if building on HP-UX.])

# Depending on the target operating system, set various command options and
# such.
case $target_os in
   # SGI running IRIX 6.*.
   irix6*)
      if test "x$USE_GCC" != "xyes" ; then
         DBG_FLAGS="$DBG_FLAGS -gslim"
      fi
      ;;
   # HP PA-RISC machine running HP-UX 10.20.
   hpux10.20)
      AC_DEFINE(_INCLUDE_TERMIO,)
      AC_DEFINE(_CMA_NOWRAPPERS_,)
      ;;
   # HP PA-RISC machine running HP-UX 11.x.
   hpux11*)
      AC_DEFINE(_INCLUDE_TERMIO,)
      AC_DEFINE(_CMA_NOWRAPPERS_,)
      ;;
esac

# -----------------------------------------------------------------------------
# Path setup.
# -----------------------------------------------------------------------------

# $srcdir is the root directory of the juggler source tree.  To get a value for
# $DRIVER_ROOT_ABS, we cd there and save the value of running pwd.  Then return
# to the directory where configure is being run ($topdir).
cd "$srcdir"
DRIVER_ROOT_ABS=`pwd`

cd "$DRIVER_ROOT_ABS/../../.."
UNIX_JUGGLERROOT_ABS=`pwd`
cd "$topdir"

if test "x$CYGPATH" != "xno" ; then
   JUGGLERROOT_ABS=`cygpath -w "$UNIX_JUGGLERROOT_ABS" | sed -e 's/\\\\/\\//g'`
else
   JUGGLERROOT_ABS="$UNIX_JUGGLERROOT_ABS"
fi

# -----------------------------------------------------------------------------
# Checks for programs.
# -----------------------------------------------------------------------------
VJ_COMPILER_SETUP
VJ_PROG_CC_PROF_FLAG(yes)
VJ_PROG_CXX_PROF_FLAG(yes)

# Ensure that the C++ compiler we've found is capable of compiling the newer
# newer C++ features that we need.
DPP_CXX_NAMESPACE([AC_MSG_ERROR([*** The library requires C++ namesapce support ***])])
DPP_CXX_HAVE_STD
DPP_CXX_INLINE([AC_MSG_ERROR([*** The library requires C++ inline support ***])])
DPP_CXX_RTTI([AC_MSG_ERROR([*** The library requires C++ RTTI support ***])])
DPP_CXX_STATIC_CAST([AC_MSG_ERROR([*** The library requires C++ static_cast<> ***])])
DPP_CXX_DYNAMIC_CAST([AC_MSG_ERROR([*** The library requires C++ dynamic_cast<> ***])])

# Ensure that a version of Perl greater than or equal to 5.004 is available.
DPP_PERL_VER(5.004, , , [AC_MSG_ERROR([*** Perl is required ***])])
AC_CHECK_PROG(MTREE_CMD, mtree, mtree, [\$(PERL) \$(scriptdir)/mtree.pl])
DPP_HAVE_GNU_MAKE(3.78, ,
    [AC_MSG_ERROR([*** The build system requires GNU make 3.78 or newer ***])])
DPP_BASIC_PROGS($PLATFORM, $OS_TYPE)
DPP_PROG_INSTALL
DPP_PROG_LINKER

# -----------------------------------------------------------------------------
# Checks for libraries.
# -----------------------------------------------------------------------------

VPR_PATH([1.1.20], , [AC_MSG_ERROR([*** VPR required for Device Drivers ***])])
GADGETEER_PATH([1.1.12], ,
               [AC_MSG_ERROR(*** Gadgeteer required for Device Drivers ***)])

# TODO: Add this back in.
#gadsave_CPPFLAGS="$CPPFLAGS"
#CPPFLAGS="$CPPFLAGS $STDFLAGS"
#GMTL_PATH(0.1.9, [/usr/local], , ,
#          [AC_MSG_ERROR([*** GMTL required for Gadgeteer ***])])
#CPPFLAGS="$gadsave_CPPFLAGS"

# -----------
# Trackd API
# -----------
DPP_LANG_SAVE
DPP_LANG_C

gadget_save_CFLAGS="$CFLAGS"
gadget_save_CPPFLAGS="$CPPFLAGS"

if test "x$trackdapi_root" != "x/usr" ; then
   TRACKD_API_INCLUDES="-I$trackdapi_root/include -I$trackdapi_root/lib"


   if test "x$OS_TYPE" = "xUNIX" ; then
      if test "x$PLATFORM" = "xIRIX" ; then
         if test "x$ABI" = "xN32" ; then
            TRACKD_API_LIB_FLAGS="-L$trackdapi_root/lib -ltrackdAPI_n32"
         elif test "x$ABI" = "x64" ; then
            TRACKD_API_LIB_FLAGS="-L$trackdapi_root/lib -ltrackdAPI_64"
         else
            TRACKD_API_LIB_FLAGS="-L$trackdapi_root/lib -ltrackdAPI"
         fi
      else
         TRACKD_API_LIB_FLAGS="-L$trackdapi_root/lib -ltrackdAPI"
      fi
   elif test "x$OS_TYPE" = "xWin32" ; then
      TRACKD_API_LIB_FLAGS="/libpath:$trackdapi_root/lib trackdAPI_MT.lib"
   fi
fi

CFLAGS="$CFLAGS $TRACKD_API_INCLUDES"
CPPFLAGS="$CPPFLAGS $TRACKD_API_INCLUDES"

# Test to see if trackdAPI.h is available.  If so, we leave
# $TRACKD_API_INCLUDES alone so that it may be used again later.  If it is
# not there, we will clear $TRACKD_API_INCLUDES to avoid polluting the compile
# line.
trackdapi_header=''
AC_CHECK_HEADER([trackdAPI.h], [trackdapi_header='trackdAPI.h'])

if test "x$trackdapi_header" != "x" ; then
   HAVE_TRACKDAPI='Y'
else
   # The test for Trackd API 4.x failed, so try the 5.x header file name.
   AC_CHECK_HEADER([trackdAPI_CC.h], [trackdapi_header='trackdAPI_CC.h'])

   if test "x$trackdapi_header" != "x" ; then
      HAVE_TRACKDAPI='Y'
   else
      AC_MSG_WARN([*** Did not find TrackdAPI in $trackdapi_root ***])
      TRACKD_API_INCLUDES=''
      HAVE_TRACKDAPI='N'
   fi
fi

# If the Trackd API was found, then define the preprocessor symbol
# GADGET_INCLUDE_TRACKDAPI_H to be the name of the header file to be
# included.
if test "x$trackdapi_header" != "x" ; then
   AC_DEFINE_UNQUOTED([GADGET_INCLUDE_TRACKDAPI_H],
                      [<$trackdapi_header>]
                      [Define to include the Trackd API header file -- for example: <trackdAPI_CC.h>])
fi

CFLAGS="$gadget_save_CFLAGS"
CPPFLAGS="$gadget_save_CPPFLAGS"

DPP_LANG_RESTORE

# ----
# DTK
# ----
try_dtk='no'
vpr_subsystem=`$VPR_CONFIG --subsystem`
VPR_SUBSYSTEM="$vpr_subsystem"

# We can use DTK in one of the following two situations:
#     1) If the NSPR subsystem is enabled, the target platform is UNIX-based,
#        and NSPR threads are not being used.
#     2) If the POSIX subsystem is enabled.
case $vpr_subsystem in
    NSPR)
        if test "x$OS_TYPE" = "xUNIX"; then
            try_dtk='yes'
        fi
        ;;
    POSIX)
        try_dtk='yes'
        ;;
esac

# The user requested that the DTK wrapper be built, and the subsystem
# configuration is right for use with DTK.
if test "x${_with_dtk}" != "xno" -a "x$try_dtk" = "xyes" ; then
    # Try to find dtk-config unless the user has already set a value for
    # $DTK_CONFIG in their environment.
    if test "x$DTK_CONFIG" = "x" ; then
        AC_PATH_PROG(DTK_CONFIG, dtk-config, no, "${_with_dtk}/bin")
    fi

    # If dtk-config was not found, we cannot proceed with the DTK checks.
    if test "x$DTK_CONFIG" = "xno" ; then
        AC_MSG_WARN(*** Cannot build DTK wrapper without dtk-config ***)
    # If dtk-config was found, $DTK_CONFIG has the path to it, so we can
    # set things up to compile with DTK.
    else
        DPP_LANG_SAVE
        DPP_LANG_CPLUSPLUS

        _vjsave_CXXFLAGS="$CXXFLAGS"
        _vjsave_CPPFLAGS="$CPPFLAGS"
        _vjsave_LDFLAGS="$LDFLAGS"
        _vjsave_LIBS="$LIBS"

        # Based on the default ABI, tell $DTK_CONFIG 
        if test "x$ABI" = "xN32" ; then
            bits='n32'
        elif test "x$ABI" = "x64" ; then
            bits='64'
        fi

        DTK_ROOT=`$DTK_CONFIG --root`
        CXXFLAGS="$CXXFLAGS `$DTK_CONFIG --cflags $bits`"
        CPPFLAGS="$CPPFLAGS `$DTK_CONFIG --include`"
        LDFLAGS="$LDFLAGS -L`$DTK_CONFIG --lib-dir $bits`"
        LIBS="$LIBS -ldtk"

        HAVE_DTK='N'

        # Determine if the installed DTK library is usable.
        AC_CACHE_CHECK(for dtkMath_matrixPrint in -ldtk,
            ac_cv_dtkMath_matrixPrint_in_dtk,
            AC_TRY_LINK([
#include <dtk.h>
void dtkMath_matrixPrint(FILE *file, dtkMath_matrix* m); ],
                        [ dtkMath_matrixPrint(0, 0); ],
                        [ ac_cv_dtkMath_matrixPrint_in_dtk='yes'
                          rm -rf ./ii_files ],
                        ac_cv_dtkMath_matrixPrint_in_dtk='no'))

        if test "x$ac_cv_dtkMath_matrixPrint_in_dtk" = "xyes" ; then
            HAVE_DTK='Y'
            DTK_INCLUDES=`$DTK_CONFIG --include`
            DTK_LDFLAGS=`$DTK_CONFIG --libs $bits`
        else
            AC_MSG_WARN(*** Cannot find DTK ***)
        fi

        CXXFLAGS="${_vjsave_CXXFLAGS}"
        CPPFLAGS="${_vjsave_CPPFLAGS}"
        LDFLAGS="${_vjsave_LDFLAGS}"
        LIBS="${_vjsave_LIBS}"

        DPP_LANG_RESTORE
    fi
# If the user wanted to compile the DTK wrapper but this script determined that
# the subsystem configuation would not allow its use, warn the user.
elif test "x${_with_dtk}" != "xno" -a "x$try_dtk" = "xno" ; then
    AC_MSG_WARN(*** DTK cannot be used with this subsystem ***)
fi


# -----------------------------------------------------------------------------
# VRPN 
# -----------------------------------------------------------------------------

# Test for the Vrpn library if the Vrpn API was enabled.
if test "x${_with_vrpn}" != "xno" ; then
   if test "x${_with_vrpn}" = "xyes" ; then
      VRPNROOT="/usr/local"
   else
      VRPNROOT="$_with_vrpn"
   fi

   _vjsave_CFLAGS="$CFLAGS"
   _vjsave_CPPFLAGS="$CPPFLAGS"
   _vjsave_LDFLAGS="$LDFLAGS"
   _vjsave_LIBS="$LIBS"

   VRPN_INCLUDES="-I$VRPNROOT/quat -I$VRPNROOT/vrpn"
#   VRPN_INCLUDES="-I$VRPNROOT/vrpn"  

   if test "x$OS_TYPE" = "xWin32" ; then
      VRPNLIBDIR='pc_win32/Debug'
   elif test "x$PLATFORM" = "xLinux" ; then
      VRPNLIBDIR='pc_linux'
   elif test "x$PLATFORM" = "xFreeBSD" ; then
      VRPNLIBDIR='pc_FreeBSD'
   elif test "x$PLATFORM" = "xAIX" ; then
      VRPNLIBDIR='powerpc_aix'
   elif test "x$PLATFORM" = "xDarwin" ; then
      VRPNLIBDIR='powerpc_macosx'
   elif test "x$PLATFORM" = "xSolaris" ; then
      case $target_cpu in
         *86)
            # Not supported by VRPN yet
            ;;
         *)
            VRPNLIBDIR='sparc_solaris'
            ;;
      esac
   elif test "x$PLATFORM" = "xIRIX" ; then
      VRPNLIBDIR='sgi_irix'
      if test "x$ABI" = "xN32" ; then
         VRPNLIBDIR="$VRPNLIBDIR.n32"	
      elif test "x$ABI" = "x64" ; then
         VRPNLIBDIR="$VRPNLIBDIR.64"	
      fi

      if test "x$ISA" = "xmips3" ; then
         VRPNLIBDIR="$VRPNLIBDIR.mips3"	
      elif test "x$ISA" = "xmips4" ; then
         VRPNLIBDIR="$VRPNLIBDIR.mips4"	
      fi
   fi

   if test "x$OS_TYPE" = "xWin32" ; then
      TEST_VRPN_LDFLAGS="/libpath:\"$VRPNROOT/quat/$VRPNLIBDIR\" /libpath:\"$VRPNROOT/vrpn/$VRPNLIBDIR\" vrpn.lib quat.lib"
   else
      # SGI link order seems to clash with default rule VRPN_LDFLAGS
      # reset below to proper value for subsequent builds.
      if test "x$PLATFORM" = "xIRIX" ; then
         TEST_VRPN_LDFLAGS="-L$VRPNROOT/quat/$VRPNLIBDIR -L$VRPNROOT/vrpn/$VRPNLIBDIR -all -lvrpn -lquat -lm"
      else
         TEST_VRPN_LDFLAGS="-L$VRPNROOT/quat/$VRPNLIBDIR -L$VRPNROOT/vrpn/$VRPNLIBDIR -lvrpn -lquat -lm"
      fi
   fi

    CPPFLAGS="$VRPN_INCLUDES $CPPFLAGS"
    LDFLAGS="$TEST_VRPN_LDFLAGS $LDFLAGS"

    vj_have_vrpn='no'

    DPP_LANG_SAVE
    DPP_LANG_CPLUSPLUS

    CPPFLAGS="$CPPFLAGS $STDFLAGS"

    # Try to link
#     AC_TRY_LINK([
# include <stdlib.h>
# include <stdio.h>
# include <signal.h>
# include <string.h>

# ifndef _WIN32
# include <strings.h>
# endif

# include <vrpn_Shared.h>
# include <vrpn_Button.h>
# ],
# [vrpn_Button_Remote * b = new vrpn_Button_Remote("");], 
#                        [ vj_have_vrpn='yes'
#                          rm -rf ./ii_files ],
#                        vj_have_vrpn='no')
   AC_CHECK_HEADER([vrpn_Button.h], [vj_have_vrpn='yes'], [vj_have_vrpn='no'])

#   AC_CHECK_LIB(vrpn, [vrpn_Button_Remote], [vj_have_vrpn='yes'],
#   [vj_have_vrpn='no'])
    CPPFLAGS="${_vjsave_CPPFLAGS}"

    DPP_LANG_RESTORE

    if test "x$vj_have_vrpn" = "xyes" ; then
       HAVE_VRPN='Y'
       VRPN_LDFLAGS="$TEST_VRPN_LDFLAGS"
    else
       HAVE_VRPN='N'
       AC_MSG_WARN([*** Did not find VRPN in $VRPNROOT ***])
    fi

    CFLAGS="${_vjsave_CFLAGS}"
    CPPFLAGS="${_vjsave_CPPFLAGS}"
    LDFLAGS="${_vjsave_LDFLAGS}"
    LIBS="${_vjsave_LIBS}"
fi


# -----------------------------------------------------------------------------
# Checks for header files.
# -----------------------------------------------------------------------------
AC_HEADER_STDC
AC_CHECK_HEADERS([fcntl.h sys/ioctl.h sys/time.h termios.h unistd.h      \
                  sys/z8530.h sys/stdsyms.h])

# -----------------------------------------------------------------------------
# Checks for typedefs, structures, and compiler characteristics.
# -----------------------------------------------------------------------------
AC_TYPE_SIZE_T

# -----------------------------------------------------------------------------
# Checks for library functions.
# -----------------------------------------------------------------------------
AC_CHECK_FUNCS(strdup strerror)

# -----------------------------------------------------------------------------
# Miscellaneous checks.
# -----------------------------------------------------------------------------
DPP_INSTALLER(vrjuggler, 0644, 0755, 0755)

# -----------------------------------------------------------------------------
# Do Makefile substitutions.
# -----------------------------------------------------------------------------


# Set the form of the version number used for versioned directory and file
# naming. Unstable builds use all three version numbers; stable builds use
# only the major and minor version numbers.
if test "x$ENABLE_VERSIONING" = "xyes" ; then
   DATA_VERSION="gadgeteer-$GADGET_VERSION"
   DRIVER_DIR_VERSION="gadgeteer-$GADGET_VERSION"

# If versioning is disabled, then none of the version variables need to be
# set except for $DATA_VERSION. Its use is different than the others. The
# important thing to note is that the directory name is unversioned in this
# case.
else
   DATA_VERSION="gadgeteer"
   DRIVER_DIR_VERSION="gadgeteer"
fi

# Add these C++ options when compiling with G++.
#if test "x$GXX" = "xyes" ; then
#   CXXFLAGS="-fhonor-std $CXXFLAGS"
#fi

CXXFLAGS="$CXXFLAGS $CXX_VISIBILITY_FLAGS $GADGET_CXXFLAGS"
DEPEND_FLAGS="$GADGET_INCLUDES"

# Note that we do not have to add to $gadget_cxxflags because user code
# would never need to know where the Trackd API headers live.
if test "x$TRACKD_API_INCLUDES" != "x" ; then
   INCLUDES="$INCLUDES $TRACKD_API_INCLUDES"
fi

# The same is true for DTK headers WRT $gadget_cxxflags.
if test "x$DTK_INCLUDES" != "x" ; then
   INCLUDES="$INCLUDES $DTK_INCLUDES"
fi

if test "x$PLATFORM" = "xIRIX" -a "x$USE_GCC" != "xyes" ; then
#    CFLAGS="$CFLAGS -woff 1685,515,608,658,799,803,852,1048,1233,1499"
    CXXFLAGS="$CXXFLAGS -w2"
elif test "x$PLATFORM" = "xDarwin" ; then
    changequote(<<, >>)
    vpr_gcc_major=`echo $dpp_gcc_ver | sed -e 's/^\([0-9]*\)\..*/\1/'`
    changequote([, ])

    # GCC 4.0 in Darwin removed -fexport-coalesced.
    if test $vpr_gcc_major -lt 4 ; then
       CXXFLAGS_DYNLIB="$CXXFLAGS_DYNLIB -fexport-coalesced"
    fi
elif test "x$OS_TYPE" = "xWin32" -a "x$USE_GCC" != "xyes" ; then
    vc_cflags='/QIfdiv /QI0f'
    vc_cxxflags='/GR /GX /EHc /QIfdiv /QI0f'
    CFLAGS="$CFLAGS $vc_cflags"
    CXXFLAGS="$CXXFLAGS $vc_cxxflags"
    DBG_FLAGS="$DBG_FLAGS /MDd"
    OPT_FLAGS="$OPT_FLAGS /MD"
fi

# For makedepend(1) to work properly on HP-UX with aCC, we have to include
# these extra paths.
if test "x$PLATFORM" = "xHP" ; then
    _aCC_ROOT="/opt/aCC"
    DEPEND_EXTRAS="$DEPEND_EXTRAS -I${_aCC_ROOT} -I${_aCC_ROOT}/include -I${_aCC_ROOT}/include/iostream"
elif test "x$OS_TYPE" = "xWin32" ; then
    DEPEND_EXTRAS="$DEPEND_EXTRAS -D__cplusplus"
fi

# Define the base path to the source directory using $(DRIVER_ROOT_ABS) as an
# alternative to using $(srcdir).
UNIX_DRIVER_ROOT_ABS="$DRIVER_ROOT_ABS"

if test "x$OS_TYPE" = "xWin32" ; then
   DSO_DRIVER_DEPS="$LDFLAGS $GADGET_LIBS_LD $GADGET_EXTRA_LIBS_LD"
   DSO_DRIVER_PROF_DEPS="$DSO_DRIVER_DEPS"
else
   DSO_DRIVER_DEPS="$LDFLAGS $GADGET_LIBS_CC $GADGET_EXTRA_LIBS_CC "
   DSO_DRIVER_PROF_DEPS="$LDFLAGS $GADGET_PROF_LIBS_CC $GADGET_EXTRA_LIBS_CC"
fi

# Translate paths from UNIX-style to Win32.
if test "x$OS_TYPE" = "xWin32" ; then
    DRIVER_ROOT_ABS=`dospath -p "$DRIVER_ROOT_ABS"`
    JUGGLERROOT_ABS=`dospath -p "$JUGGLERROOT_ABS"`

    DEPEND_EXTRAS=`dospath "$DEPEND_EXTRAS"`
    INCLUDES=`dospath "$INCLUDES"`
    LIBS=`dospath "$LIBS"`
    LN_S='cp -r'
    RM_LN='rm -rf'
    MTREE_CMD=`dospath "$MTREE_CMD"`

    JDK_HOME=`dospath -p "$JDK_HOME"`
else
    RM_LN='rm -f'
    DRIVER_ROOT_ABS="$DRIVER_ROOT_ABS"
    JUGGLERROOT_ABS="$JUGGLERROOT_ABS"
fi

DPP_SUBST

AC_SUBST(topdir)
AC_SUBST(UNIX_DRIVER_ROOT_ABS)
AC_SUBST(UNIX_JUGGLERROOT_ABS)
AC_SUBST(DRIVER_ROOT_ABS)
AC_SUBST(JUGGLERROOT_ABS)
AC_SUBST(VPR_SUBSYSTEM)

AC_SUBST(DATA_VERSION)
AC_SUBST(DRIVER_DIR_VERSION)

AC_SUBST(EXTRA_LDFLAGS)
AC_SUBST(RM_LN)

AC_SUBST(DTK_CONFIG)
AC_SUBST(HAVE_DTK)

AC_SUBST(HAVE_VRPN)
AC_SUBST(VRPNROOT)
AC_SUBST(VRPN_LDFLAGS)

AC_SUBST(DSO_DRIVER_DEPS)
AC_SUBST(DSO_DRIVER_PROF_DEPS)

AC_SUBST(HAVE_TRACKDAPI)
AC_SUBST(TRACKD_API_LIB_FLAGS)

# -----------------------------------------------------------------------------
# Final file generation step.
# -----------------------------------------------------------------------------
AC_CONFIG_FILES([
   Makefile
   Makefile.inc
   common.defs.mk
   driver.defs.mk
   make.defs.mk
   3Dconnexion/Makefile
   3Dconnexion/SpaceBall/Makefile
   5DT/Makefile
   5DT/DataGlove/Makefile
   ART/Makefile
   ART/DTrack/Makefile
   Ascension/Makefile
   Ascension/Flock/Makefile
   Ascension/MotionStar/Makefile
   Ascension/Wanda/Makefile
   Elexol/Makefile
   Elexol/Ether24/Makefile
   Fakespace/Makefile
   Fakespace/PinchGlove/Makefile
   Immersion/Makefile
   Immersion/IBox/Makefile
   Intersense/Makefile
   Intersense/IS900/Makefile
   Intersense/IntersenseAPI/Makefile
   Logitech/Makefile
   Logitech/ThreeDMouse/Makefile
   Open/Makefile
   Open/DTK/Makefile
   Open/LinuxJoydev/Makefile
   Open/Trackd/Makefile
   Open/VRPN/Makefile
   Polhemus/Makefile
   Polhemus/Fastrak/Makefile
   USDigital/Makefile
   USDigital/SerialEncoder/Makefile
   VRCO/Makefile
   VRCO/TrackdAPI/Makefile
   VirtualTechnologies/Makefile
   VirtualTechnologies/CyberGlove/Makefile
   VARS.pl
   ])

AC_OUTPUT