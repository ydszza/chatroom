/****************************************************************************
** Meta object code from reading C++ file 'mgs_manage.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.5.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../client/mgs_manage.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mgs_manage.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.5.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_msg_manage_t {
    QByteArrayData data[14];
    char stringdata0[194];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_msg_manage_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_msg_manage_t qt_meta_stringdata_msg_manage = {
    {
QT_MOC_LITERAL(0, 0, 10), // "msg_manage"
QT_MOC_LITERAL(1, 11, 21), // "signal_sign_in_verify"
QT_MOC_LITERAL(2, 33, 0), // ""
QT_MOC_LITERAL(3, 34, 6), // "msg_t*"
QT_MOC_LITERAL(4, 41, 27), // "signal_add_or_out_newclient"
QT_MOC_LITERAL(5, 69, 18), // "signal_get_new_msg"
QT_MOC_LITERAL(6, 88, 12), // "slot_sign_in"
QT_MOC_LITERAL(7, 101, 9), // "account_t"
QT_MOC_LITERAL(8, 111, 10), // "password_t"
QT_MOC_LITERAL(9, 122, 14), // "slot_connected"
QT_MOC_LITERAL(10, 137, 17), // "slot_disconnected"
QT_MOC_LITERAL(11, 155, 14), // "slot_read_data"
QT_MOC_LITERAL(12, 170, 14), // "slot_send_data"
QT_MOC_LITERAL(13, 185, 8) // "QString*"

    },
    "msg_manage\0signal_sign_in_verify\0\0"
    "msg_t*\0signal_add_or_out_newclient\0"
    "signal_get_new_msg\0slot_sign_in\0"
    "account_t\0password_t\0slot_connected\0"
    "slot_disconnected\0slot_read_data\0"
    "slot_send_data\0QString*"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_msg_manage[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   54,    2, 0x06 /* Public */,
       4,    1,   57,    2, 0x06 /* Public */,
       5,    1,   60,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       6,    2,   63,    2, 0x0a /* Public */,
       9,    0,   68,    2, 0x0a /* Public */,
      10,    0,   69,    2, 0x0a /* Public */,
      11,    0,   70,    2, 0x0a /* Public */,
      12,    1,   71,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    2,
    QMetaType::Void, 0x80000000 | 3,    2,
    QMetaType::Void, 0x80000000 | 3,    2,

 // slots: parameters
    QMetaType::Void, QMetaType::QString, QMetaType::QString,    7,    8,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 13,    2,

       0        // eod
};

void msg_manage::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        msg_manage *_t = static_cast<msg_manage *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->signal_sign_in_verify((*reinterpret_cast< msg_t*(*)>(_a[1]))); break;
        case 1: _t->signal_add_or_out_newclient((*reinterpret_cast< msg_t*(*)>(_a[1]))); break;
        case 2: _t->signal_get_new_msg((*reinterpret_cast< msg_t*(*)>(_a[1]))); break;
        case 3: _t->slot_sign_in((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 4: _t->slot_connected(); break;
        case 5: _t->slot_disconnected(); break;
        case 6: _t->slot_read_data(); break;
        case 7: _t->slot_send_data((*reinterpret_cast< QString*(*)>(_a[1]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (msg_manage::*_t)(msg_t * );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&msg_manage::signal_sign_in_verify)) {
                *result = 0;
            }
        }
        {
            typedef void (msg_manage::*_t)(msg_t * );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&msg_manage::signal_add_or_out_newclient)) {
                *result = 1;
            }
        }
        {
            typedef void (msg_manage::*_t)(msg_t * );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&msg_manage::signal_get_new_msg)) {
                *result = 2;
            }
        }
    }
}

const QMetaObject msg_manage::staticMetaObject = {
    { &QThread::staticMetaObject, qt_meta_stringdata_msg_manage.data,
      qt_meta_data_msg_manage,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *msg_manage::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *msg_manage::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_msg_manage.stringdata0))
        return static_cast<void*>(const_cast< msg_manage*>(this));
    return QThread::qt_metacast(_clname);
}

int msg_manage::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QThread::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 8)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 8;
    }
    return _id;
}

// SIGNAL 0
void msg_manage::signal_sign_in_verify(msg_t * _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void msg_manage::signal_add_or_out_newclient(msg_t * _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void msg_manage::signal_get_new_msg(msg_t * _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_END_MOC_NAMESPACE
