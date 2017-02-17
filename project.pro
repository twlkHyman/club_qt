TEMPLATE = app

QT += qml quick widgets
QT += serialport
QT += sql
CONFIG += c++11

SOURCES += main.cpp \
    sqlite3.c \
    agvcom.cpp \
    agvfloattostring.cpp \
    agvpara.cpp \
    agvrx.cpp \
    agvtx.cpp \
    voice.cpp \
    spcom.cpp \
    modbus.cpp \
    v_modbusrx.cpp \
    v_modbutx.cpp

RESOURCES += qml.qrc

INCLUDEPATH += "C:/Program Files (x86)/Windows Kits/10/Include/10.0.10240.0/ucrt"
LIBS += -L"C:/Program Files (x86)/Windows Kits/10/Lib/10.0.10240.0/ucrt/x64"
# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

HEADERS += \
    sqlite3.h \
    sqlite3ext.h \
    agvcom.h \
    agvfloattostring.h \
    agvparastruct.h \
    agvrx.h \
    agvtx.h \
    voice.h \
    spcom.h \
    modbus.h \
    paratypedef.h \
    v_modbusrx.h \
    v_modbutx.h
