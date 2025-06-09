/****************************************************************************
** Meta object code from reading C++ file 'albumwidget.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.8.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../albumwidget.h"
#include <QtCore/qmetatype.h>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'albumwidget.h' doesn't include <QObject>."
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
struct qt_meta_tag_ZN16AlbumTrackButtonE_t {};
} // unnamed namespace


#ifdef QT_MOC_HAS_STRINGDATA
static constexpr auto qt_meta_stringdata_ZN16AlbumTrackButtonE = QtMocHelpers::stringData(
    "AlbumTrackButton",
    "trackButtonClicked",
    "",
    "track*",
    "trackData",
    "trackNameButtonClicked",
    "trackAuthorButtonClicked",
    "albumLiked"
);
#else  // !QT_MOC_HAS_STRINGDATA
#error "qtmochelpers.h not found or too old."
#endif // !QT_MOC_HAS_STRINGDATA

Q_CONSTINIT static const uint qt_meta_data_ZN16AlbumTrackButtonE[] = {

 // content:
      12,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    1,   38,    2, 0x06,    1 /* Public */,
       5,    0,   41,    2, 0x06,    3 /* Public */,
       6,    0,   42,    2, 0x06,    4 /* Public */,
       7,    0,   43,    2, 0x06,    5 /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

Q_CONSTINIT const QMetaObject AlbumTrackButton::staticMetaObject = { {
    QMetaObject::SuperData::link<QPushButton::staticMetaObject>(),
    qt_meta_stringdata_ZN16AlbumTrackButtonE.offsetsAndSizes,
    qt_meta_data_ZN16AlbumTrackButtonE,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_tag_ZN16AlbumTrackButtonE_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<AlbumTrackButton, std::true_type>,
        // method 'trackButtonClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<track *, std::false_type>,
        // method 'trackNameButtonClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'trackAuthorButtonClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        // method 'albumLiked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>
    >,
    nullptr
} };

void AlbumTrackButton::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<AlbumTrackButton *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->trackButtonClicked((*reinterpret_cast< std::add_pointer_t<track*>>(_a[1]))); break;
        case 1: _t->trackNameButtonClicked(); break;
        case 2: _t->trackAuthorButtonClicked(); break;
        case 3: _t->albumLiked(); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _q_method_type = void (AlbumTrackButton::*)(track * );
            if (_q_method_type _q_method = &AlbumTrackButton::trackButtonClicked; *reinterpret_cast<_q_method_type *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
        {
            using _q_method_type = void (AlbumTrackButton::*)();
            if (_q_method_type _q_method = &AlbumTrackButton::trackNameButtonClicked; *reinterpret_cast<_q_method_type *>(_a[1]) == _q_method) {
                *result = 1;
                return;
            }
        }
        {
            using _q_method_type = void (AlbumTrackButton::*)();
            if (_q_method_type _q_method = &AlbumTrackButton::trackAuthorButtonClicked; *reinterpret_cast<_q_method_type *>(_a[1]) == _q_method) {
                *result = 2;
                return;
            }
        }
        {
            using _q_method_type = void (AlbumTrackButton::*)();
            if (_q_method_type _q_method = &AlbumTrackButton::albumLiked; *reinterpret_cast<_q_method_type *>(_a[1]) == _q_method) {
                *result = 3;
                return;
            }
        }
    }
}

const QMetaObject *AlbumTrackButton::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *AlbumTrackButton::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ZN16AlbumTrackButtonE.stringdata0))
        return static_cast<void*>(this);
    return QPushButton::qt_metacast(_clname);
}

int AlbumTrackButton::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QPushButton::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void AlbumTrackButton::trackButtonClicked(track * _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void AlbumTrackButton::trackNameButtonClicked()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void AlbumTrackButton::trackAuthorButtonClicked()
{
    QMetaObject::activate(this, &staticMetaObject, 2, nullptr);
}

// SIGNAL 3
void AlbumTrackButton::albumLiked()
{
    QMetaObject::activate(this, &staticMetaObject, 3, nullptr);
}
namespace {
struct qt_meta_tag_ZN11AlbumWidgetE_t {};
} // unnamed namespace


#ifdef QT_MOC_HAS_STRINGDATA
static constexpr auto qt_meta_stringdata_ZN11AlbumWidgetE = QtMocHelpers::stringData(
    "AlbumWidget",
    "trackButtonClicked",
    "",
    "track*",
    "trackData",
    "trackNameButtonClicked",
    "authorButtonClickedByTrackId",
    "QString*",
    "trackUsertag",
    "authorButtonClicked",
    "albumLiked"
);
#else  // !QT_MOC_HAS_STRINGDATA
#error "qtmochelpers.h not found or too old."
#endif // !QT_MOC_HAS_STRINGDATA

Q_CONSTINIT static const uint qt_meta_data_ZN11AlbumWidgetE[] = {

 // content:
      12,       // revision
       0,       // classname
       0,    0, // classinfo
       5,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       5,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    1,   44,    2, 0x06,    1 /* Public */,
       5,    1,   47,    2, 0x06,    3 /* Public */,
       6,    1,   50,    2, 0x06,    5 /* Public */,
       9,    1,   53,    2, 0x06,    7 /* Public */,
      10,    1,   56,    2, 0x06,    9 /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, 0x80000000 | 7,    8,
    QMetaType::Void, 0x80000000 | 7,    8,
    QMetaType::Void, 0x80000000 | 3,    4,

       0        // eod
};

Q_CONSTINIT const QMetaObject AlbumWidget::staticMetaObject = { {
    QMetaObject::SuperData::link<QWidget::staticMetaObject>(),
    qt_meta_stringdata_ZN11AlbumWidgetE.offsetsAndSizes,
    qt_meta_data_ZN11AlbumWidgetE,
    qt_static_metacall,
    nullptr,
    qt_incomplete_metaTypeArray<qt_meta_tag_ZN11AlbumWidgetE_t,
        // Q_OBJECT / Q_GADGET
        QtPrivate::TypeAndForceComplete<AlbumWidget, std::true_type>,
        // method 'trackButtonClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<track *, std::false_type>,
        // method 'trackNameButtonClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<track *, std::false_type>,
        // method 'authorButtonClickedByTrackId'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QString *, std::false_type>,
        // method 'authorButtonClicked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<QString *, std::false_type>,
        // method 'albumLiked'
        QtPrivate::TypeAndForceComplete<void, std::false_type>,
        QtPrivate::TypeAndForceComplete<track *, std::false_type>
    >,
    nullptr
} };

void AlbumWidget::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<AlbumWidget *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->trackButtonClicked((*reinterpret_cast< std::add_pointer_t<track*>>(_a[1]))); break;
        case 1: _t->trackNameButtonClicked((*reinterpret_cast< std::add_pointer_t<track*>>(_a[1]))); break;
        case 2: _t->authorButtonClickedByTrackId((*reinterpret_cast< std::add_pointer_t<QString*>>(_a[1]))); break;
        case 3: _t->authorButtonClicked((*reinterpret_cast< std::add_pointer_t<QString*>>(_a[1]))); break;
        case 4: _t->albumLiked((*reinterpret_cast< std::add_pointer_t<track*>>(_a[1]))); break;
        default: ;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _q_method_type = void (AlbumWidget::*)(track * );
            if (_q_method_type _q_method = &AlbumWidget::trackButtonClicked; *reinterpret_cast<_q_method_type *>(_a[1]) == _q_method) {
                *result = 0;
                return;
            }
        }
        {
            using _q_method_type = void (AlbumWidget::*)(track * );
            if (_q_method_type _q_method = &AlbumWidget::trackNameButtonClicked; *reinterpret_cast<_q_method_type *>(_a[1]) == _q_method) {
                *result = 1;
                return;
            }
        }
        {
            using _q_method_type = void (AlbumWidget::*)(QString * );
            if (_q_method_type _q_method = &AlbumWidget::authorButtonClickedByTrackId; *reinterpret_cast<_q_method_type *>(_a[1]) == _q_method) {
                *result = 2;
                return;
            }
        }
        {
            using _q_method_type = void (AlbumWidget::*)(QString * );
            if (_q_method_type _q_method = &AlbumWidget::authorButtonClicked; *reinterpret_cast<_q_method_type *>(_a[1]) == _q_method) {
                *result = 3;
                return;
            }
        }
        {
            using _q_method_type = void (AlbumWidget::*)(track * );
            if (_q_method_type _q_method = &AlbumWidget::albumLiked; *reinterpret_cast<_q_method_type *>(_a[1]) == _q_method) {
                *result = 4;
                return;
            }
        }
    }
}

const QMetaObject *AlbumWidget::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *AlbumWidget::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_ZN11AlbumWidgetE.stringdata0))
        return static_cast<void*>(this);
    return QWidget::qt_metacast(_clname);
}

int AlbumWidget::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 5)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 5;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 5)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 5;
    }
    return _id;
}

// SIGNAL 0
void AlbumWidget::trackButtonClicked(track * _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void AlbumWidget::trackNameButtonClicked(track * _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void AlbumWidget::authorButtonClickedByTrackId(QString * _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void AlbumWidget::authorButtonClicked(QString * _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void AlbumWidget::albumLiked(track * _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}
QT_WARNING_POP
