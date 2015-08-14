#!/bin/sh
APPNAME=panicbutton
APPVERSION=0.5.0
APPDIR=$APPNAME-$APPVERSION
DESKTOP=$APPNAME.desktop
ICON=$APPNAME.png
MAD=mad
PKG_COMMAND="dpkg-buildpackage -rfakeroot -sa -S"
PKGDIR=$APPDIR/src
PROJECT=$APPNAME.pro
QTC_DEBIAN=qtc_packaging/debian_fremantle
USERPROJECT=$PROJECT.user


FILES="*.cpp *.h *.ui $DESKTOP $ICON *.pri"

rm -rf $APPDIR
mkdir -p $PKGDIR
cp -p $FILES $PKGDIR
cp -p $PROJECT $PKGDIR/src.pro
cp -p $USERPROJECT $PKGDIR/src.pro.user

if [ -e ../qmakefile ] ; then
	cp -p ../qmakefile $PKGDIR
fi

cp -rp $QTC_DEBIAN $APPDIR/debian
cp -rp doc $PKGDIR
cp -p pb.pro $APPDIR/$APPNAME.pro

cd $APPDIR
$MAD $PKG_COMMAND

