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
}

Dialog::~Dialog()
{
    delete ui;
}


void Dialog::on_testBut_clicked()
{

    QStringList arguments;
    arguments << "devices";

    AdbProcess *myProcess = new AdbProcess(this);
    connect(myProcess, &AdbProcess::adbProcessResult, this, [](AdbProcess::ADB_EXEC_RESULT processResult){
        qDebug() << ">>>>>>" << processResult;
    });
    myProcess->execute("", arguments);
}
