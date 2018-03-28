#include "player.h"
QList <QImage>*player::m_gold = new QList<QImage>;
QMutex m_mutex;
//用于实例化场景生物
player::player(QString &name, qreal speed, qreal posx, qreal posy, qreal range, QColor color, quint32 invincibleTime):
    m_name(name),m_speed(speed),m_pos_x(posx),m_pos_y(posy),m_range(range),m_color(color),m_hpLocked(3000)
{
    //初次载入图片
    if(m_gold->empty()){
        QImage g_gold[10];
         g_gold[0].load(":gold.png");
         g_gold[1].load(":gold1.png");
         g_gold[2].load(":bond.png");
         g_gold[3].load(":boom.png");

         for(int i=0;i<10&&(!g_gold[i].isNull());i++){
            m_gold->push_back(g_gold[i].scaled(40,40));
         }
    }
    //选取皮肤
    m_skinId = qrand()%m_gold->length();
    if(m_skinId == 3){
        m_bonus = -2;
    }
    else{
        m_bonus = 1;
    }
}

//玩家
player::player(){
    m_name = "bb";
    m_speed = 8;
    m_pos_x = 10;
    m_pos_y = 10;
    m_range = 10;
    m_color = rand()%0xffffff;
    m_hpLocked = 3000;//3s 无敌
    m_rangeMax = 200;
}

player::~player()
{
    if(m_gold->empty()){
        delete m_gold;
    }
}

void player::setgoto(quint32 x, quint32 y,QPoint pos)
{
    m_goto.rx() = x+pos.x();
    m_goto.ry() = y+pos.y();
}

bool player::xarrived(){
    return(abs(getpos().x()-m_goto.x()) < m_speed);
}

bool player::yarrived(){
    return(abs(getpos().y()-m_goto.y()) < m_speed);
}

bool player::knocked(player&npc){
    if(this->getrange()<0){
        return false;
    }
    if(this->m_hpLocked>0){return false;}
    qreal x_2 = qPow((getpos()-npc.getpos()).x(),2.0);
    qreal y_2 = qPow((getpos()-npc.getpos()).y(),2.0);

    if((getrange()+npc.getrange()) > qPow(x_2+y_2,0.5)){//距离检测
        if(npc.m_bonus > 0)
        {
            if(this->getrange() < this->m_rangeMax){//上限检测{
                this->addRange(npc.m_bonus);// qPow(qPow(npc.getrange(),2.0)+qPow(getrange(),2.0),0.5)*2;//吞噬，增加半径，面积和
            }
        }
        else{
            this->addRange(npc.m_bonus);
        }
        return true;
    }
    else{
        return false;
    }
}

QPoint player::getpos(QPoint pos)const
{
    QPoint p((quint32)(m_pos_x-pos.x()),(quint32)(m_pos_y-pos.y()));
    return p;
}

qreal player::getrange()const
{
    m_mutex.lock();
    qreal i = m_range;
    m_mutex.unlock();
    return i;
}

QRect player::getPlayerRect(void)
{
    return QRect (m_pos_x-this->getrange(),m_pos_y-this->getrange(),this->getrange()*2,this->getrange()*2);
}

void player::setPen(QPen &pen,qreal ratio)
{
    pen.setColor(this->m_color);
    pen.setWidth(this->getrange()*ratio);
}

player player::setDead()
{
    player deadPlayer;
    deadPlayer = *this;
    m_speed = 0;
    this->setRange(0);
    return deadPlayer;
}

void player::drawSelf(QPainter&painter,QRect*p_rect, qreal ratio)
{
    QMargins maring;
    if(!p_rect->contains(this->getpos())){return;}
    painter.drawEllipse(this->getpos(p_rect->topLeft()),10,10);
    QPoint imgPos(this->getpos(p_rect->topLeft()).x()-(*m_gold)[m_skinId].width()/2,this->getpos(p_rect->topLeft()).y()-(*m_gold)[m_skinId].height()/2);
    painter.drawImage(imgPos,(*m_gold)[m_skinId]);
}

void player::drawPlayer(QPainter&painter, QRect *p_rect, QImage &player)
{
    QPoint topLeft = p_rect->topLeft(),bottomRight = p_rect->bottomRight();
    QRect beginPaintRect;
    beginPaintRect.setTopLeft(topLeft-QPoint(this->getrange(),this->getrange()));
    beginPaintRect.setBottomRight(bottomRight+QPoint(this->getrange(),this->getrange()));
    painter.drawLine(this->getpos()-p_rect->topLeft(),this->m_goto-p_rect->topLeft());
    if(!beginPaintRect.contains(this->getpos())){return;}
    QImage playerImg = player.scaled(this->getrange()*2,this->getrange()*2);
    QPoint imgPos(this->getpos(p_rect->topLeft()).x()-getrange(),this->getpos(p_rect->topLeft()).y()-getrange());
    painter.drawImage(imgPos,playerImg);
}

void player::setRange(qreal range)
{
    m_mutex.tryLock(1);
    m_range = range;
    m_mutex.unlock();
}

void player::addRange(qreal range)
{
    m_mutex.tryLock(1);
    m_range += range;
    m_mutex.unlock();
}

quint32 player::getrangeInt()const
{
    m_mutex.tryLock(1);
    return (quint32)getrange();
    m_mutex.unlock();
}
