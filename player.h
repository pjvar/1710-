#ifndef PLAYER_H
#define PLAYER_H

#include <unistd.h>
#include <iostream>
#include <QPoint>
#include <QtMath>
#include <QColor>
#include <QPen>
#include <QPainter>
#include <QThread>
#include <QDebug>
#include <QMutex>
#include <QTime>
#include <QPicture>
#include "threadNet.h"

//using namespace std;
class player;
extern QMutex m_mutex;
//THREAD
class threadUpdata:public QThread
{
public:
    threadUpdata(player &that,QList<player>&thatList,QList<QPoint>&scorePos);
    ~threadUpdata();
    void threadBegin();
    void run();
private:
    player& m_pl;
    QList<player>& m_thatList;
    QList<QPoint>& m_scorePos;
    threadNet m_net;
};


class player
{
public:
    player(QString& name, qreal speed, qreal posx, qreal posy, qreal range, QColor color,quint32  invincibleTime=0);
    player();
    ~player();
    friend void threadUpdata::run();

    void drawSelf(QPainter&painter, QRect *p_rect, qreal ratio);
    void drawPlayer(QPainter&painter, QRect *p_rect, QImage &player);
//    void drawSelf(QPainter &painter, qreal ratio);
    bool move(QList<player> &env_list);
    void setgoto(quint32 x, quint32 y, QPoint pos = QPoint(0,0));
    qreal getrange() const;
    quint32 getrangeInt() const;
    void setRange(qreal range);
    void addRange(qreal range);
    void setPen(QPen&pen, qreal ratio);
    player setDead();
    QPoint getpos(QPoint p = QPoint(0,0))const;

    qreal m_pos_x,m_pos_y;
    QPoint m_goto;
    qreal m_speed;
    QColor m_color;
    quint32 m_dps;
    static QList<QImage> *m_gold;
private:
    QString m_name;
    QRect getPlayerRect(void);

    bool knocked(player &npc);
    bool xarrived();
    bool yarrived();
    qreal m_range;
    quint32 m_hpLocked;//无敌状态
    quint32 m_rangeMax;
    quint8 m_skinId;
    qint16 m_bonus;

};

#endif // PLAYER_H
