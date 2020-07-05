#-------------------------------------------------
#
# Project created by QtCreator 2020-06-29T09:45:43
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = client
TEMPLATE = app


SOURCES += main.cpp\
        sign_in.cpp \
    home.cpp \
    chat.cpp \
    msg_t.cpp \
    list.cpp \
    mgs_manage.cpp

HEADERS  += sign_in.h\
    home.h \
    chat.h \
    msg_t.h \
    list.h \
    mgs_manage.h

FORMS    += sign_in.ui
CONFIG   +=c++11
