#ifndef ADBPROCESS_H
#define ADBPROCESS_H
#include<QProcess>

class AdbProcess:public QProcess
{
public:
    AdbProcess(QObject *parent = nullptr);


    void execute(const QString& serial, const QStringList& args);
    void push(const QString& serial, const QString& local, const QString& remote);
    void remotePath(const QString& serial, const QString& remote);
    void reverse(const QString& serial, const QString& deviceSocketName, quint16 localPort);
    void reverseRemove(const QString& serial, const QString& deviceSocketName);

    static QString getAdbPath();
private:

    void initSignals();
    static QString s_adbPath;
};

#endif // ADBPROCESS_H
