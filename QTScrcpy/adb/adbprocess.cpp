#include "adbprocess.h"
#include <QDebug>

QString AdbProcess::s_adbPath="";

AdbProcess::AdbProcess(QObject *parent )
    :QProcess(parent)
{

    initSignals();
}

QString AdbProcess::getAdbPath()
{
    if(s_adbPath.isEmpty()){
        s_adbPath = QString::fromLocal8Bit(qgetenv("QTSCRCPY_ADB_PATH"));

    }
    return s_adbPath;
}

// 执行adb启动函数
void AdbProcess::execute(const QString &serial, const QStringList &args)
{
    // serial 是序列号，args 是具体的命令 如devices
    //如 push C:\Users\YourName\Documents\example.txt /sdcard/
    QStringList adbArgs;
    if(!serial.isEmpty()){
        adbArgs << "-s" <<serial;
    }
    adbArgs << args;
    start(getAdbPath(), adbArgs);
}

void AdbProcess::push(const QString &serial, const QString &local, const QString &remote)
{
    QStringList adbArgs;
    adbArgs << "push";
    adbArgs << local;
    adbArgs << remote;
    execute(serial, adbArgs);
}

void AdbProcess::remotePath(const QString &serial, const QString &remote)
{
    QStringList adbArgs;
    adbArgs << "shell";
    adbArgs << "rm";
    adbArgs << remote;
    execute(serial, adbArgs);
}

void AdbProcess::reverse(const QString &serial, const QString &deviceSocketName, quint16 localPort)

{
    QStringList adbArgs;
    adbArgs << "reverse";
    adbArgs << QString("localabstract:%1").arg(deviceSocketName);
    adbArgs << QString("tcp:%1").arg(localPort);
    execute(serial, adbArgs);
}
void AdbProcess::reverseRemove(const QString &serial, const QString &deviceSocketName)
{
    QStringList adbArgs;
    adbArgs << "reverse";
    adbArgs << "--remove";
    adbArgs << QString("localabstract:%1").arg(deviceSocketName);
    execute(serial, adbArgs);
}

void AdbProcess::initSignals()
{
    connect(this,&QProcess::errorOccurred,this,[](QProcess::ProcessError error){
        qDebug()<<error;
    });
    connect(this, static_cast<void(QProcess::*)(int, QProcess::ExitStatus)>(&QProcess::finished),
        this,[](int exitCode, QProcess::ExitStatus exitStatus){
        qDebug()<<exitCode<<exitStatus;
    });

    connect(this,&QProcess::readyReadStandardError,this,[this](){
        qDebug()<<readAllStandardError();
    });

    connect(this,&QProcess::readyReadStandardOutput,this,[this](){
        qDebug()<<readAllStandardOutput();
    });

    connect(this,&QProcess::started,this,[](){
        qDebug()<<"started";
    });

}
