# A script I use on the Mac to help manage multiple build environments.
# This allows working with the released version and also testing using
# the current SourceForge trunk. (Use svn to manage that)
#
# This will clean and configure the build directory for a wxWidget build.
# (I put this script in my home dir)
#
# History
# 2018-02-19 Upgraded 3.1.0 to 3.1.1
# 2016-11-29 Removed wx3.0 support
# 2016-02-29 Added wx3.1 support
# 2015-11-10 Removed wx2.8/osx10.4 support
# 2015-11-25 Strip 10.5/10.8 sdk (my build machine is 10.11 now)

export BUILDDIR=
export DEBUG=
export LIBRARIES=
export TARGETARCH=
export TARGETSDK=
export VERSION=
export WXWIN=

USAGE="Usage $0 trunk|3.1 [debug|release] [carbon]"

if test "x$1" = "xtrunk"; then
	WXWIN=~/devtools/wx/trunk
	VERSION="--disable-compat28 --disable-compat30"
elif test "x$1" = "x3.1"; then
	WXWIN=~/devtools/wx/wxWidgets-3.1.1
	VERSION="--disable-compat28 --disable-compat30"
else
	echo $USAGE
	exit
fi
BUILDDIR="$WXWIN/build"

CONFIG_PARAMS=" --disable-mediactrl --disable-shared --enable-unicode"

if test "x$3" = "xcarbon"; then
	BUILDDIR+="-carbon"
	CONFIG_PARAMS+=" --with-osx_carbon"
else
	BUILDDIR+="-cocoa"
	CONFIG_PARAMS+=" --with-osx_cocoa"
fi

if test "x$2" = "xdebug"; then
	DEBUG="--enable-debug --enable-debug_gdb"
	BUILDDIR+="-debug"
elif test "x$2" = "xrelease"; then
	DEBUG="--disable-debug_flag"
else
	$0 $1 debug
	$0 $1 release
	exit
fi

rm -rf $BUILDDIR
mkdir $BUILDDIR
cd $BUILDDIR

if test -d /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.14.sdk; then
	echo "Using 10.14 SDK"
	export CXXFLAGS="-std=c++11 -stdlib=libc++"
	export OBJCXXFLAGS="-std=c++11 -stdlib=libc++"
	export LDFLAGS="-stdlib=libc++"
	export LIBS="-lc++"
	TARGETSDK=/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.14.sdk
	MINSDK=10.7
	TARGETARCH="--enable-macosx_arch=x86_64"

elif test -d /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.13.sdk; then
	echo "Using 10.13 SDK"
	export CXXFLAGS="-std=c++11 -stdlib=libc++"
	export OBJCXXFLAGS="-std=c++11 -stdlib=libc++"
	export LDFLAGS="-stdlib=libc++"
	export LIBS="-lc++"
	TARGETSDK=/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.13.sdk
	MINSDK=10.7
	TARGETARCH="--enable-macosx_arch=x86_64"

elif test -d /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.12.sdk; then
	echo "Using 10.12 SDK"
	export CXXFLAGS="-std=c++11 -stdlib=libc++"
	export OBJCXXFLAGS="-std=c++11 -stdlib=libc++"
	export LDFLAGS="-stdlib=libc++"
	export LIBS="-lc++"
	TARGETSDK=/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.12.sdk
	MINSDK=10.7
	TARGETARCH="--enable-macosx_arch=x86_64"

elif test -d /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.11.sdk; then
	echo "Using 10.11 SDK"
	export CXXFLAGS="-std=c++11 -stdlib=libc++"
	export OBJCXXFLAGS="-std=c++11 -stdlib=libc++"
	export LDFLAGS="-stdlib=libc++"
	export LIBS="-lc++"
	TARGETSDK=/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.11.sdk
	MINSDK=10.7
	TARGETARCH="--enable-macosx_arch=x86_64"

elif test -d /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.9.sdk; then
	echo "Using 10.9 SDK"
	export CXXFLAGS="-std=c++11 -stdlib=libc++"
	export OBJCXXFLAGS="-std=c++11 -stdlib=libc++"
	export LDFLAGS="-stdlib=libc++"
	export LIBS="-lc++"
	TARGETSDK=/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.9.sdk
	MINSDK=10.7
	TARGETARCH="--enable-macosx_arch=x86_64"
fi

if test "x$TARGETSDK" = "x"; then
	echo "ERROR: Can't find an SDK"
	exit
fi

CONFIG_PARAMS+=" --with-macosx-version-min=$MINSDK --with-macosx-sdk=$TARGETSDK $TARGETARCH"

LIBRARIES+=" --with-cxx=11 --with-expat=builtin --with-regex=builtin --with-zlib=builtin --without-libiconv --without-liblzma"

echo "../configure $COMPILERS $DEBUG $VERSION $LIBRARIES $CONFIG_PARAMS"

../configure $COMPILERS $DEBUG $VERSION $LIBRARIES $CONFIG_PARAMS