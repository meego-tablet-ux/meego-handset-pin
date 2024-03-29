# ---- Include
include(common.pri)


# ---- Configuration

QT += core gui declarative
CONFIG += qdbus

TARGET = pinpuk
TEMPLATE = app
target.path=/usr/bin
INSTALLS=target


# ---- Generated DBus Interfaces

!exists(modemif.h) {
    system(qdbusxml2cpp -v -c ModemIf -p modemif.h:modemif.cpp -i ofonodbustypes.h interfaces/org.ofono.Modem.xml)
}
!exists(mgrif.h) {
    system(qdbusxml2cpp -v -c MgrIf -p mgrif.h:mgrif.cpp -i ofonodbustypes.h interfaces/org.ofono.Manager.xml)
}
!exists(simif.h) {
    system(qdbusxml2cpp -v -c SimIf -p simif.h:simif.cpp interfaces/org.ofono.SimManager.xml)
}


# ---- Sources

SOURCES += \
	*.cpp

HEADERS += \
	*.h 

OTHER_FILES += \
	*.js \
	*.qml \
	interfaces/*.xml

RESOURCES += \
	*.qrc


# ---- Installation

autostart.files += *.desktop
autostart.path += $$INSTALL_ROOT/etc/xdg/autostart

pixmaps.files += *.xpm *.png
pixmaps.path += $$INSTALL_ROOT/usr/share/pixmaps

INSTALLS += pixmaps autostart


# ---- Distribution

TRANSLATIONS += *.qml *.h *.cpp

PROJECT_NAME = meego-handset-pin

dist.commands += rm -fR $${PROJECT_NAME}-$${VERSION} &&
dist.commands += git clone . $${PROJECT_NAME}-$${VERSION} &&
dist.commands += rm -fR $${PROJECT_NAME}-$${VERSION}/.git &&
dist.commands += rm -f $${PROJECT_NAME}-$${VERSION}/.gitignore &&
dist.commands += mkdir -p $${PROJECT_NAME}-$${VERSION}/ts &&
dist.commands += lupdate $${TRANSLATIONS} -ts $${PROJECT_NAME}-$${VERSION}/ts/$${PROJECT_NAME}.ts &&
dist.commands += tar jcpf $${PROJECT_NAME}-$${VERSION}.tar.bz2 $${PROJECT_NAME}-$${VERSION} &&
dist.commands += rm -fR $${PROJECT_NAME}-$${VERSION} &&
dist.commands += echo; echo Created $${PROJECT_NAME}-$${VERSION}.tar.bz2
QMAKE_EXTRA_TARGETS += dist
