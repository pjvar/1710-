#ifndef WIN_H
#define WIN_H

#include "threadNet.h"
#include <QDebug>
#include <QWidget>
#include <QMouseEvent>
#include <QTimer>
#include <QTime>
#include "player.h"
#include <QPainter>
#include <QtMath>
#include <QPicture>
#include <QKeyEvent>
//
#include <thread>
#include <iostream>
#include <unistd.h>
#include <QApplication>
#include <QDesktopWidget>


class threadPaint:public QThread
{
public:
    threadPaint(QPainter &thatPainter, player &pl, QList<player> &plist);
    ~threadPaint();
    void run();

private:
    QPainter&m_painter_p;
    player &m_pl;
    QList<player>&m_plist;
};


namespace Ui {
class win;
}

//win
class win : public QWidget
{
    Q_OBJECT
public:
    explicit win(QWidget *parent = 0);
    ~win();
    void mousePressEvent(QMouseEvent*e);
    void mouseMoveEvent(QMouseEvent*e);
    void paintEvent(QPaintEvent *e);  
    void keyPressEvent(QKeyEvent*e);
    bool knocked(player p);
    void addenv(quint32 invincible=0);
    void addenv(const player &deadPlayer);
    void setWorld();
public slots:
    void up_data();
    void moveCamery();
private slots:
    quint32 getFps();
    void on_ui_timeOutScollBar_valueChanged(int value);

private:
    QList<player>m_env;
    player *m_player;
    QList<player> m_others;

    int m_envcount;
    Ui::win *ui;
    QTimer m_timer_flush,m_timer_speed, m_timer_fps, m_timer_moveCamery;
    quint16 m_paintCount, m_upDataCount;
    threadUpdata*m_threadUpdata;
    threadPaint*m_threadPaint;

    QTime m_lastTime_paint;
    QTime m_lastTime_updata;
    quint32 m_standerdTime_ms;
    quint64 m_worldx,m_worldy;
    QRect*m_cameryRect;
    QPoint m_mousePos;
    QRect m_deskRect;
    QImage *m_worldImg ,*m_playerImg,*m_scoreImg;
    quint32 m_envMaxCount;
    QList<QPoint>m_scorePos;

};




#endif // WIN_H
