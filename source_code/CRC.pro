QT       += core gui xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CRC
TEMPLATE = app

#includepath 可以让引用的头文件b不需要加目录
INCLUDEPATH += $$PWD/libraries

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
