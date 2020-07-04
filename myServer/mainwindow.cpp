#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDateTime>
#include <QDataStream>
#include <QString>
#include <QMessageBox>
#include <QtSql>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    setWindowTitle("UDP Server");
    ui->setupUi(this);
    my_udp = new QUdpSocket(this);
    db = QSqlDatabase::addDatabase("QPSQL");
    db.setHostName("localhost");
    db.setDatabaseName("Server database");
    db.setUserName("postgres");
    db.setPassword("Cler0323");
    bool checker = db.open();
    if (!checker)
        qDebug() <<db.lastError().text();
    else
        qDebug() <<"Success!";
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{
    QString myData =ui->textEdit->toPlainText();
    QDateTime dt = QDateTime::currentDateTime();
    QString date = dt.toString("dd.MM.yyyy HH:mm:ss");
    QSqlQuery writeMessage = QSqlQuery(db);
    writeMessage.prepare("SELECT * FROM public.write_message(:_date, :_message);");
    writeMessage.bindValue(":_date",date);
    writeMessage.bindValue(":_message",myData);
    myData = "    " + myData;
    QByteArray myDatagram;
    myDatagram+=myData;
    QDataStream out(&myDatagram, QIODevice::WriteOnly);
    out << myDatagram;
    my_udp->writeDatagram(myDatagram, QHostAddress::LocalHost, 2424);
    myData = QString::fromStdString(myDatagram.toStdString());
    QMessageBox::information(this, "..", myData);
    bool checker = writeMessage.exec();
    if (!checker)
        QMessageBox::about(this, "Error", writeMessage.lastError().text());

}

void MainWindow::on_pushButton_2_clicked()
{
    myModel = new QSqlTableModel(this,db);
    myModel->setTable("mytable");
    myModel->select();
    ui->tableView->setModel(myModel);

}
