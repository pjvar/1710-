#include "threadclass.h"
//threadClass::threadClass(void){}

threadClass::threadClass(player&that):m_pl(that)
{}

threadClass::~threadClass()
{
    qDebug() << "thread end";
}

void threadClass::run()
{

}
