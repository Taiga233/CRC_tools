QT       += core gui xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CRC
TEMPLATE = app


SOURCES += \
    main.cpp \
    crc.cpp \
    qtmmlwidget.cpp

HEADERS += \
    crc.h \
    qtmmlwidget.h

FORMS += \
    crc.ui

RESOURCES += \
    resources.qrc
