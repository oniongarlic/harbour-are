# NOTICE:
#
# Application name defined in TARGET has a corresponding QML filename.
# If name defined in TARGET is changed, the following needs to be done
# to match new name:
#   - corresponding QML filename must be changed
#   - desktop icon filename must be changed
#   - desktop filename must be changed
#   - icon definition filename in desktop file must be changed
#   - translation filenames have to be changed

# The name of your application
TARGET = harbour-are

CONFIG += sailfishapp
# CONFIG += link_pkgconfig

PKGCONFIG+= libosmscout
PKGCONFIG+= libosmscout-map
PKGCONFIG+= libosmscout-map-qt

QMAKE_CXXFLAGS += -fPIC -fvisibility=hidden -fvisibility-inlines-hidden
QMAKE_LFLAGS += -pie -rdynamic -Wl,-rpath=/usr/share/harbour-are/lib/ -Llib/

QT += dbus

SOURCES += src/harbour-are.cpp \
    src/MapWidget.cpp src/DBThread.cpp src/SearchLocationModel.cpp \
    src/positionsource.cpp

HEADERS = src/MapWidget.h src/DBThread.h src/SearchLocationModel.h \
    src/positionsource.h

OTHER_FILES += qml/harbour-are.qml \
    qml/cover/CoverPage.qml \
    qml/pages/FirstPage.qml \
    qml/pages/SecondPage.qml \
    rpm/harbour-are.changes.in \
    rpm/harbour-are.spec \
    rpm/harbour-are.yaml \
    translations/*.ts \
    harbour-are.desktop

# to disable building translations every time, comment out the
# following CONFIG line
#CONFIG += sailfishapp_i18n
#TRANSLATIONS += translations/harbour-are-de.ts

lib.path = $$PREFIX/share/harbour-are/lib/
lib.files = lib/libosm*.so.0

osm.path = $$PREFIX/share/harbour-are/osm/
osm.files = osm/*

INSTALLS += lib osm
