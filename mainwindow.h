#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <ControlCAN.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    void on_connect_clicked();

    void on_close_clicked();

    void on_send_clicked();

    void sendData(unsigned ID, unsigned char data[8]);

    void recieve();

    void reset();
    void gotoZero();
    void setRPDO3();
    void setRPDO1();
    void setTPDO3();
    void setTPDO1();
    void setBasicParams();
    void guideStart();
    void start(unsigned char data[8]);

    void on_enable_clicked();

    void on_stop_clicked();

    void on_running_clicked();

    void on_reset_clicked();

    void on_gotozero_clicked();

    void on_PDO_clicked();

    void on_basic_clicked();

    void on_gotozero_2_clicked();

    void on_reset_2_clicked();

    void on_running_3_clicked();

    void on_running_2_clicked();

private:
    Ui::MainWindow *ui;
    DWORD DeviceType = 4;
    DWORD DeviceInd = 0;
    DWORD CANInd = 0;
    DWORD Reserved = 1;
    VCI_CAN_OBJ vco;
};

#endif // MAINWINDOW_H
