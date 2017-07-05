#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QDateTime>
#include <QTimer>
#include <QDebug>

#include "canhelper.h"

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
    void on_pushButton_open_clicked();

    void on_pushButton_sync_clicked();

    void on_pushButton_close_clicked();

    void onTimerOut();

    void on_comboBox_currentIndexChanged(int index);

    void on_pushButton_sync_2_clicked();

    void on_pushButton_sync_3_clicked();

private:
    Ui::MainWindow *ui;
    QTimer*  timer;


    qint64 m_DevIndex = 0;
    qint64 m_nDevType = 1;
    qint64 m_nCanIndex = 0;
    DWORD m_DevType = VCI_USBCAN2;
    DWORD Reserved = 0;

    bool displayFlag = false;
    qint64 deviceState = 0;
    qint64 stepCount = 0;
    CanHelper *helper;
};

#endif // MAINWINDOW_H
