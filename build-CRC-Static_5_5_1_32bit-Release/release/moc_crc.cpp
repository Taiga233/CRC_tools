/****************************************************************************
** Meta object code from reading C++ file 'crc.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.5.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../source_code/crc.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'crc.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.5.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_crc_t {
    QByteArrayData data[8];
    char stringdata0[158];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_crc_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_crc_t qt_meta_stringdata_crc = {
    {
QT_MOC_LITERAL(0, 0, 3), // "crc"
QT_MOC_LITERAL(1, 4, 31), // "on_comboBox_currentIndexChanged"
QT_MOC_LITERAL(2, 36, 0), // ""
QT_MOC_LITERAL(3, 37, 5), // "index"
QT_MOC_LITERAL(4, 43, 27), // "on_pushButton_clear_clicked"
QT_MOC_LITERAL(5, 71, 31), // "on_pushButton_calculate_clicked"
QT_MOC_LITERAL(6, 103, 27), // "on_pushButton_table_clicked"
QT_MOC_LITERAL(7, 131, 26) // "on_pushButton_file_clicked"

    },
    "crc\0on_comboBox_currentIndexChanged\0"
    "\0index\0on_pushButton_clear_clicked\0"
    "on_pushButton_calculate_clicked\0"
    "on_pushButton_table_clicked\0"
    "on_pushButton_file_clicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_crc[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   39,    2, 0x08 /* Private */,
       4,    0,   42,    2, 0x08 /* Private */,
       5,    0,   43,    2, 0x08 /* Private */,
       6,    0,   44,    2, 0x08 /* Private */,
       7,    0,   45,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void, QMetaType::Int,    3,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void crc::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        crc *_t = static_cast<crc *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->on_comboBox_currentIndexChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 1: _t->on_pushButton_clear_clicked(); break;
        case 2: _t->on_pushButton_calculate_clicked(); break;
        case 3: _t->on_pushButton_table_clicked(); break;
        case 4: _t->on_pushButton_file_clicked(); break;
        default: ;
        }
    }
}

const QMetaObject crc::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_crc.data,
      qt_meta_data_crc,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *crc::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *crc::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_crc.stringdata0))
        return static_cast<void*>(const_cast< crc*>(this));
    return QWidget::qt_metacast(_clname);
}

int crc::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 5)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 5;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
