#include "threadNet.h"

QReadWriteLock mutex_RDWR;

threadNet::threadNet(quint16 port):m_port(port)
{
    m_timer.setInterval(100);
    connect(&m_timer,SIGNAL(timeout()),this,SLOT(writeData()));
    if(!m_address.setAddress("176.201.120.38")){//
//    if(!m_address.setAddress("127.0.0.1")){
        qDebug() << "IP设置错误！";
        return;
    }
    m_socket.connectToHost(m_address,m_port);
    connect(&m_socket,SIGNAL(connected()),this,SLOT(onConnect()));
    connect(&m_socket,SIGNAL(readyRead()),this,SLOT(startThread()));
    if(m_socket.isOpen()){
        qDebug() << "net is open";
    }
    m_timer.start();
}

threadNet::~threadNet()
{

}

void threadNet::run()
{
//    qDebug() << "run0";
    QByteArray dataBuf;
    if(quint16 size = m_socket.bytesAvailable()){
        qDebug() << "read";
        dataBuf.resize(size);
        m_socket.read(dataBuf.data(), size);
    }
//    m_socket.write(QString("hello").toLocal8Bit());
}

void threadNet::onConnect()
{
    QString str = "FOX";
//    m_socket.write(str.toLocal8Bit());
}

void threadNet::startThread()
{
    start();
}

void threadNet::writeData(const char *p_chars)
{
    //str = x|y|range;
//    QString str = QString::number(pl.getpos().x()) + "|" + QString::number(pl.getpos().y());
//    str += "|" +QString::number(pl.getrangeInt());
//    QByteArray bytes(p_chars);
//    m_socket.write(bytes);
    QString str = "hello";
    m_socket.write(str.toLocal8Bit());
}









