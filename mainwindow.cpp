#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->button_send->setEnabled(false);
    ui->button_on->setEnabled(false);
    ui->button_off->setEnabled(false);
    socketSend      = new QUdpSocket(this);
    socketReceive   = new QUdpSocket(this);
    connect(ui->line_ip1, &QLineEdit::textEdited, this, &MainWindow::CharIPCheck);
    connect(ui->line_ip2, &QLineEdit::textEdited, this, &MainWindow::CharIPCheck);
    connect(ui->line_ip3, &QLineEdit::textEdited, this, &MainWindow::CharIPCheck);
    connect(ui->line_ip4, &QLineEdit::textEdited, this, &MainWindow::CharIPCheck);
    connect(ui->line_port, &QLineEdit::textEdited, this, &MainWindow::CharPortCheck);

    connect(ui->button_connect, &QPushButton::clicked, this, &MainWindow::ConnectIP);
    connect(ui->button_clear, &QPushButton::clicked, this, &MainWindow::clearTextBrowser);

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::writeDatagram()
{
    QByteArray Data;
    QString mode;
    QString tx_status;
    QString time_format = "yyyy-MM-dd  HH:mm:ss";
    QDateTime a = QDateTime::currentDateTime();

    QString as = a.toString(time_format);


    Data.append("Hello world");
    if(ui->check_1->isChecked() && ui->check_2->isChecked() && ui->check_3->isChecked() && ui->check_C->isChecked()){Data[1] = 'F';        mode = QStringLiteral("mode 1 mode 2 mode 3A mode C");}
    else if(!ui->check_1->isChecked() && ui->check_2->isChecked() && ui->check_3->isChecked() && ui->check_C->isChecked()){Data[1] = 'E';  mode = QStringLiteral("mode 2 mode 3A mode C");}
    else if(ui->check_1->isChecked() && !ui->check_2->isChecked() && ui->check_3->isChecked() && ui->check_C->isChecked()){Data[1] = 'D';  mode = QStringLiteral("mode 1 mode 3A mode C");}
    else if(!ui->check_1->isChecked() && !ui->check_2->isChecked() && ui->check_3->isChecked() && ui->check_C->isChecked()){Data[1] = 'C'; mode = QStringLiteral("mode 3A mode C");}

    else if(ui->check_1->isChecked() && ui->check_2->isChecked() && !ui->check_3->isChecked() && ui->check_C->isChecked()){Data[1] = 'B';   mode = QStringLiteral("mode 1 mode 2 mode C");}
    else if(!ui->check_1->isChecked() && ui->check_2->isChecked() && !ui->check_3->isChecked() && ui->check_C->isChecked()){Data[1] = 'A';  mode = QStringLiteral("mode 2 mode C");}
    else if(ui->check_1->isChecked() && !ui->check_2->isChecked() && !ui->check_3->isChecked() && ui->check_C->isChecked()){Data[1] = '9';  mode = QStringLiteral("mode 1 mode C");}
    else if(!ui->check_1->isChecked() && !ui->check_2->isChecked() && !ui->check_3->isChecked() && ui->check_C->isChecked()){Data[1] = '8';mode = QStringLiteral("mode C");}

    else if(ui->check_1->isChecked() && ui->check_2->isChecked() && ui->check_3->isChecked() && !ui->check_C->isChecked()){Data[1] = '7';   mode = QStringLiteral("mode 1 mode 2 mode 3A");}
    else if(!ui->check_1->isChecked() && ui->check_2->isChecked() && ui->check_3->isChecked() && !ui->check_C->isChecked()){Data[1] = '6';  mode = QStringLiteral("mode 2 mode 3A");}
    else if(ui->check_1->isChecked() && !ui->check_2->isChecked() && ui->check_3->isChecked() && !ui->check_C->isChecked()){Data[1] = '5';  mode = QStringLiteral("mode 1 mode 3A");}
    else if(!ui->check_1->isChecked() && !ui->check_2->isChecked() && ui->check_3->isChecked() && !ui->check_C->isChecked()){Data[1] = '4'; mode = QStringLiteral("mode 2");}

    else if(ui->check_1->isChecked() && ui->check_2->isChecked() && !ui->check_3->isChecked() && !ui->check_C->isChecked()){Data[1] = '3';  mode = QStringLiteral("mode 1 mode 2");}
    else if(!ui->check_1->isChecked() && ui->check_2->isChecked() && !ui->check_3->isChecked() && !ui->check_C->isChecked()){Data[1] = '2'; mode = QStringLiteral("mode 2");}
    else if(ui->check_1->isChecked() && !ui->check_2->isChecked() && !ui->check_3->isChecked() && !ui->check_C->isChecked()){Data[1] = '1'; mode = QStringLiteral("mode 1");}
    else if(!ui->check_1->isChecked() && !ui->check_2->isChecked() && !ui->check_3->isChecked() && !ui->check_C->isChecked()){Data[1] = '0';mode = QStringLiteral("mode off");}
    if(buttonOnIsActive){Data[0] = 'y'; tx_status = QStringLiteral("TX_ON");}
    else if(buttonOffIsActive){Data[0] = 'n'; tx_status = QStringLiteral("TX_OFF"); mode = QStringLiteral("mode off");}

    ui->textB_cmd->append(as+"   "+tx_status+"  "+mode);

    hostAddress.setAddress(remoteIp);
    socketSend->writeDatagram(Data, hostAddress, remotePort);
}

void MainWindow::readPendingDatagrams()
{
    while (socketReceive->hasPendingDatagrams()) {
            QByteArray datagram;
            datagram.resize(socketReceive->pendingDatagramSize());
            socketReceive->readDatagram(datagram.data(), datagram.size());
    }
}

void MainWindow::ConnectIP()
{
    remotePort = ui->line_port->text().toUShort(nullptr,10);
    remoteIp.append(ui->line_ip4->text()+QStringLiteral(".")+ui->line_ip3->text()+QStringLiteral(".")+ui->line_ip2->text()+QStringLiteral(".")+ui->line_ip1->text());

//    qDebug() << QString::number(port);
//    qDebug() << ip;

    socketReceive->bind(QHostAddress::LocalHost, 25000);
    connect(socketReceive,SIGNAL(readyRead()),this,SLOT(readPendingDatagrams()));
    connect(ui->button_send, &QPushButton::clicked, this, &MainWindow::writeDatagram);
    ui->button_send->setEnabled(true);
    connect(ui->button_on, &QPushButton::clicked, this, &MainWindow::modeDecode);
    connect(ui->button_off, &QPushButton::clicked, this, &MainWindow::modeDecode);
    ui->button_on->setEnabled(true);
    ui->button_off->setEnabled(false);
    buttonOnIsActive = false;
    buttonOffIsActive = true;
}

void MainWindow::clearTextBrowser()
{
    ui->textB_cmd->clear();
}

void MainWindow::CharIPCheck()
{
    QLineEdit *line = (QLineEdit *)(sender());
    QString text = line->text();
    QMessageBox msgBox;
    int nbr;
    bool ok;

    nbr = text.toInt(&ok, 10);

    if(!ok)
    {
        msgBox.setText("Number Only");
        msgBox.exec();
        line->clear();
    }

    if((nbr > 255) || (nbr <0))
    {
        msgBox.setText("Exceed IP range!");
        msgBox.exec();
        line->clear();
    }

}

void MainWindow::CharPortCheck()
{
    QLineEdit *line = (QLineEdit *)(sender());
    QString text = line->text();
    QMessageBox msgBox;
    bool ok;
    int nbr;

    nbr = text.toInt(&ok, 10);

    if(!ok)
    {
        msgBox.setText("Number Only");
        msgBox.exec();
        line->clear();
    }

    if((nbr > 65535) || (nbr <0))
    {
        msgBox.setText("Exceed port range!");
        msgBox.exec();
        line->clear();
    }
}

void MainWindow::modeDecode()
{
    QPushButton *button = (QPushButton *)(sender());
    QString text = button->text();
    if(text == "ON")
    {
        if(!buttonOnIsActive)
        {
            buttonOnIsActive  = true;
            buttonOffIsActive = false;
            ui->button_on->setEnabled(false);
            ui->button_off->setEnabled(true);
        }
    }
    else if(text == "OFF")
    {
        if(!buttonOffIsActive)
        {
            buttonOnIsActive  = false;
            buttonOffIsActive = true;
            ui->button_on->setEnabled(true);
            ui->button_off->setEnabled(false);
        }
    }
}
