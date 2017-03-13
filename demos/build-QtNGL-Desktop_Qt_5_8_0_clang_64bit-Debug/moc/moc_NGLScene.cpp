/****************************************************************************
** Meta object code from reading C++ file 'NGLScene.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.8.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../QtNGL-master/include/NGLScene.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'NGLScene.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.8.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_NGLScene_t {
    QByteArrayData data[22];
    char stringdata0[207];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_NGLScene_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_NGLScene_t qt_meta_stringdata_NGLScene = {
    {
QT_MOC_LITERAL(0, 0, 8), // "NGLScene"
QT_MOC_LITERAL(1, 9, 15), // "toggleWireframe"
QT_MOC_LITERAL(2, 25, 0), // ""
QT_MOC_LITERAL(3, 26, 5), // "_mode"
QT_MOC_LITERAL(4, 32, 12), // "setXRotation"
QT_MOC_LITERAL(5, 45, 2), // "_x"
QT_MOC_LITERAL(6, 48, 12), // "setYRotation"
QT_MOC_LITERAL(7, 61, 2), // "_y"
QT_MOC_LITERAL(8, 64, 12), // "setZRotation"
QT_MOC_LITERAL(9, 77, 2), // "_z"
QT_MOC_LITERAL(10, 80, 9), // "setXScale"
QT_MOC_LITERAL(11, 90, 9), // "setYScale"
QT_MOC_LITERAL(12, 100, 9), // "setZScale"
QT_MOC_LITERAL(13, 110, 12), // "setXPosition"
QT_MOC_LITERAL(14, 123, 12), // "setYPosition"
QT_MOC_LITERAL(15, 136, 12), // "setZPosition"
QT_MOC_LITERAL(16, 149, 13), // "setObjectMode"
QT_MOC_LITERAL(17, 163, 2), // "_i"
QT_MOC_LITERAL(18, 166, 9), // "setColour"
QT_MOC_LITERAL(19, 176, 7), // "LoadOBJ"
QT_MOC_LITERAL(20, 184, 15), // "ngl::ShaderLib*"
QT_MOC_LITERAL(21, 200, 6) // "shader"

    },
    "NGLScene\0toggleWireframe\0\0_mode\0"
    "setXRotation\0_x\0setYRotation\0_y\0"
    "setZRotation\0_z\0setXScale\0setYScale\0"
    "setZScale\0setXPosition\0setYPosition\0"
    "setZPosition\0setObjectMode\0_i\0setColour\0"
    "LoadOBJ\0ngl::ShaderLib*\0shader"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_NGLScene[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      13,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    1,   79,    2, 0x0a /* Public */,
       4,    1,   82,    2, 0x0a /* Public */,
       6,    1,   85,    2, 0x0a /* Public */,
       8,    1,   88,    2, 0x0a /* Public */,
      10,    1,   91,    2, 0x0a /* Public */,
      11,    1,   94,    2, 0x0a /* Public */,
      12,    1,   97,    2, 0x0a /* Public */,
      13,    1,  100,    2, 0x0a /* Public */,
      14,    1,  103,    2, 0x0a /* Public */,
      15,    1,  106,    2, 0x0a /* Public */,
      16,    1,  109,    2, 0x0a /* Public */,
      18,    0,  112,    2, 0x0a /* Public */,
      19,    1,  113,    2, 0x0a /* Public */,

 // slots: parameters
    QMetaType::Void, QMetaType::Bool,    3,
    QMetaType::Void, QMetaType::Double,    5,
    QMetaType::Void, QMetaType::Double,    7,
    QMetaType::Void, QMetaType::Double,    9,
    QMetaType::Void, QMetaType::Double,    5,
    QMetaType::Void, QMetaType::Double,    7,
    QMetaType::Void, QMetaType::Double,    9,
    QMetaType::Void, QMetaType::Double,    5,
    QMetaType::Void, QMetaType::Double,    7,
    QMetaType::Void, QMetaType::Double,    9,
    QMetaType::Void, QMetaType::Int,   17,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 20,   21,

       0        // eod
};

void NGLScene::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        NGLScene *_t = static_cast<NGLScene *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->toggleWireframe((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 1: _t->setXRotation((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 2: _t->setYRotation((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 3: _t->setZRotation((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 4: _t->setXScale((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 5: _t->setYScale((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 6: _t->setZScale((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 7: _t->setXPosition((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 8: _t->setYPosition((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 9: _t->setZPosition((*reinterpret_cast< double(*)>(_a[1]))); break;
        case 10: _t->setObjectMode((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 11: _t->setColour(); break;
        case 12: _t->LoadOBJ((*reinterpret_cast< ngl::ShaderLib*(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject NGLScene::staticMetaObject = {
    { &QOpenGLWidget::staticMetaObject, qt_meta_stringdata_NGLScene.data,
      qt_meta_data_NGLScene,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *NGLScene::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *NGLScene::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_NGLScene.stringdata0))
        return static_cast<void*>(const_cast< NGLScene*>(this));
    return QOpenGLWidget::qt_metacast(_clname);
}

int NGLScene::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QOpenGLWidget::qt_metacall(_c, _id, _a);
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
