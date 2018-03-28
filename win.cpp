#include "win.h"
#include "ui_win.h"

win::win(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::win),m_paintCount(0)
{
    setMouseTracking(true);
    m_standerdTime_ms = 10;
    m_scoreImg = new QImage(":score.gif");
    m_player = new player();
    m_threadUpdata = new threadUpdata(*m_player,m_env,m_scorePos);
    m_cameryRect = new QRect;
    m_worldImg = new QImage(":world.jpg");
    m_playerImg = new QImage(":player.png");

    this->setWindowState(Qt::WindowMaximized);

    m_cameryRect->setRect(0,0,QApplication::desktop()->width(),QApplication::desktop()->height());
    ui->setupUi(this);
    qsrand(time(0));
    m_timer_flush.setInterval(1);m_timer_speed.setInterval(10);
    m_timer_fps.setInterval(1000);m_timer_moveCamery.setInterval(10);

    connect(&m_timer_moveCamery,SIGNAL(timeout()), this, SLOT(moveCamery()));
    connect(&m_timer_flush,SIGNAL(timeout()),this, SLOT(update()));
    connect(&m_timer_speed,SIGNAL(timeout()),this,SLOT(up_data()));
    connect(&m_timer_fps,SIGNAL(timeout()),this,SLOT(getFps()));
    m_envMaxCount = 100;
    setWorld();
    m_timer_flush.start();
    m_timer_speed.start();
    m_timer_fps.start();
    m_lastTime_paint.start();
    m_threadUpdata->start();
    m_timer_moveCamery.start();
}

win::~win()
{
    qDebug() << "程序开始退出";
    player::m_gold->clear();
    delete player::m_gold;
    delete m_player;
    delete m_playerImg;
    delete m_cameryRect;
    delete m_worldImg;
    m_threadUpdata->terminate();
    m_threadUpdata->wait(0);
    qDebug() << "数据更新线程正在运行：" << m_threadUpdata->isRunning();
    delete m_threadUpdata;
    delete ui;
}

//使用背景图片确定场景大小
void win::setWorld()
{
    m_worldx = m_worldImg->width()-106;/*-m_cameryRect->width()*/;
    m_worldy = m_worldImg->height()-250/*-m_cameryRect->height()-150*/;
}


void win::paintEvent(QPaintEvent *e)
{
    m_paintCount++;
    QPainter painter(this);
////    if(!m_scorePos.empty()){
//        painter.drawImage(m_scorePos.front(),*m_scoreImg);
//        m_scorePos.pop_front();
//    }
    painter.drawImage(QPoint(0,0)-m_cameryRect->topLeft(),*m_worldImg);
    m_player->drawPlayer(painter,m_cameryRect, *m_playerImg);
    for(int i=0;i<m_env.length();i++){
        m_env[i].drawSelf(painter, m_cameryRect, 1);
    }
}

void win::up_data()
{
    ui->label_env->setText("生物：" + QString::number(m_env.length()));
    ui->label_hp->setText("奖励：" + QString::number(m_player->getrange()));
    QString str = QString::number(m_player->getpos().x()) + "("+QString::number(m_worldx)+")" + ":" +
                            QString::number(m_player->getpos().y()) + "(" +QString::number(m_worldy)+ ")";
    ui->label_playerPos->setText(str);
    //add NPC
    while(m_env.length()<m_envMaxCount){
        addenv();
    }
//    QPainter painter(this);
//    QRectF target(0,0,m_worldx,m_worldy),source(*m_cameryRect);
//    painter.drawImage(target, *m_worldImg, target);
//auto
    qreal minlen = 100000;
    quint32 j = 0;
    for(int i=0; i<m_env.length(); i++){
        qreal len = qAbs(m_player->m_pos_x - m_env[i].m_pos_x) + qAbs(m_player->m_pos_y - m_env[i].m_pos_y);
        if(minlen > len){
            j = i;
            minlen = len;
        }
    }
    m_player->setgoto(m_env[j].getpos().x(),m_env[j].getpos().y());
}

void win::mousePressEvent(QMouseEvent *e)
{
//    QPoint adjustedPos = e->pos();
    m_player->setgoto(e->x(),e->y(),m_cameryRect->topLeft());
}

//void win::on_pushButton_clicked()
//{
//    player deadPlayer = m_player->setDead();
//    addenv(deadPlayer);
//}

void win::addenv(quint32  invincible)
{
    QPoint p(0,0);
    QString str = "n";
    qrand();
    player per(str, 0, qrand()%m_worldx, qrand()%m_worldy, qrand()%20, qrand()%(0xffffff));
    m_env.push_back(per);
}

void win::addenv(const player&deadPlayer)//。。。
{
    quint32 count = deadPlayer.getrange();
    while(count--){
        QString str = "n";
        qreal xx,yy;
        do{
            xx = qrand()%(deadPlayer.getrangeInt()*2)+deadPlayer.getpos().x()-deadPlayer.getrange();
            yy = qrand()%(deadPlayer.getrangeInt()*2)+deadPlayer.getpos().y()-deadPlayer.getrange();
        }while(qPow(xx*xx*yy*yy,0.5) < deadPlayer.getrangeInt());
        player per(str, 0, xx, yy, 3, deadPlayer.m_color);
        m_env.push_back(per);
    }
}

quint32 win::getFps()
{
    QString fps = "FPS:";
    fps += QString::number(m_paintCount);
    fps += " DPS:" + QString::number(m_player->m_dps);
    m_paintCount = 0;
    m_player->m_dps=0;
    ui->label_fps->setText(fps);
}

void win::mouseMoveEvent(QMouseEvent *e)
{
    m_mousePos = e->pos();
}

void win::moveCamery()
{
    quint16 speed = 6;
    if((m_mousePos.x() < 50)&&(m_cameryRect->x()>0)){
        m_cameryRect->moveLeft(m_cameryRect->x()-speed);
    }
    if((m_mousePos.x() > (width()-50))&&(m_cameryRect->x()+m_cameryRect->width() < m_worldx+100)){
        m_cameryRect->moveLeft(m_cameryRect->x()+speed);
    }
    if(m_mousePos.y() < 50 && (m_cameryRect->y()>0)){
        m_cameryRect->moveTop(m_cameryRect->y()-speed);
    }
    if(m_mousePos.y() > height() - 50 && (m_cameryRect->y()+m_cameryRect->height() < m_worldy+100)){
        m_cameryRect->moveTop(m_cameryRect->y()+speed);
    }
    update();
}

void win::on_ui_timeOutScollBar_valueChanged(int value)
{
    m_envMaxCount = value*100;
}

void win::keyPressEvent(QKeyEvent *e)
{
    if(e->key() == 32){
        m_cameryRect->moveCenter(m_player->getpos());
    }
}
