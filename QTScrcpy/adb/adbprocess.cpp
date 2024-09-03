#include "adbprocess.h"
#include <QDebug>


//static 静态成员变量，不能在类内进行初始化，必须要在类外进行初始化
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

QStringList AdbProcess::getDevicesSerialFromStdOut()
{
    QStringList serials;
    QStringList devicesInfoList = m_standardOutput.split(QRegExp("\r\n|\n"), QString::SkipEmptyParts);
    for(QString deviceInfo : devicesInfoList){
        QStringList deviceInfos = deviceInfo.split(QRegExp("\t"), QString::SkipEmptyParts);
        if(2 == deviceInfos.count() && 0 == deviceInfos[1].compare("device")){
            serials << deviceInfos[0];
        }
    }
    return serials;
}

QString AdbProcess::getDeviceIPFromStdOut()
{
    QString ip = "";
    QString strIPExp = "inet [\\d.]*";
    QRegExp ipRegExp(strIPExp, Qt::CaseInsensitive);
    if(ipRegExp.indexIn(m_standardOutput) != -1){
        ip = ipRegExp.cap(0);
        ip = ip.right(ip.size() -5);
    }
    return ip;
}

QString AdbProcess::getStdOut()
{
    return m_standardOutput;
}

QString AdbProcess::getRrrorOut()
{
    return m_errorOutput;
}

void AdbProcess::initSignals()
{
    connect(this,&QProcess::errorOccurred,this,[this](QProcess::ProcessError error){
        if(QProcess::FailedToStart == error){
            emit adbProcessResult(AER_ERROR_MISSING_BINARY);
        }else{
            emit adbProcessResult(AER_ERROR_START);
        }
        qDebug()<<error;
    });
    connect(this, static_cast<void(QProcess::*)(int, QProcess::ExitStatus)>(&QProcess::finished),
        this,[this](int exitCode, QProcess::ExitStatus exitStatus){
        if (NormalExit == exitStatus && 0 == exitCode) {
            emit adbProcessResult(AER_SUCCESS_EXEC);
        } else {
            emit adbProcessResult(AER_ERROR_EXEC);
        }
        qDebug()<<exitCode<<exitStatus;
    });

    connect(this,&QProcess::readyReadStandardError,this,[this](){
        m_errorOutput = QString::fromLocal8Bit(readAllStandardError()).trimmed();
        qDebug()<<m_errorOutput;
    });

    connect(this,&QProcess::readyReadStandardOutput,this,[this](){
        m_standardOutput = QString::fromLocal8Bit(readAllStandardOutput()).trimmed();
        qDebug()<<m_standardOutput;
    });

    connect(this,&QProcess::started,this,[this](){
        emit adbProcessResult(AER_SUCCESS_START);
        qDebug()<<"started";
    });

}
