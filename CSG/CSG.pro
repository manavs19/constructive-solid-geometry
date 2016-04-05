#-------------------------------------------------
#
# Project created by QtCreator 2016-03-31T17:55:07
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CSG
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    voxelset.cpp \
    point3d.cpp \
    pointset.cpp \
    glwidget.cpp \
    inputdialog.cpp

HEADERS  += mainwindow.h \
    voxelset.h \
    point3d.h \
    pointset.h \
    glwidget.h \
    inputdialog.h

FORMS    += mainwindow.ui \
    inputdialog.ui

LIBS += -lglut -L/usr/local/lib -lGLU
