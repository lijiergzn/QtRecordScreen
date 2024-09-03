#ifndef SERVER_H
#define SERVER_H
#include<QObject>
#include "adbprocess.h"

// 启动命令
// adb shell /data/local/tmp/scrcpy-srever.jar app_process / com.genymobile.scrcpy.Server maxsize bitrate false ""
// adb shell /data/local/tmp/scrcpy-srever.jar app_process / com.genymobile.scrcpy.Server 1080 800000 false ""
class server : public QObject
{
    Q_OBJECT

    enum SERVER_START_STEP {
        SSS_NULL,
        SSS_PUSH,
        SSS_ENABLE_REVERSE,
        SSS_EXECUTE_SERVER,
        SSS_RUNNING,
    };

public:
    server(QObject *parent = Q_NULLPTR);

    bool start(const QString& serial, quint16 localPort, quint16 maxSize, quint32 bitRate);

private:
    bool startServerByStep();
    bool pushServer();
    bool enableTunnelReverse();
    QString getServerPath();

signals:
    void serverStartResult(bool success);

private slots:
    void onWorkProcessResult(AdbProcess::ADB_EXEC_RESULT processResult);

private:
    QString m_serical = "";
    quint16 m_maxSize = 0;
    quint16 m_localPort = 0;
    quint32 m_bitRate = 0;
    SERVER_START_STEP m_serverStartStep = SSS_NULL;

    AdbProcess m_workProcess;
    QString m_serverPath = "";
};

#endif // SERVER_H
