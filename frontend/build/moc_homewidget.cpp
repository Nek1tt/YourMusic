/****************************************************************************
** Meta object code from reading C++ file 'homewidget.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.8.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../homewidget.h"
#include <QtCore/qmetatype.h>
#include <QtCore/QList>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'homewidget.h' doesn't include <QObject>."
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
struct qt_meta_tag_ZN10HomeWidgetE_t {};
} // unnamed namespace


#ifdef QT_MOC_HAS_STRINGDATA
static constexpr auto qt_meta_stringdata_ZN10HomeWidgetE = QtMocHelpers::stringData(
    "HomeWidget",
    "albumClicked",
    "",
    "album",
    "albumData",
    "albumListClicked",
    "QList<album>",
    "albumList",
    "trackDoubleClicked",
    "track*",
    "trackData",
    "followersButtonClicked",
    "followingButtonClicked",
    "tracksLoadedButtonClicked",
    "tracksAddedButtonClicked",
    "authorButtonClicked",
    "QString*",
    "authorUsertag",
    "trackNameButtonClicked",
    "TrackData"
);
#else  // !QT_MOC_HAS_STRINGDATA
#error "qtmochelpers.h not found or too old."
#endif // !QT_MOC_HAS_STRINGDATA

Q_CONSTINIT static const uint qt_meta_data_ZN10HomeWidgetE[] = {

 // content:
      12,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       9,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    1,   68,    2, 0x06,    1 /* Public */,
       5,    1,   71,    2, 0x06,    3 /* Public */,
       8,    1,   74,    2, 0x06,    5 /* Public */,
      11,    0,   77,    2, 0x06,    7 /* Public */,
      12,    0,   78,    2, 0x06,    8 /* Public */,
      13,    0,   79,    2, 0x06,    9 /* Public */,
      14,    0,   80,    2, 0x06,   10 /* Public */,
      15,    1,   81,    2, 0x06,   11 /* Public */,
      18,    1,   84,    2, 0x06,   13 /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, 0x80000000 | 6,    7,
    QMetaType::Void, 0x80000000 | 9,   10,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 16,   17,
    QMetaType::Void, 0x80000000 | 9,   19,

       0        // eod
};

Q_CONSTINIT const QMetaObject HomeWidget::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_ZN10HomeWidgetE.offsetsAndSizes,
    qt_meta_data_ZN10HomeWidgetE,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_tag_ZN10HomeWidgetE_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<HomeWidget, std::true_type>,
        // method 'albumClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<album, std::false_type>,
        // method 'albumListClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QVector<album>, std::false_type>,
        // method 'trackDoubleClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<track *, std::false_type>,
        // method 'followersButtonClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'followingButtonClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'tracksLoadedButtonClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'tracksAddedButtonClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'authorButtonClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QString *, std::false_type>,
        // method 'trackNameButtonClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<track *, std::false_type>
    >,
    nullptr
} };

void HomeWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<HomeWidget *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->albumClicked((*reinterpret_cast< std::add_pointer_t<album>>(_a[1]))); break;
        case 1: _t->albumListClicked((*reinterpret_cast< std::add_pointer_t<QList<album>>>(_a[1]))); break;
        case 2: _t->trackDoubleClicked((*reinterpret_cast< std::add_pointer_t<track*>>(_a[1]))); break;
        case 3: _t->followersButtonClicked(); break;
        case 4: _t->followingButtonClicked(); break;
        case 5: _t->tracksLoadedButtonClicked(); break;
        case 6: _t->tracksAddedButtonClicked(); break;
        case 7: _t->authorButtonClicked((*reinterpret_cast< std::add_pointer_t<QString*>>(_a[1]))); break;
        case 8: _t->trackNameButtonClicked((*reinterpret_cast< std::add_pointer_t<track*>>(_a[1]))); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _q_method_type = void (HomeWidget::*)(album );
            if (_q_method_type _q_method = &HomeWidget::albumClicked; *reinterpret_cast<_q_method_type *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
        {
            using _q_method_type = void (HomeWidget::*)(QVector<album> );
            if (_q_method_type _q_method = &HomeWidget::albumListClicked; *reinterpret_cast<_q_method_type *>(_a[1]) == _q_method) {
                *result = 1;
                return;
            }
        }
        {
            using _q_method_type = void (HomeWidget::*)(track * );
            if (_q_method_type _q_method = &HomeWidget::trackDoubleClicked; *reinterpret_cast<_q_method_type *>(_a[1]) == _q_method) {
                *result = 2;
                return;
            }
        }
        {
            using _q_method_type = void (HomeWidget::*)();
            if (_q_method_type _q_method = &HomeWidget::followersButtonClicked; *reinterpret_cast<_q_method_type *>(_a[1]) == _q_method) {
                *result = 3;
                return;
            }
        }
        {
            using _q_method_type = void (HomeWidget::*)();
            if (_q_method_type _q_method = &HomeWidget::followingButtonClicked; *reinterpret_cast<_q_method_type *>(_a[1]) == _q_method) {
                *result = 4;
                return;
            }
        }
        {
            using _q_method_type = void (HomeWidget::*)();
            if (_q_method_type _q_method = &HomeWidget::tracksLoadedButtonClicked; *reinterpret_cast<_q_method_type *>(_a[1]) == _q_method) {
                *result = 5;
                return;
            }
        }
        {
            using _q_method_type = void (HomeWidget::*)();
            if (_q_method_type _q_method = &HomeWidget::tracksAddedButtonClicked; *reinterpret_cast<_q_method_type *>(_a[1]) == _q_method) {
                *result = 6;
                return;
            }
        }
        {
            using _q_method_type = void (HomeWidget::*)(QString * );
            if (_q_method_type _q_method = &HomeWidget::authorButtonClicked; *reinterpret_cast<_q_method_type *>(_a[1]) == _q_method) {
                *result = 7;
                return;
            }
        }
        {
            using _q_method_type = void (HomeWidget::*)(track * );
            if (_q_method_type _q_method = &HomeWidget::trackNameButtonClicked; *reinterpret_cast<_q_method_type *>(_a[1]) == _q_method) {
                *result = 8;
                return;
            }
        }
    }
}

const QMetaObject *HomeWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *HomeWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ZN10HomeWidgetE.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int HomeWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 9)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 9;
    }
    return _id;
}

// SIGNAL 0
void HomeWidget::albumClicked(album _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void HomeWidget::albumListClicked(QVector<album> _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void HomeWidget::trackDoubleClicked(track * _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void HomeWidget::followersButtonClicked()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}

// SIGNAL 4
void HomeWidget::followingButtonClicked()
{
    QMetaObject::activate(this, &staticMetaObject, 4, nullptr);
}

// SIGNAL 5
void HomeWidget::tracksLoadedButtonClicked()
{
    QMetaObject::activate(this, &staticMetaObject, 5, nullptr);
}

// SIGNAL 6
void HomeWidget::tracksAddedButtonClicked()
{
    QMetaObject::activate(this, &staticMetaObject, 6, nullptr);
}

// SIGNAL 7
void HomeWidget::authorButtonClicked(QString * _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}

// SIGNAL 8
void HomeWidget::trackNameButtonClicked(track * _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 8, _a);
}
QT_WARNING_POP
