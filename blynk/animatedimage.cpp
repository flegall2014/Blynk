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

// Load cursor images:
void AnimatedImage::loadCursorImages(const QDir &imgDir)
{
    QStringList lFilters;
    lFilters << "*.png";

    QStringList lPngFiles = Utils::files(imgDir.absolutePath(), lFilters);
    foreach (QString sPngFile, lPngFiles)
        m_vCursorImages << QImage(sPngFile);
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
