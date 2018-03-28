#ifndef THREADCLASS_H
#define THREADCLASS_H
#include "player.h"
#include <QThread>
#include <QDebug>

class threadClass:public QThread
{
public:
    //threadClass(void);
    threadClass(player&that);
    ~threadClass();
    void run();
    void threadBegin();
private:
    int runCount;
    player& m_pl;
};

#endif // THREADCLASS_H
