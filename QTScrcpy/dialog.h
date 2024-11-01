#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include"server.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Dialog; }
QT_END_NAMESPACE

class Dialog : public QDialog
{
    Q_OBJECT

public:
    Dialog(QWidget *parent = nullptr);
    ~Dialog();

private slots:
    void on_testBut_clicked();

    void on_stopBut_clicked();

private:
    Ui::Dialog *ui;

    server m_server;
};
#endif // DIALOG_H
