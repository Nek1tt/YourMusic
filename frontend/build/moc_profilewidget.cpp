/****************************************************************************
** Meta object code from reading C++ file 'profilewidget.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.8.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../profilewidget.h"
#include <QtCore/qmetatype.h>
#include <QtCore/QList>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'profilewidget.h' doesn't include <QObject>."
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
struct qt_meta_tag_ZN13ProfileWidgetE_t {};
} // unnamed namespace


#ifdef QT_MOC_HAS_STRINGDATA
static constexpr auto qt_meta_stringdata_ZN13ProfileWidgetE = QtMocHelpers::stringData(
    "ProfileWidget",
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

Q_CONSTINIT static const uint qt_meta_data_ZN13ProfileWidgetE[] = {

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

Q_CONSTINIT const QMetaObject ProfileWidget::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_ZN13ProfileWidgetE.offsetsAndSizes,
    qt_meta_data_ZN13ProfileWidgetE,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_tag_ZN13ProfileWidgetE_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<ProfileWidget, std::true_type>,
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

void ProfileWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<ProfileWidget *>(_o);
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
            using _q_method_type = void (ProfileWidget::*)(album );
            if (_q_method_type _q_method = &ProfileWidget::albumClicked; *reinterpret_cast<_q_method_type *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
        {
            using _q_method_type = void (ProfileWidget::*)(QVector<album> );
            if (_q_method_type _q_method = &ProfileWidget::albumListClicked; *reinterpret_cast<_q_method_type *>(_a[1]) == _q_method) {
                *result = 1;
                return;
            }
        }
        {
            using _q_method_type = void (ProfileWidget::*)(track * );
            if (_q_method_type _q_method = &ProfileWidget::trackDoubleClicked; *reinterpret_cast<_q_method_type *>(_a[1]) == _q_method) {
                *result = 2;
                return;
            }
        }
        {
            using _q_method_type = void (ProfileWidget::*)();
            if (_q_method_type _q_method = &ProfileWidget::followersButtonClicked; *reinterpret_cast<_q_method_type *>(_a[1]) == _q_method) {
                *result = 3;
                return;
            }
        }
        {
            using _q_method_type = void (ProfileWidget::*)();
            if (_q_method_type _q_method = &ProfileWidget::followingButtonClicked; *reinterpret_cast<_q_method_type *>(_a[1]) == _q_method) {
                *result = 4;
                return;
            }
        }
        {
            using _q_method_type = void (ProfileWidget::*)();
            if (_q_method_type _q_method = &ProfileWidget::tracksLoadedButtonClicked; *reinterpret_cast<_q_method_type *>(_a[1]) == _q_method) {
                *result = 5;
                return;
            }
        }
        {
            using _q_method_type = void (ProfileWidget::*)();
            if (_q_method_type _q_method = &ProfileWidget::tracksAddedButtonClicked; *reinterpret_cast<_q_method_type *>(_a[1]) == _q_method) {
                *result = 6;
                return;
            }
        }
        {
            using _q_method_type = void (ProfileWidget::*)(QString * );
            if (_q_method_type _q_method = &ProfileWidget::authorButtonClicked; *reinterpret_cast<_q_method_type *>(_a[1]) == _q_method) {
                *result = 7;
                return;
            }
        }
        {
            using _q_method_type = void (ProfileWidget::*)(track * );
            if (_q_method_type _q_method = &ProfileWidget::trackNameButtonClicked; *reinterpret_cast<_q_method_type *>(_a[1]) == _q_method) {
                *result = 8;
                return;
            }
        }
    }
}

const QMetaObject *ProfileWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ProfileWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ZN13ProfileWidgetE.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int ProfileWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
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
void ProfileWidget::albumClicked(album _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void ProfileWidget::albumListClicked(QVector<album> _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void ProfileWidget::trackDoubleClicked(track * _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void ProfileWidget::followersButtonClicked()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}

// SIGNAL 4
void ProfileWidget::followingButtonClicked()
{
    QMetaObject::activate(this, &staticMetaObject, 4, nullptr);
}

// SIGNAL 5
void ProfileWidget::tracksLoadedButtonClicked()
{
    QMetaObject::activate(this, &staticMetaObject, 5, nullptr);
}

// SIGNAL 6
void ProfileWidget::tracksAddedButtonClicked()
{
    QMetaObject::activate(this, &staticMetaObject, 6, nullptr);
}

// SIGNAL 7
void ProfileWidget::authorButtonClicked(QString * _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 7, _a);
}

// SIGNAL 8
void ProfileWidget::trackNameButtonClicked(track * _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 8, _a);
}
QT_WARNING_POP
