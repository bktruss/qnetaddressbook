#!/bin/bash
APPNAME="QNetAddressBook"
APPVERSION="1.2rc1"
FRAMEWORKS_DIR="/Library/Frameworks"
QT_PLUGINS_DIR="/Developer/Applications/Qt/plugins"
BUNDLE_DIR=$1

# Check arguments
if [ -z $BUNDLE_DIR ]; then
	echo "Usage: `basename $0` bundle"
	exit
fi

# Copy files
# Qt
echo -n "Coping files... "
mkdir ${BUNDLE_DIR}/Contents/Frameworks
cp -R ${FRAMEWORKS_DIR}/QtCore.framework ${BUNDLE_DIR}/Contents/Frameworks/
cp -R ${FRAMEWORKS_DIR}/QtGui.framework ${BUNDLE_DIR}/Contents/Frameworks/
cp -R ${FRAMEWORKS_DIR}/QtNetwork.framework ${BUNDLE_DIR}/Contents/Frameworks/
cp -R ${FRAMEWORKS_DIR}/QtSql.framework ${BUNDLE_DIR}/Contents/Frameworks/
for i in `find ${BUNDLE_DIR}/Contents/Frameworks/ -name Headers`; do rm -rf "$i"; done
cp ${BUNDLE_DIR}/../../resources/data/QNetAddressBook.plist ${BUNDLE_DIR}/Contents/Info.plist

# libqmapcontrol
cp -R ${BUNDLE_DIR}/../libqmapcontrol.* ${BUNDLE_DIR}/Contents/Frameworks/
# Plug-ins
cp -R ${BUNDLE_DIR}/../PlugIns ${BUNDLE_DIR}/Contents/
# Qt Plug-ins
mkdir ${BUNDLE_DIR}/Contents/PlugIns/sqldrivers
cp ${QT_PLUGINS_DIR}/sqldrivers/libqsqlite.dylib ${BUNDLE_DIR}/Contents/PlugIns/sqldrivers/
echo "Done."

echo -n "Stripping... "
strip -x ${BUNDLE_DIR}/Contents/Frameworks/QtCore.framework/Versions/4/QtCore
strip -x ${BUNDLE_DIR}/Contents/Frameworks/QtGui.framework/Versions/4/QtGui
strip -x ${BUNDLE_DIR}/Contents/Frameworks/QtSql.framework/Versions/4/QtSql
strip -x ${BUNDLE_DIR}/Contents/Frameworks/QtNetwork.framework/Versions/4/QtNetwork
strip -x ${BUNDLE_DIR}/Contents/Frameworks/libqmapcontrol.0.9.6.dylib
strip -x ${BUNDLE_DIR}/Contents/PlugIns/sqldrivers/libqsqlite.dylib
strip -x ${BUNDLE_DIR}/Contents/PlugIns/libimportkismetcsv.dylib
strip -x ${BUNDLE_DIR}/Contents/PlugIns/libimportnetstumbler.dylib
strip ${BUNDLE_DIR}/Contents/MacOS/QNetAddressBook
echo "Done"

echo -n "Changing Frameworks ids... "
# Change the id of the frameworks.
install_name_tool -id @executable_path/../Frameworks/QtCore.framework/Versions/4/QtCore ${BUNDLE_DIR}/Contents/Frameworks/QtCore.framework/Versions/4/QtCore
install_name_tool -id @executable_path/../Frameworks/QtGui.framework/Versions/4/QtGui ${BUNDLE_DIR}/Contents/Frameworks/QtGui.framework/Versions/4/QtGui
install_name_tool -id @executable_path/../Frameworks/QtNetwork.framework/Versions/4/QtNetwork ${BUNDLE_DIR}/Contents/Frameworks/QtNetwork.framework/Versions/4/QtNetwork
install_name_tool -id @executable_path/../Frameworks/QtSql.framework/Versions/4/QtSql ${BUNDLE_DIR}/Contents/Frameworks/QtSql.framework/Versions/4/QtSql
# Change the id of libqmapcontrol
install_name_tool -id @executable_path/../Frameworks/libqmapcontrol.0.9.6.dylib ${BUNDLE_DIR}/Contents/Frameworks/libqmapcontrol.0.9.6.dylib
echo "Done"

echo -n "Changing the library references in QNetAddressBook... "
# Change the reference in QNetAddressBook.
install_name_tool -change QtCore.framework/Versions/4/QtCore @executable_path/../Frameworks/QtCore.framework/Versions/4/QtCore ${BUNDLE_DIR}/Contents/MacOS/QNetAddressBook
install_name_tool -change QtGui.framework/Versions/4/QtGui @executable_path/../Frameworks/QtGui.framework/Versions/4/QtGui ${BUNDLE_DIR}/Contents/MacOS/QNetAddressBook
install_name_tool -change QtNetwork.framework/Versions/4/QtNetwork @executable_path/../Frameworks/QtNetwork.framework/Versions/4/QtNetwork ${BUNDLE_DIR}/Contents/MacOS/QNetAddressBook
install_name_tool -change QtSql.framework/Versions/4/QtSql @executable_path/../Frameworks/QtSql.framework/Versions/4/QtSql ${BUNDLE_DIR}/Contents/MacOS/QNetAddressBook
install_name_tool -change libqmapcontrol.0.dylib @executable_path/../Frameworks/libqmapcontrol.0.dylib ${BUNDLE_DIR}/Contents/MacOS/QNetAddressBook
echo "Done"

echo -n "Changing the library references in the libraries and plug-ins... "
# Change the reference in the libraries.
install_name_tool -change QtCore.framework/Versions/4/QtCore @executable_path/../Frameworks/QtCore.framework/Versions/4/QtCore ${BUNDLE_DIR}/Contents/Frameworks/QtGui.framework/Versions/4/QtGui
install_name_tool -change QtCore.framework/Versions/4/QtCore @executable_path/../Frameworks/QtCore.framework/Versions/4/QtCore ${BUNDLE_DIR}/Contents/Frameworks/QtNetwork.framework/Versions/4/QtNetwork
install_name_tool -change QtCore.framework/Versions/4/QtCore @executable_path/../Frameworks/QtCore.framework/Versions/4/QtCore ${BUNDLE_DIR}/Contents/Frameworks/QtSql.framework/Versions/4/QtSql
# Change the reference in libqmapcontrol
install_name_tool -change QtCore.framework/Versions/4/QtCore @executable_path/../Frameworks/QtCore.framework/Versions/4/QtCore ${BUNDLE_DIR}/Contents/Frameworks/libqmapcontrol.0.9.6.dylib
install_name_tool -change QtGui.framework/Versions/4/QtGui @executable_path/../Frameworks/QtGui.framework/Versions/4/QtGui ${BUNDLE_DIR}/Contents/Frameworks/libqmapcontrol.0.9.6.dylib
install_name_tool -change QtNetwork.framework/Versions/4/QtNetwork @executable_path/../Frameworks/QtNetwork.framework/Versions/4/QtNetwork ${BUNDLE_DIR}/Contents/Frameworks/libqmapcontrol.0.9.6.dylib
# Change the reference in plug-ins
install_name_tool -change QtCore.framework/Versions/4/QtCore @executable_path/../Frameworks/QtCore.framework/Versions/4/QtCore ${BUNDLE_DIR}/Contents/PlugIns/libimportkismetcsv.dylib
install_name_tool -change QtCore.framework/Versions/4/QtCore @executable_path/../Frameworks/QtCore.framework/Versions/4/QtCore ${BUNDLE_DIR}/Contents/PlugIns/libimportnetstumbler.dylib
# Change the reference in Qt plug-ins
install_name_tool -change QtCore.framework/Versions/4/QtCore @executable_path/../Frameworks/QtCore.framework/Versions/4/QtCore ${BUNDLE_DIR}/Contents/PlugIns/sqldrivers/libqsqlite.dylib
install_name_tool -change QtSql.framework/Versions/4/QtSql @executable_path/../Frameworks/QtSql.framework/Versions/4/QtSql ${BUNDLE_DIR}/Contents/PlugIns/sqldrivers/libqsqlite.dylib
echo "Done"

echo -n "Building disk image... "
# Build disk image
IMAGE_DIR="/tmp/${APPNAME}.$$"
mkdir $IMAGE_DIR
cp -R $BUNDLE_DIR $IMAGE_DIR
hdiutil create -ov -srcfolder $IMAGE_DIR -format UDBZ -volname "$APPNAME - $APPVERSION" "${APPNAME} - ${APPVERSION}.dmg" -quiet
hdiutil internet-enable -yes "${APPNAME} - ${APPVERSION}.dmg" -quiet
rm -rf $IMAGE_DIR
echo "Done"
