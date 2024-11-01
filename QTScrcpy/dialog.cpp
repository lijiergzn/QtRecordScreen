//#include<QProcess>
#include"adbprocess.h"
#include "dialog.h"
#include "ui_dialog.h"
#include<QDebug>

Dialog::Dialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialog)
{
    ui->setupUi(this);
    connect(&m_server, &server::serverStartResult, this,[](bool success){
        qDebug()<<"server start"<<success;
    });

    connect(&m_server, &server::connectToResult, this,[](bool success){
        qDebug()<<"connectToResult"<<success;
    });
}

Dialog::~Dialog()
{
    delete ui;
}


void Dialog::on_testBut_clicked()
{

    m_server.start("", 27183, 720, 8000000);
}

void Dialog::on_stopBut_clicked()
{
    m_server.stop();
}
