/****************************************************************************
** Meta object code from reading C++ file 'sign_in.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.5.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../client/sign_in.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'sign_in.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.5.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_sign_in_t {
    QByteArrayData data[9];
    char stringdata0[85];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_sign_in_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_sign_in_t qt_meta_stringdata_sign_in = {
    {
QT_MOC_LITERAL(0, 0, 7), // "sign_in"
QT_MOC_LITERAL(1, 8, 14), // "signal_sign_in"
QT_MOC_LITERAL(2, 23, 0), // ""
QT_MOC_LITERAL(3, 24, 7), // "account"
QT_MOC_LITERAL(4, 32, 8), // "password"
QT_MOC_LITERAL(5, 41, 12), // "slot_sign_in"
QT_MOC_LITERAL(6, 54, 19), // "slot_sign_in_verify"
QT_MOC_LITERAL(7, 74, 6), // "msg_t*"
QT_MOC_LITERAL(8, 81, 3) // "msg"

    },
    "sign_in\0signal_sign_in\0\0account\0"
    "password\0slot_sign_in\0slot_sign_in_verify\0"
    "msg_t*\0msg"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_sign_in[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       1,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,   29,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       5,    0,   34,    2, 0x0a /* Public */,
       6,    1,   35,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString, QMetaType::QString,    3,    4,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 7,    8,

       0        // eod
};

void sign_in::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        sign_in *_t = static_cast<sign_in *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->signal_sign_in((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 1: _t->slot_sign_in(); break;
        case 2: _t->slot_sign_in_verify((*reinterpret_cast< msg_t*(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (sign_in::*_t)(QString , QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&sign_in::signal_sign_in)) {
                *result = 0;
            }
        }
    }
}

const QMetaObject sign_in::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_sign_in.data,
      qt_meta_data_sign_in,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *sign_in::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *sign_in::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_sign_in.stringdata0))
        return static_cast<void*>(const_cast< sign_in*>(this));
    return QWidget::qt_metacast(_clname);
}

int sign_in::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 3)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 3;
    }
    return _id;
}

// SIGNAL 0
void sign_in::signal_sign_in(QString _t1, QString _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}
QT_END_MOC_NAMESPACE