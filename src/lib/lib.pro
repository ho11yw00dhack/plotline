
TEMPLATE = lib
TARGET = plotline
QT += core gui widgets
DEFINES += LIBPLOTLINE

LIBPLOTLINE.path = $$OUT_PWD/

HEADERS += chapter.h \
    character.h \
    completable.h \
    novel.h \
    plotline.h \
    scene.h \
    serializable.h \
    revision.h \
    utils.h \
    characterparser.h \
    author.h

SOURCES += chapter.cpp \
    character.cpp \
    completable.cpp \
    novel.cpp \
    plotline.cpp \
    scene.cpp \
    serializable.cpp \
    revision.cpp \
    utils.cpp \
    characterparser.cpp \
    author.cpp

INSTALLS += LIBPLOTLINE

CONFIG += c++11

CONFIG +=  -Wl,-export-all-symbols
win:LIBS += -lws2_32
