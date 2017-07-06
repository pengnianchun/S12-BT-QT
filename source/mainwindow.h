#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QProgressDialog>
#include <QFileDialog>
#include <QDateTime>
#include <QDebug>
#include "canhelper.h"
#include "devaddrinputdialog.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_comboBox_selectCan_currentIndexChanged(int index);

    void on_openFirmwareFileAction_triggered();

    void on_scanNodeAction_triggered();

    void on_clearDisplayAction_triggered();

    void on_aboutAction_triggered();

    void on_exitAction_triggered();

    void on_pushButton_openCan_clicked();

    void on_pushButton_closeCan_clicked();

    void on_pushButton_scanNode_clicked();

    void on_openFirmwareFilePushButton_clicked();

    void on_updateFirmwarePushButton_clicked();

    void on_executeFirmwarePushButton_clicked();

    void outputInformation(QString log);

private:
    Ui::MainWindow *ui;

    qint64 m_DevIndex = 0;
    qint64 m_nDevType = 1;
    qint64 m_nCanIndex = 0;
    DWORD m_DevType = VCI_USBCAN2;
    DWORD Reserved = 0;
    int node_addr = 0;

    bool displayFlag = false;
    qint64 deviceState = 0;
    qint64 stepCount = 0;
    CanHelper *helper;
    QLabel  *labelTag;
};

#endif // MAINWINDOW_H
