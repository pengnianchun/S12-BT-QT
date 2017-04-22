#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui>
#include <QMainWindow>
#include <QFileDialog>
#include <QMessageBox>
#include <QProgressDialog>
#include "CANBootloader.h"
#include "controlcan.h"
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
    int CAN_GetBaudRateNum(unsigned int BaudRate);
    bool DeviceConfig(void);
    uint16_t crc16_ccitt(const uint8_t *buf, uint32_t len);
    void sendData(int ID, char* msg, uint length);
    
private slots:
    void on_openFirmwareFilePushButton_clicked();

    void on_updateFirmwarePushButton_clicked();

    void on_openFirmwareFileAction_triggered();

    void on_scanNodeAction_triggered();

    void on_executeFirmwarePushButton_clicked();

    void on_setbaudRatePushButton_clicked();

    void on_contactUsAction_triggered();

    void on_aboutAction_triggered();

    void on_exitAction_triggered();

    void on_actionCAN_BaudRateCalculator_triggered();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
