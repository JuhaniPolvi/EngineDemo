TEMPLATE = app

QT += qml quick
CONFIG += c++11

SOURCES += main.cpp \
    engine.cpp \
    gear.cpp \
    acceleratoritem.cpp \
    brakeitem.cpp \
    speedometeritem.cpp \
    rewsmeteritem.cpp \
    gearboxitem.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

HEADERS += \
    engine.h \
    gear.h \
    acceleratoritem.h \
    brakeitem.h \
    speedometeritem.h \
    rewsmeteritem.h \
    gearboxitem.h

