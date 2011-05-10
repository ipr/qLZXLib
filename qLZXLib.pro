#-------------------------------------------------
#
# Project created by QtCreator 2011-05-10T18:33:58
#
#-------------------------------------------------

QT       -= gui

TARGET = qLZXLib
TEMPLATE = lib

DEFINES += QLZXLIB_LIBRARY
DEFINES += _CRT_SECURE_NO_WARNINGS

SOURCES += qlzxlib.cpp \
    UnLzx.cpp \
    AnsiFile.cpp

HEADERS += qlzxlib.h\
        qLZXLib_global.h \
    UnLzx.h \
    AnsiFile.h
