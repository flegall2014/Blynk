// Qt:
#include <QMovie>
#include <QPainter>

// Application:
#include "bigeyewidget.h"

// Constructor:
BigEyeWidget::BigEyeWidget(const QString &sMoviePath, QWidget *parent) : QLabel(parent),
    m_iOpacity(255)
{
    // Set attributes:
    setWindowFlags(Qt::FramelessWindowHint | Qt::SubWindow | Qt::WindowStaysOnTopHint);
    setAttribute(Qt::WA_TranslucentBackground);

    // Create movie:
    m_pBigEyeMovie = new QMovie(sMoviePath);
    m_pBigEyeMovie->setCacheMode(QMovie::CacheAll);
    connect(m_pBigEyeMovie, &QMovie::frameChanged, this, &BigEyeWidget::onFrameChanged);
    hide();
    setMovie(m_pBigEyeMovie);
}

// Set opacity:
void BigEyeWidget::setOpacity(int iOpacity)
{
    m_iOpacity = iOpacity;
    update();
}

// Frame changed:
void BigEyeWidget::onFrameChanged(int iFrameIndex)
{
    static int loopCount = 0;
    if (iFrameIndex == (m_pBigEyeMovie->frameCount()-1))
        loopCount++;
    if (loopCount == 2)
    {
        hide();
        m_pBigEyeMovie->stop();
        loopCount = 0;
    }
}

// Handle paint events:
void BigEyeWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);

    // Draw window background:
    painter.fillRect(rect(), QColor(255, 255, 255, m_iOpacity));
}

