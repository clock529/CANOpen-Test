#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <ControlCAN.h>
#include <QLibrary>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    on_close_clicked();
    delete ui;
}


void MainWindow::on_connect_clicked()
{

    vco.SendType=0;
    vco.RemoteFlag=0;
    vco.ExternFlag=0;
    vco.DataLen=8;
    DWORD dwRel;
    dwRel = VCI_OpenDevice(DeviceType, DeviceInd, Reserved);
    qDebug() << "open" << dwRel;
    dwRel = VCI_ClearBuffer(DeviceType, DeviceInd, CANInd);
    qDebug() << "clear" << dwRel;
    VCI_INIT_CONFIG vic;
    vic.AccCode=0;
    vic.AccMask=0xffffffff;
    vic.Filter=0;
    vic.Mode=0;
    vic.Timing0 = 0;
    vic.Timing1 = 0x1c;
    dwRel = VCI_InitCAN(DeviceType, DeviceInd, CANInd, &vic);
    qDebug() << "init" << dwRel;
    dwRel = VCI_StartCAN(DeviceType, DeviceInd, CANInd);
    qDebug() << "start" << dwRel;
}

void MainWindow::on_close_clicked()
{
    DWORD dwRel = VCI_CloseDevice(DeviceType, DeviceInd);
    qDebug() << "close" << dwRel;
}

void MainWindow::on_send_clicked()
{
    UINT ID = 0x00;
    unsigned char data[8] = {0x2f, 0x60, 0x60, 0x00, 0x06, 0x00, 0x00, 0x00};
    sendData(ID, data);
}


void MainWindow::sendData(UINT ID, unsigned char data[8])
{
    DWORD dwRel;
    vco.ID = ID;
    for (int i=0; i < 8; i++)
    {
        vco.Data[i]=data[i];
    }
    dwRel = VCI_Transmit(DeviceType, DeviceInd, CANInd, &vco, 1);
    qDebug()<<"send: " << dwRel;
    Sleep(10);
    recieve();
}

void MainWindow::recieve()
{
    VCI_CAN_OBJ receiveData;

    //接收
    ULONG res = 0;

    //获取缓冲区中接收但尚未被读取的帧数
    res = VCI_Receive(DeviceType, DeviceInd, 0, &receiveData,1,10);
    if (res <= 0)
    {
        VCI_ERR_INFO vei;
        VCI_ReadErrInfo(DeviceType,DeviceInd,CANInd,&vei);
        qDebug()<<"Read Data failed"<<"Error Data:"<<QString::number(vei.ErrCode,16);
        return;
    }
    else
    {
        qDebug()<<"receive: " << res;
        qDebug()<<"帧ID: "<<QString::number(receiveData.ID,16);
        qDebug()<<"帧数据: "<<QString::number(receiveData.Data[0],16)<<QString::number(receiveData.Data[1],16);
        qDebug()<<QString::number(receiveData.Data[2],16)<<QString::number(receiveData.Data[3],16);
        qDebug()<<QString::number(receiveData.Data[4],16)<<QString::number(receiveData.Data[5],16);
        qDebug()<<QString::number(receiveData.Data[6],16)<<QString::number(receiveData.Data[7],16);
        qDebug()<<"帧长度: "<<receiveData.DataLen;
        recieve();
    }
}

void MainWindow::on_enable_clicked()
{
    reset();
    //gotoZero();
    setRPDO3();
    setRPDO1();
    setTPDO3();
    setTPDO1();
    setBasicParams();
    guideStart();
}
void MainWindow::on_running_clicked()
{

   // double joint = ui->jointAngle->text().toDouble();
    unsigned char data[8] = {0xa4, 0x4d, 0x00, 0x00, 0x90, 0xb1, 0x00, 0x00};
    unsigned char data1[8] = {0x00};
//    unsigned char data[8] = {0x00, 0x00, 0x03, 0xe8, 0x00, 0x00, 0x10, 0x00};
    INT64 joint = -19876;
    INT64 jointV = 45456;
    if(joint < 0)
        joint = 4294967295 + joint + 1;
    if(jointV < 0)
        jointV = 4294967295 + jointV + 1;
    for (int i = 0; i < 4; i++)
    {
        data1[i] = (joint % 256);
        data1[i + 4] = jointV % 256;
        jointV = jointV / 256;
        joint = joint / 256;
        if (joint == 0 && jointV == 0)
            break;
    }
    start(data);
}

//第二步复位节点：
//通过NMT复位节点，数据格式为[0 : 81 06 00 00 00 00 00 00];
//通过NMT启动节点，数据格式为[0 : 01 06 00 00 00 00 00 00];
void MainWindow::reset()
{
    unsigned int id = 0x000;
    unsigned char data[8] = {0x81, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    sendData(id, data);
    Sleep(1000);
    data[0] = 0x01;
    sendData(id, data);
    qDebug()<<"复位";
}

//第三步回零（用于归零电机的位置数据）：
//设置控制模式（6060-00h）为回零模式，数据格式为[606 : 2f 60 60 00 06 00 00 00];
//设置回零方式（6098-00h）为0x23，数据格式为[606 : 2f 98 60 00 23 00 00 00];
//设置控制字（6040-00h）伺服准备好，数据格式为[606 : 2b 40 60 00 06 00 00 00];
//设置控制字（6040-00h）等待打开伺服使能，数据格式为[606 : 2b 40 60 00 07 00 00 00];
//设置控制字（6040-00h）伺服运行，数据格式为[606 : 2b 40 60 00 0f 00 00 00];
//设置控制字（6040-00h）更新指令，数据格式为[606 : 2b 40 60 00 1f 00 00 00];
//设置控制字（6040-00h）伺服准备好，数据格式为[606 : 2b 40 60 00 06 00 00 00];
void MainWindow::gotoZero()
{
    unsigned int id = 0x606;
    unsigned char data[8] = {0x2f, 0x60, 0x60, 0x00, 0x06, 0x00, 0x00, 0x00};
    sendData(id, data);
    data[1] = 0x98; data[4] = 0x23;
    sendData(id, data);
    data[0] = 0x2b; data[1] = 0x40; data[4] = 0x06;
    sendData(id, data);
    data[4] = 0x07;
    sendData(id, data);
    data[4] = 0x0f;
    sendData(id, data);
    data[4] = 0x1f;
    sendData(id, data);
    data[4] = 0x06;
    sendData(id, data);
    qDebug()<<"回零";
}

//将RPDO3接收的数据配置为目标位置和轮廓速度：
//关闭RPDO3（1402-01h），数据格式为[606 : 23 02 14 01 06 04 00 80];
//清除RPDO3（1602-00h）的映射内容，数据格式为[606 : 2f 02 16 00 00 00 00 00];
//将目标位置参数（607A-00h）映射到RPDO3的第1个子索引上（1602-01h），数据格式为[606 : 23 02 16 01 20 00 7a 60];
//将轮廓速度参数（6081-00h）映射到RPDO3的第2个子索引上（1602-02h），数据格式为[606 : 23 02 16 02 20 00 81 60];
//使能RPDO3（1602-00h）的映射，数据格式为[606 : 2f 02 16 00 02 00 00 00];
//打开RPDO3（1402-01h），数据格式为[606 : 23 02 14 01 06 04 00 00];
void MainWindow::setRPDO3()
{
    unsigned int id = 0x606;
    unsigned char data[8] = {0x23, 0x02, 0x14, 0x01, 0x06, 0x04, 0x00, 0x80};
    sendData(id, data);
    data[0] = 0x2f; data[2] = 0x16; data[3] = 0x00; data[4] = 0x00; data[5] = 0x00; data[7] = 0x00;
    sendData(id, data);
    data[0] = 0x23; data[3] = 0x01; data[4] = 0x20; data[6] = 0x7a; data[7] = 0x60;
    sendData(id, data);
    data[3] = 0x02; data[6] = 0x81;
    sendData(id, data);
    data[0] = 0x2f; data[3] = 0x00; data[4] = 0x02; data[6] = 0x00; data[7] = 0x00;
    sendData(id, data);
    data[0] = 0x23; data[2] = 0x14; data[3] = 0x01; data[4] = 0x06; data[5] = 0x04;
    sendData(id, data);
    qDebug()<<"RPDO3配置";
}

//将RPDO1接收的数据配置为控制字：
//关闭RPDO1（1400-01h），数据格式为[606 : 23 00 14 01 06 02 00 80];
//清除RPDO1（1600-00h）的映射内容，数据格式为[606 : 2f 00 16 00 00 00 00 00];
//将控制字参数（6040-00h）映射到RPDO1的第1个子索引上（1600-01h），数据格式为[606 : 23 00 16 01 10 00 40 60];
//使能RPDO1（1600-00h）的映射，数据格式为[606 : 2f 00 16 00 01 00 00 00];
//打开RPDO1（1400-01h），数据格式为[606 : 23 00 14 01 06 02 00 00];
void MainWindow::setRPDO1()
{
    unsigned int id = 0x606;
    unsigned char data[8] = {0x23, 0x00, 0x14, 0x01, 0x06, 0x02, 0x00, 0x80};
    sendData(id, data);
    data[0] = 0x2f; data[2] = 0x16; data[3] = 0x00; data[4] = 0x00; data[5] = 0x00; data[7] = 0x00;
    sendData(id, data);
    data[0] = 0x23; data[3] = 0x01; data[4] = 0x10; data[6] = 0x40; data[7] = 0x60;
    sendData(id, data);
    data[0] = 0x2f; data[3] = 0x00; data[4] = 0x01; data[6] = 0x00; data[7] = 0x00;
    sendData(id, data);
    data[0] = 0x23; data[2] = 0x14; data[3] = 0x01; data[4] = 0x06; data[5] = 0x02;
    sendData(id, data);
    qDebug()<<"RPDO1配置";
}

//将TPDO3发送的数据配置为位置反馈和速度反馈：
//关闭TPDO3（1802-01h），数据格式为[606 : 23 02 18 01 86 03 00 c0];
//清除TPDO3（1a02-00h）的映射内容，数据格式为[606 : 2f 02 1a 00 00 00 00 00];
//将位置反馈参数（6064-00h）映射到TPDO3的第1个子索引上（1a02-01h），数据格式为[606 : 23 02 1a 01 20 00 64 60];
//将轮廓速度参数（606c-00h）映射到TPDO3的第2个子索引上（1a02-02h），数据格式为[606 : 23 02 1a 02 20 00 6c 60];
//使能TPDO3（1a02-00h）的映射，数据格式为[606 : 2f 02 1a 00 02 00 00 00];
//打开TPDO3（1802-01h），数据格式为[606 : 23 02 18 01 86 03 00 40];
void MainWindow::setTPDO3()
{
    unsigned int id = 0x606;
    unsigned char data[8] = {0x23, 0x02, 0x18, 0x01, 0x86, 0x03, 0x00, 0xc0};
    sendData(id, data);
    data[0] = 0x2f; data[2] = 0x1a; data[3] = 0x00; data[4] = 0x00; data[5] = 0x00; data[7] = 0x00;
    sendData(id, data);
    data[0] = 0x23; data[3] = 0x01; data[4] = 0x20; data[6] = 0x64; data[7] = 0x60;
    sendData(id, data);
    data[3] = 0x02; data[6] = 0x6c;
    sendData(id, data);
    data[0] = 0x2f; data[3] = 0x00; data[4] = 0x02; data[6] = 0x00; data[7] = 0x00;
    sendData(id, data);
    data[0] = 0x23; data[2] = 0x18; data[3] = 0x01; data[4] = 0x86; data[5] = 0x03; data[7] = 0x40;
    sendData(id, data);
    qDebug()<<"TPDO3配置";
}

//将TPDO1发送的数据配置为状态字：
//关闭TPDO1（1800-01h），数据格式为[606 : 23 00 18 01 86 01 00 c0];
//清除TPDO1（1a00-00h）的映射内容，数据格式为[606 : 2f 00 1a 00 00 00 00 00];
//将状态字（6061-00h）映射到TPDO1的第1个子索引上（1a00-01h），数据格式为[606 : 23 00 1a 01 10 00 61 60];
//使能TPDO1（1a00-00h）的映射，数据格式为[606 : 2f 00 1a 00 01 00 00 00];
//打开TPDO1（1800-01h），数据格式为[606 : 23 00 18 01 86 01 00 40];
void MainWindow::setTPDO1()
{
    unsigned int id = 0x606;
    unsigned char data[8] = {0x23, 0x00, 0x18, 0x01, 0x86, 0x01, 0x00, 0xc0};
    sendData(id, data);
    data[0] = 0x2f; data[2] = 0x1a; data[3] = 0x00; data[4] = 0x00; data[5] = 0x00; data[7] = 0x00;
    sendData(id, data);
    data[0] = 0x23; data[3] = 0x01; data[4] = 0x10; data[6] = 0x61; data[7] = 0x60;
    sendData(id, data);
    data[0] = 0x2f; data[3] = 0x00; data[4] = 0x01; data[6] = 0x00; data[7] = 0x00;
    sendData(id, data);
    data[0] = 0x23; data[2] = 0x18; data[3] = 0x01; data[4] = 0x86; data[5] = 0x01; data[7] = 0x40;
    sendData(id, data);
    qDebug()<<"TPDO1配置";
}

//第五步基本参数配置：
//1）	设置转换因子（包括位置因子、转速编码器因子、转速因子1、加速度因子以及指令极性，此处只修改位置因子，其他因子采用默认值）：
//设置位置因子的分子部分（6093-01h）为0xa0，数据格式为[606 : 23 93 60 01 f0 00 00 00];
//设置位置因子的分母部分（6093-02h）0x01，数据格式为[606 : 23 93 60 02 01 00 00 00];
//其他转换因子的设置跟位置因子类似；
//2）	配置运行模式：
//设置控制模式（6060-00h）为轮廓位置模式，数据格式为[606 : 2f 60 60 00 01 00 00 00];
void MainWindow::setBasicParams()
{
    unsigned int id = 0x606;
    unsigned char data[8] = {0x23, 0x93, 0x60, 0x01, 0xf0, 0x00, 0x00, 0x00};
    sendData(id, data);
    data[3] = 0x02; data[4] = 0x01;
    sendData(id, data);
    data[0] = 0x2f; data[1] = 0x60; data[3] = 0x00;
    sendData(id, data);
    qDebug()<<"基本参数配置";
}

//启动引导（因为之前已经做过PDO配置，所以此处直接使用PDO发送数据）：
//设置控制字（6040-00h）伺服准备好，数据格式为[206 : 06 00 00 00 00 00 00 00];
//设置控制字（6040-00h）等待打开伺服使能，数据格式为[206 : 07 00 00 00 00 00 00 00];
void MainWindow::guideStart()
{
    unsigned id = 0x206;
    unsigned char data[8] = {0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    sendData(id, data);
    data[0] = 0x07;
    sendData(id, data);
    qDebug()<<"引导启动";
}

//启动运行（绝对位置立即更新）
//设置目标位置（607a-00h）和轮廓速度（6081-00h），数据格式为[406 : 00 00 10 00 e8 03 00 00]（其中前四个字节表示目标位置，后四个字节表示轮廓速度）；
//设置控制字（6040-00h）为伺服运行，数据格式为[206 : 2f 00 00 00 00 00 00 00];
//设置控制字（6040-00h）为指令更新，数据格式为[206 : 3f 00 00 00 00 00 00 00];
void MainWindow::start(unsigned char data[8])
{
    unsigned int id = 0x406;
    sendData(id, data);
    id = 0x206;
    unsigned char data2[8] = {0x2f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    sendData(id, data2);
    data2[0] = 0x3f;
    sendData(id, data2);
    qDebug()<<"启动";
}

//停止运行：
//设置控制字（6040-00h）伺服准备好，数据格式为[206 : 06 00 00 00 00 00 00 00]即可立即停止运行。
void MainWindow::on_stop_clicked()
{
    unsigned int id = 0x206;
    unsigned char data[8] = {0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    sendData(id, data);
    qDebug()<<"停止";
}


void MainWindow::on_reset_clicked()
{
    reset();
}

void MainWindow::on_gotozero_clicked()
{
    gotoZero();
}

void MainWindow::on_PDO_clicked()
{
    setRPDO3();
    setRPDO1();
    setTPDO3();
    setTPDO1();
}

void MainWindow::on_basic_clicked()
{
    setBasicParams();
}

void MainWindow::on_gotozero_2_clicked()
{
    guideStart();
}

void MainWindow::on_reset_2_clicked()
{
    unsigned int id = 0x000;
    unsigned char data[8] = {0x01, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    sendData(id, data);

    qDebug()<<"复位2";
}

void MainWindow::on_running_3_clicked()
{
    unsigned id = 0x206;
    unsigned char data2[8] = {0x6f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    sendData(id, data2);
    qDebug()<<"启动2";
}

void MainWindow::on_running_2_clicked()
{
    unsigned id = 0x206;
    unsigned char data2[8] = {0x7f, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    sendData(id, data2);
    qDebug()<<"启动3";
}
