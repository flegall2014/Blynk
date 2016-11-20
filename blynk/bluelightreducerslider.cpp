// Qt:
#include <QPainter>

// Application:
#include "bluelightreducerslider.h"

// Constructor:
BlueLightReducerSlider::BlueLightReducerSlider(QWidget *parent) : QSlider(parent)
{
    setOrientation(Qt::Horizontal);
    setPageStep(1);
}

// Handle paint event:
void BlueLightReducerSlider::paintEvent(QPaintEvent *event)
{
    QSlider::paintEvent(event);
    QPainter painter(this);
    QFont segoeUIFont("Segoe-UI", 12);
    segoeUIFont.setBold(true);
    painter.setFont(segoeUIFont);

    QList<QColor> lTextColors;
    if (value() == 0)
        lTextColors << QColor("#FFFFFF") << QColor("#B3B3B3") << QColor("#B3B3B3");
    else
    if (value() == 1)
        lTextColors << QColor("#B3B3B3") << QColor("#FFFFFF") << QColor("#B3B3B3");
    else
    if (value() == 2)
        lTextColors << QColor("#B3B3B3") << QColor("#B3B3B3") << QColor("#FFFFFF");

    QRect r = rect();
    QRect r1(0, 0, r.width()/3, r.height());
    painter.setPen(lTextColors[0]);
    painter.drawText(r1, Qt::AlignVCenter|Qt::AlignHCenter, tr("LIGHT"));
    QRect r2(r.width()/3, 0, r.width()/3, r.height());
    painter.setPen(lTextColors[1]);
    painter.drawText(r2, Qt::AlignVCenter|Qt::AlignHCenter, tr("MEDIUM"));
    QRect r3(2*r.width()/3, 0, r.width()/3, r.height());
    painter.setPen(lTextColors[2]);
    painter.drawText(r3, Qt::AlignVCenter|Qt::AlignHCenter, tr("STRONG"));
}


