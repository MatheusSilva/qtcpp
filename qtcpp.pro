#-------------------------------------------------
#
# Project created by QtCreator 2015-12-05T12:12:17
#
#-------------------------------------------------

QT       += core
QT       += sql
QT       += gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = qtcpp
QTPLUGIN += QSQLMYSQL
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h \
    banco.h

FORMS    += mainwindow.ui
