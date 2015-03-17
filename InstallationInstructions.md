# Introduction #
QNetAddressBook is based on Qt Libraries. Those libraries could be deployed on various platforms.

# Requirements #
QNetAddressBook needs Qt Libraries version >= 4.4.x.

# Platforms #
## X11 ##
```
$ qmake -config release PREFIX=/usr/local
$ make
# make install
# ldconfig
```

## MacOS X ##
```
$ qmake -config release -spec macx-g++
$ make
$ ./scripts/macdeploy.sh bin/QNetAddressBook.app
```

## Windows ##
```
$ qmake -config release
$ make
```