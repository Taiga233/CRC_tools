QT       += core gui xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CRC
TEMPLATE = app

#includepath 可以让引用的头文件b不需要加目录
INCLUDEPATH += $$PWD/libraries

#修改编译标志QMAKE，使Qt中添加对C++11标准的支持
# QMAKE_CXXFLAGS += -std=c++11
# CONFIG += C++11

SOURCES += \
    main.cpp \
    crc.cpp \
    libraries/qtmmlwidget.cpp

HEADERS += \
    crc.h \
    libraries/qtmmlwidget.h

FORMS += \
    crc.ui

RESOURCES += \
    resources.qrc
