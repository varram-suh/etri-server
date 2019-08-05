#-------------------------------------------------
#
# Project created by QtCreator 2019-03-18T00:15:46
#
#-------------------------------------------------

QT       += core gui multimedia widgets charts printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = qt-json-visualization
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
    circular-buffer.cpp \
    debugger.cpp \
    license-session.cpp \
    main.cpp \
    mainwindow.cpp \
    mdi-child.cpp \
    protobuf-c/protobuf-c.c \
    qcustomplot.cpp \
    simple.pb-c.c

HEADERS += \
    asio-server.h \
    circular-buffer.h \
    debugger.h \
    license-session.h \
    mainwindow.h \
    packet-buffer.h \
    mdi-child.h \
    protobuf-c/protobuf-c.h \
    qcustomplot.h \
    simple.pb-c.h




INCLUDEPATH += /usr/local/include/ \
               /usr/local/include/opencv

FORMS += \
    mainwindow.ui \
    mdichild.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target




unix|win32: LIBS += `pkg-config --libs opencv` -lboost_filesystem -lboost_system -lboost_date_time -lpthread /usr/local/lib/libprotobuf.so

