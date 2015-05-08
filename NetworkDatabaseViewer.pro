#-------------------------------------------------
#
# Project created by QtCreator 2015-05-07T16:06:48
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets sql network

TARGET = NetworkDatabaseViewer
TEMPLATE = app


QMAKE_LFLAGS_WINDOWS += -Wl,--stack,32000000

win32:RC_ICONS += cube.ico

SOURCES += main.cpp\
        mainview.cpp \
    loadprogress.cpp \
    missingrecordreport.cpp \
    exportdialog.cpp

HEADERS  += mainview.h \
    loadprogress.h \
    missingrecordreport.h \
    exportdialog.h

FORMS    += mainview.ui \
    loadprogress.ui \
    missingrecordreport.ui \
    exportdialog.ui
