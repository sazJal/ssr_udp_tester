#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QUdpSocket>
#include <QMessageBox>
#include <QDebug>
#include <QDateTime>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void sayHello();
    QHostAddress hostAddress;


private:
    Ui::MainWindow *ui;
    QUdpSocket *socketSend;
    QUdpSocket *socketReceive;
    quint16 remotePort;
    QString remoteIp;
    bool buttonOnIsActive, buttonOffIsActive;

    void ConnectIP();
    void CharIPCheck();
    void CharPortCheck();
    void writeDatagram();
    void modeDecode();
    void clearTextBrowser();

private slots:
     void readPendingDatagrams();


};
#endif // MAINWINDOW_H
