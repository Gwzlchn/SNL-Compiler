#-------------------------------------------------
#
# Project created by QtCreator 2019-05-23T13:39:47
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = qt_snl
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

CONFIG += c++14

SOURCES += \
        source/SNL_LL1.cpp \
        source/SNL_Lexer.cpp \
        source/SNL_Production.cpp \
        source/SNL_Tokens.cpp \
        source/Utils.cpp \
        source/mainwindow.cpp \
        source/snl_compiler.cpp

HEADERS += \
        source/SNL_LL1.h \
        source/SNL_Lexer.h \
        source/SNL_Production.h \
        source/SNL_Tokens.h \
        source/Utils.h \
        source/mainwindow.h \
        source/mainwindow.h \
        source/SNL_Lexer.h

FORMS += \
        source/mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
