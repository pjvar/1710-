#ifndef NETTHREAD_H
#define NETTHREAD_H

#include <QObject>

class netthread : public QObject
{
    Q_OBJECT
public:
    explicit netthread(QObject *parent = 0);
    ~netthread();

signals:

public slots:
};

#endif // NETTHREAD_H
