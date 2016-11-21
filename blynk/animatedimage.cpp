// Qt:
#include <QDebug>
#include <QApplication>

// Application:
#include "animatedimage.h"
#include "utils.h"
#include "parameters.h"

// Constructor:
AnimatedImage::AnimatedImage(QWidget *parent) :
    QObject(parent), m_iCurrentImageIndex(0)
{
    m_tTimer.setInterval(25);
    connect(&m_tTimer, &QTimer::timeout, this, &AnimatedImage::onTimeOut);
}

// Destructor:
AnimatedImage::~AnimatedImage()
{
}

// Load images:
void AnimatedImage::loadImages(const What &what)
{   
    if (what == BIGEYE_IMAGES)
    {
        for (int i=19; i<=36; i++) {
            QString sPngFile = QString(":/animatedimage/Eye_%1.png").arg(i);
            m_vCursorImages << QImage(sPngFile);
        }
    }
    else
    if (what == CURSOR_IMAGES)
    {
        for (int i=1; i<=36; i++) {
            QString sIndex = QString::number(i);
            if (sIndex.length() < 2)
                sIndex = "0"+sIndex;
            QString sPngFile = QString(":/cursor/Fra_%1.png").arg(sIndex);
            m_vCursorImages << QImage(sPngFile);
        }
    }
}

// Play:
void AnimatedImage::play()
{
    m_iCurrentImageIndex = 0;
    m_tTimer.stop();
    m_tTimer.start();
}

// Time out:
void AnimatedImage::onTimeOut()
{
    if (m_iCurrentImageIndex < m_vCursorImages.size()) {
        bool bIsLast =  m_iCurrentImageIndex == (m_vCursorImages.size()-1);
        emit nextImage(m_vCursorImages[m_iCurrentImageIndex++], bIsLast);
    }
    else
    {
        m_tTimer.stop();
        emit done();
    }
}
