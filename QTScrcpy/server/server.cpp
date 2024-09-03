#include "server.h"


#define DEVICE_SERVER_PATH "/data/local/tmp/scrcpy-server.jar"
#define SOCKET_NAME "scrcpy"
server::server(QObject *parent)
    : QObject(parent)
{
    connect(&m_workProcess, &AdbProcess::adbProcessResult, this, &server::onWorkProcessResult);
}

bool server::start(const QString &serial, quint16 localPort, quint16 maxSize, quint32 bitRate)
{
    m_serical = serial;
    m_localPort = localPort;
    m_maxSize = maxSize;
    m_bitRate = bitRate;

    // start push server
    m_serverStartStep = SSS_PUSH;

    return startServerByStep();
}

bool server::startServerByStep()
{
    bool stepSuccess = false;
    // push,enable reverse ,execute server
    if(SSS_NULL != m_serverStartStep){
        switch (m_serverStartStep) {
        case SSS_PUSH:
            stepSuccess = pushServer();
            break;
        case SSS_ENABLE_REVERSE:
            stepSuccess = enableTunnelReverse();
            break;
        case SSS_EXECUTE_SERVER:

            break;
        default:
            break;
        }
    }

    return stepSuccess;
}

bool server::pushServer()
{
    m_workProcess.push(m_serical, getServerPath(), DEVICE_SERVER_PATH);
    return true;
}

bool server::removeServer()
{
    AdbProcess* adb = new AdbProcess();
    if(!adb){
        return false;
    }
    connect(adb, &AdbProcess::adbProcessResult, this,[this](AdbProcess::ADB_EXEC_RESULT processResult){
        if(AdbProcess::AER_SUCCESS_START != processResult){
            sender()->deleteLater();
        }
    });
    adb->remotePath(m_serical, DEVICE_SERVER_PATH);
    return true;
}

bool server::enableTunnelReverse()
{
    m_workProcess.reverse(m_serical, SOCKET_NAME, m_localPort);
    return true;
}

QString server::getServerPath()
{

    if(m_serverPath.isEmpty()){
        m_serverPath = QString::fromLocal8Bit(qgetenv("QTSCRCPY_SERVER_PATH"));
    }
    return m_serverPath;
}

void server::onWorkProcessResult(AdbProcess::ADB_EXEC_RESULT processResult)
{
    if(SSS_NULL != m_serverStartStep){
        switch (m_serverStartStep) {
        case SSS_PUSH:
            if(AdbProcess::AER_SUCCESS_EXEC == processResult){
                m_serverStartStep = SSS_ENABLE_REVERSE;
                startServerByStep();
            }else if(AdbProcess::AER_SUCCESS_START != processResult){
                qCritical("adb push failed");
                m_serverStartStep = SSS_NULL;
                emit serverStartResult(false);
            }
            break;
        case SSS_ENABLE_REVERSE:
            if(AdbProcess::AER_SUCCESS_EXEC == processResult){
                m_serverStartStep = SSS_EXECUTE_SERVER;
                startServerByStep();
            }else if(AdbProcess::AER_SUCCESS_START != processResult){
                qCritical("adb reverse failed");
                m_serverStartStep = SSS_NULL;
                // 把第一步的apk文件删除
                removeServer();
                emit serverStartResult(false);
            }
            break;
        default:
            break;
        }
    }
}




