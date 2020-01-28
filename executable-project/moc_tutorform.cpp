/****************************************************************************
** Meta object code from reading C++ file 'tutorform.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.13.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../CourseWorkFinal/tutorform.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'tutorform.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.13.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_TutorForm_t {
    QByteArrayData data[17];
    char stringdata0[324];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_TutorForm_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_TutorForm_t qt_meta_stringdata_TutorForm = {
    {
QT_MOC_LITERAL(0, 0, 9), // "TutorForm"
QT_MOC_LITERAL(1, 10, 15), // "on_exit_clicked"
QT_MOC_LITERAL(2, 26, 0), // ""
QT_MOC_LITERAL(3, 27, 21), // "on_AddStudent_clicked"
QT_MOC_LITERAL(4, 49, 24), // "on_UpdateStudent_clicked"
QT_MOC_LITERAL(5, 74, 24), // "on_DeleteStudent_clicked"
QT_MOC_LITERAL(6, 99, 22), // "on_AddMaterial_clicked"
QT_MOC_LITERAL(7, 122, 19), // "on_EditName_clicked"
QT_MOC_LITERAL(8, 142, 23), // "on_EditMaterial_clicked"
QT_MOC_LITERAL(9, 166, 25), // "on_treeView_doubleClicked"
QT_MOC_LITERAL(10, 192, 11), // "QModelIndex"
QT_MOC_LITERAL(11, 204, 5), // "index"
QT_MOC_LITERAL(12, 210, 25), // "on_DeleteMaterial_clicked"
QT_MOC_LITERAL(13, 236, 21), // "on_Connection_clicked"
QT_MOC_LITERAL(14, 258, 27), // "on_DeleteConnection_clicked"
QT_MOC_LITERAL(15, 286, 18), // "on_AddTest_clicked"
QT_MOC_LITERAL(16, 305, 18) // "on_Results_clicked"

    },
    "TutorForm\0on_exit_clicked\0\0"
    "on_AddStudent_clicked\0on_UpdateStudent_clicked\0"
    "on_DeleteStudent_clicked\0"
    "on_AddMaterial_clicked\0on_EditName_clicked\0"
    "on_EditMaterial_clicked\0"
    "on_treeView_doubleClicked\0QModelIndex\0"
    "index\0on_DeleteMaterial_clicked\0"
    "on_Connection_clicked\0on_DeleteConnection_clicked\0"
    "on_AddTest_clicked\0on_Results_clicked"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_TutorForm[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
      13,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   79,    2, 0x08 /* Private */,
       3,    0,   80,    2, 0x08 /* Private */,
       4,    0,   81,    2, 0x08 /* Private */,
       5,    0,   82,    2, 0x08 /* Private */,
       6,    0,   83,    2, 0x08 /* Private */,
       7,    0,   84,    2, 0x08 /* Private */,
       8,    0,   85,    2, 0x08 /* Private */,
       9,    1,   86,    2, 0x08 /* Private */,
      12,    0,   89,    2, 0x08 /* Private */,
      13,    0,   90,    2, 0x08 /* Private */,
      14,    0,   91,    2, 0x08 /* Private */,
      15,    0,   92,    2, 0x08 /* Private */,
      16,    0,   93,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 10,   11,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void TutorForm::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<TutorForm *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->on_exit_clicked(); break;
        case 1: _t->on_AddStudent_clicked(); break;
        case 2: _t->on_UpdateStudent_clicked(); break;
        case 3: _t->on_DeleteStudent_clicked(); break;
        case 4: _t->on_AddMaterial_clicked(); break;
        case 5: _t->on_EditName_clicked(); break;
        case 6: _t->on_EditMaterial_clicked(); break;
        case 7: _t->on_treeView_doubleClicked((*reinterpret_cast< const QModelIndex(*)>(_a[1]))); break;
        case 8: _t->on_DeleteMaterial_clicked(); break;
        case 9: _t->on_Connection_clicked(); break;
        case 10: _t->on_DeleteConnection_clicked(); break;
        case 11: _t->on_AddTest_clicked(); break;
        case 12: _t->on_Results_clicked(); break;
        default: ;
        }
    }
}

QT_INIT_METAOBJECT const QMetaObject TutorForm::staticMetaObject = { {
    &QDialog::staticMetaObject,
    qt_meta_stringdata_TutorForm.data,
    qt_meta_data_TutorForm,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *TutorForm::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *TutorForm::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_TutorForm.stringdata0))
        return static_cast<void*>(this);
    return QDialog::qt_metacast(_clname);
}

int TutorForm::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 13)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 13;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 13)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 13;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
