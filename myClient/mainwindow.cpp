#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QUdpSocket>
#include <QMessageBox>
#include <QString>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QTimer>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    my_udp = new QUdpSocket(this);
    my_udp->bind(2424);
    connect(my_udp, SIGNAL(readyRead()), SLOT(read()));
    mySystemTrayIcon = new QSystemTrayIcon(this);
    mySystemTrayIcon->setVisible(true);
    QTimer *timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()), SLOT(on_pushButton_clicked()));
    timer->start(100);


}


MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::on_pushButton_clicked()
{
    QByteArray myDatagram;
    do
    {
        if (my_udp->hasPendingDatagrams() == true)
        {
            myDatagram.resize(my_udp->pendingDatagramSize());
            my_udp->readDatagram(myDatagram.data(),myDatagram.size());
            QString myString = QString::fromStdString(myDatagram.toStdString());
            const char* str = myString.toStdString().c_str();
            //QMessageBox::information(this, "..", myString);
            mySystemTrayIcon->showMessage(tr("Server"), myString);
        }
    } while (my_udp->hasPendingDatagrams());
}
