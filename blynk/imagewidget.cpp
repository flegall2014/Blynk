// Qt:
#include <QPainter>
#include <QDebug>

// Application:
#include "imagewidget.h"

// Constructor:
ImageWidget::ImageWidget(QWidget *parent) : QWidget(parent),
    m_sImagePath(""), m_sText(""), m_textColor(QColor(0, 0, 0))
{

}

// Set image:
void ImageWidget::setImage(const QString &sImagePath)
{
    m_sImagePath = sImagePath;
    update();
}

// Set text:
void ImageWidget::setText(const QString &sText, const QColor &textColor)
{
    m_sText = sText;
    m_textColor = textColor;
}

// Paint event:
void ImageWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.fillRect(rect(), Qt::white);
    QImage image(m_sImagePath);
    if (!image.isNull())
    {
        // Compute image ratio:
        double ratio = qRound((double)image.width()/(double)image.height());
        int targetHeight = qRound(rect().height()*.75);
        int targetWidth = qRound(targetHeight*ratio);
        painter.setRenderHint(QPainter::SmoothPixmapTransform);
        int x = (rect().width()-targetWidth)/2;
        int y = (rect().height()-targetHeight)/2;
        painter.drawImage(QPoint(x, y), image.scaled(targetWidth, targetHeight, Qt::KeepAspectRatio));
        painter.setFont(QFont("times",24));
        QRect textRect(QPoint(rect().width()/2, 0), QSize(rect().width()/2-8, rect().height()));
        painter.setPen(m_textColor);
        painter.drawText(textRect, Qt::AlignVCenter, m_sText);
    }
}
