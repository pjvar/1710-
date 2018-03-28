#include "win.h"
//PAINT THREAD
threadPaint::threadPaint(QPainter &thatPainter, player &pl, QList<player> &plist):
    m_painter_p(thatPainter),m_pl(pl),m_plist(plist) {}

threadPaint::~threadPaint(){}

void threadPaint::run()
{
    while(1){
        QImage background(":/background.jpg");
        QRectF target(0,0,1000,800);
        QRectF source(0,0,1000,800);
        m_pl.drawSelf(&background, 0.1);

        for(int i=0;i<m_plist.length();i++){
            m_plist[i].drawSelf(&background, 2);
        }
        m_painter_p.drawImage(target, background, source);
        msleep(10);
        qDebug() << "paing thread:" << currentThreadId();
    }
}
