/****************************************************************************
** Meta object code from reading C++ file 'mainwindow.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.8.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../source/mainwindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'mainwindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.8.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_MainWindow_t {
    QByteArrayData data[12];
    char stringdata0[340];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MainWindow_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MainWindow_t qt_meta_stringdata_MainWindow = {
    {
QT_MOC_LITERAL(0, 0, 10), // "MainWindow"
QT_MOC_LITERAL(1, 11, 37), // "on_openFirmwareFilePushButton..."
QT_MOC_LITERAL(2, 49, 0), // ""
QT_MOC_LITERAL(3, 50, 35), // "on_updateFirmwarePushButton_c..."
QT_MOC_LITERAL(4, 86, 35), // "on_openFirmwareFileAction_tri..."
QT_MOC_LITERAL(5, 122, 27), // "on_scanNodeAction_triggered"
QT_MOC_LITERAL(6, 150, 36), // "on_executeFirmwarePushButton_..."
QT_MOC_LITERAL(7, 187, 32), // "on_setbaudRatePushButton_clicked"
QT_MOC_LITERAL(8, 220, 28), // "on_contactUsAction_triggered"
QT_MOC_LITERAL(9, 249, 24), // "on_aboutAction_triggered"
QT_MOC_LITERAL(10, 274, 23), // "on_exitAction_triggered"
QT_MOC_LITERAL(11, 298, 41) // "on_actionCAN_BaudRateCalculat..."

    },
    "MainWindow\0on_openFirmwareFilePushButton_clicked\0"
    "\0on_updateFirmwarePushButton_clicked\0"
    "on_openFirmwareFileAction_triggered\0"
    "on_scanNodeAction_triggered\0"
    "on_executeFirmwarePushButton_clicked\0"
    "on_setbaudRatePushButton_clicked\0"
    "on_contactUsAction_triggered\0"
    "on_aboutAction_triggered\0"
    "on_exitAction_triggered\0"
    "on_actionCAN_BaudRateCalculator_triggered"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MainWindow[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    0,   64,    2, 0x08 /* Private */,
       3,    0,   65,    2, 0x08 /* Private */,
       4,    0,   66,    2, 0x08 /* Private */,
       5,    0,   67,    2, 0x08 /* Private */,
       6,    0,   68,    2, 0x08 /* Private */,
       7,    0,   69,    2, 0x08 /* Private */,
       8,    0,   70,    2, 0x08 /* Private */,
       9,    0,   71,    2, 0x08 /* Private */,
      10,    0,   72,    2, 0x08 /* Private */,
      11,    0,   73,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        MainWindow *_t = static_cast<MainWindow *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->on_openFirmwareFilePushButton_clicked(); break;
        case 1: _t->on_updateFirmwarePushButton_clicked(); break;
        case 2: _t->on_openFirmwareFileAction_triggered(); break;
        case 3: _t->on_scanNodeAction_triggered(); break;
        case 4: _t->on_executeFirmwarePushButton_clicked(); break;
        case 5: _t->on_setbaudRatePushButton_clicked(); break;
        case 6: _t->on_contactUsAction_triggered(); break;
        case 7: _t->on_aboutAction_triggered(); break;
        case 8: _t->on_exitAction_triggered(); break;
        case 9: _t->on_actionCAN_BaudRateCalculator_triggered(); break;
        default: ;
        }
    }
    Q_UNUSED(_a);
}

const QMetaObject MainWindow::staticMetaObject = {
    { &QMainWindow::staticMetaObject, qt_meta_stringdata_MainWindow.data,
      qt_meta_data_MainWindow,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_MainWindow.stringdata0))
        return static_cast<void*>(const_cast< MainWindow*>(this));
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 10)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 10;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
