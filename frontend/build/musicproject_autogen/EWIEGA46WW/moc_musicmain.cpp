/****************************************************************************
** Meta object code from reading C++ file 'musicmain.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.8.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../musicmain.h"
#include <QtGui/qtextcursor.h>
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'musicmain.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.8.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
QT_WARNING_DISABLE_GCC("-Wuseless-cast")
namespace {
struct qt_meta_tag_ZN9MusicMainE_t {};
} // unnamed namespace


#ifdef QT_MOC_HAS_STRINGDATA
static constexpr auto qt_meta_stringdata_ZN9MusicMainE = QtMocHelpers::stringData(
    "MusicMain",
    "toggle_buttons",
    "",
    "QPushButton*",
    "on_homeTab_clicked",
    "on_createTab_clicked",
    "on_profileTab_clicked",
    "resizeEvent",
    "QResizeEvent*",
    "event",
    "setInitialSize",
    "width",
    "on_backButton_clicked",
    "on_forwardButton_clicked",
    "on_albumButton_clicked"
);
#else  // !QT_MOC_HAS_STRINGDATA
#error "qtmochelpers.h not found or too old."
#endif // !QT_MOC_HAS_STRINGDATA

Q_CONSTINIT static const uint qt_meta_data_ZN9MusicMainE[] = {

 // content:
      12,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       1,    1,   68,    2, 0x08,    1 /* Private */,
       4,    0,   71,    2, 0x08,    3 /* Private */,
       5,    0,   72,    2, 0x08,    4 /* Private */,
       6,    0,   73,    2, 0x08,    5 /* Private */,
       7,    1,   74,    2, 0x08,    6 /* Private */,
      10,    1,   77,    2, 0x08,    8 /* Private */,
      12,    0,   80,    2, 0x08,   10 /* Private */,
      13,    0,   81,    2, 0x08,   11 /* Private */,
      14,    0,   82,    2, 0x08,   12 /* Private */,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 3,    2,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 8,    9,
    QMetaType::Void, QMetaType::Int,   11,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

Q_CONSTINIT const QMetaObject MusicMain::staticMetaObject = { {
    QMetaObject::SuperData::link<QMainWindow::staticMetaObject>(),
    qt_meta_stringdata_ZN9MusicMainE.offsetsAndSizes,
    qt_meta_data_ZN9MusicMainE,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_tag_ZN9MusicMainE_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<MusicMain, std::true_type>,
        // method 'toggle_buttons'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QPushButton *, std::false_type>,
        // method 'on_homeTab_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_createTab_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_profileTab_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'resizeEvent'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QResizeEvent *, std::false_type>,
        // method 'setInitialSize'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<int, std::false_type>,
        // method 'on_backButton_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_forwardButton_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'on_albumButton_clicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>
    >,
    nullptr
} };

void MusicMain::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<MusicMain *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->toggle_buttons((*reinterpret_cast< std::add_pointer_t<QPushButton*>>(_a[1]))); break;
        case 1: _t->on_homeTab_clicked(); break;
        case 2: _t->on_createTab_clicked(); break;
        case 3: _t->on_profileTab_clicked(); break;
        case 4: _t->resizeEvent((*reinterpret_cast< std::add_pointer_t<QResizeEvent*>>(_a[1]))); break;
        case 5: _t->setInitialSize((*reinterpret_cast< std::add_pointer_t<int>>(_a[1]))); break;
        case 6: _t->on_backButton_clicked(); break;
        case 7: _t->on_forwardButton_clicked(); break;
        case 8: _t->on_albumButton_clicked(); break;
        default: ;
        }
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
        case 0:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< QPushButton* >(); break;
            }
            break;
        }
    }
}

const QMetaObject *MusicMain::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MusicMain::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ZN9MusicMainE.stringdata0))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int MusicMain::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    }
    return _id;
}
QT_WARNING_POP
