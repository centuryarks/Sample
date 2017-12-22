#-------------------------------------------------
#
# Project created by QtCreator 2017-08-28T11:23:16
#
#-------------------------------------------------

QMAKE_CC  = gcc
QMAKE_CXX = g++
QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DemoGUI
TEMPLATE = app

SOURCES += main.cpp\
        mainwindow.cpp\
        demo_control.c\
        af_control.c\
        lsc_control.c\
        ois_control.c\
        communication_jetson.c
HEADERS += mainwindow.h
FORMS   += mainwindow.ui

# SSP Setting
QMAKE_LIBDIR += /opt/vc/lib
QMAKE_LIBDIR += /usr/lib/arm-linux-gnueabihf
LIBS         += -lm -lrt

