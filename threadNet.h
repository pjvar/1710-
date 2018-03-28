#ifndef NET_H
#define NET_H
//IP 176.201.120.38
#include <QThread>
#include <QTcpSocket>
#include <QHostAddress>
#include <QMessageBox>
#include <QDebug>
#include <QReadWriteLock>
#include <QTimer>

class threadNet:public QThread
{
Q_OBJECT

public:
    explicit threadNet(quint16 port=8080);
    ~threadNet();
    void run();
public slots:
    void startThread();
    void onConnect();
    void writeData(const char*p_chars);
private:
    QTcpSocket m_socket;
    QHostAddress m_address;
    quint16 m_port;
    QByteArray m_dataArray;
    QTimer m_timer;
};

#endif // NET_H
