#include "player.h"
threadUpdata::threadUpdata(player &that, QList<player>&thatList, QList<QPoint> &scorePos)
    :m_pl(that),m_thatList(thatList),m_scorePos(scorePos)
{
//    m_threadCount = 0;
}

threadUpdata::~threadUpdata(){}

void threadUpdata::threadBegin()
{}

void threadUpdata::run()
{
    QTime delayTime;
    while(1){  
        this->m_pl.m_dps ++;
        delayTime.start();
        for(int i=0;i<m_thatList.length();i++){
            if(m_pl.knocked(m_thatList[i])){
                m_scorePos.push_back(m_thatList[i].getpos());
                m_thatList.removeAt(i);
            }
        }

        qint32 xlen = (m_pl.m_goto - m_pl.getpos()).x();
        qint32 ylen = (m_pl.m_goto - m_pl.getpos()).y();
        qreal R = qAtan2(ylen,xlen);
        if(!m_pl.xarrived()){
            if(m_pl.m_goto.x()>m_pl.getpos().x()){
                m_pl.m_pos_x += m_pl.m_speed*qCos(R);
            }
            else{
                m_pl.m_pos_x += m_pl.m_speed*qCos(R);
            }
        }
        if(!m_pl.yarrived()){
            if(m_pl.m_goto.y()>m_pl.getpos().y()){
                m_pl.m_pos_y += m_pl.m_speed*qSin(R);
            }
            else{
                m_pl.m_pos_y += m_pl.m_speed*qSin(R);
            }
        }
        if(m_pl.m_hpLocked >0)
        m_pl.m_hpLocked -=10;
        msleep ((1000-delayTime.elapsed()));
    }
    m_net.terminate();
    m_net.wait();
}
