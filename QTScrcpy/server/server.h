#ifndef SERVER_H
#define SERVER_H
#include<QObject>
#include "adbprocess.h"
#include<QTcpServer>
#include <QTcpSocket>

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
    void stop();

private:
    bool startServerByStep();
    bool pushServer();
    bool removeServer();
    bool enableTunnelReverse();
    bool disableTunnelReverse();
    bool execute();
    
    QString getServerPath();
    bool readInfo(QString& deviceName, QSize& size);

signals:
    void serverStartResult(bool success);
    void connectToResult(bool success, const QString& deviceName, QSize& size);

private slots:
    void onWorkProcessResult(AdbProcess::ADB_EXEC_RESULT processResult);


private:
    QString m_serical = "";
    quint16 m_maxSize = 0;
    quint16 m_localPort = 0;
    quint32 m_bitRate = 0;
    SERVER_START_STEP m_serverStartStep = SSS_NULL;

    AdbProcess m_workProcess;
    AdbProcess m_serverProcess;

    QString m_serverPath = "";
    bool m_serverCopiedToDevice = false;  // 判断sdk push是否成功
    bool m_enableReverse = false;

    QTcpServer m_serverSocket;
    QTcpSocket* m_deviceSocket = Q_NULLPTR;

};

#endif // SERVER_H
