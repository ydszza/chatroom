#-------------------------------------------------
#
# Project created by QtCreator 2020-08-12T16:27:01
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Client
TEMPLATE = app


SOURCES += main.cpp\
        signin.cpp \
    home.cpp \
    msgmanage.cpp \
    listmanage.cpp \
    chat.cpp

HEADERS  += signin.h \
    home.h \
    msgmanage.h \
    listmanage.h \
    chat.h

FORMS    += signin.ui

CONFIG += c++11
