#include "animatedcursor.h"
#include <QDebug>
#include <QApplication>
#include "utils.h"

// Constructor:
AnimatedCursor::AnimatedCursor(QWidget *parent) :
    QObject(parent), m_iCurrentImageIndex(0)
{
    loadCursorImages();
    m_tTimer.setInterval(25);
    connect(&m_tTimer, &QTimer::timeout, this, &AnimatedCursor::onTimeOut);
}

// Destructor:
AnimatedCursor::~AnimatedCursor()
{
}

// Load cursor images:
void AnimatedCursor::loadCursorImages()
{
    QDir appDir = Utils::appDir();
    appDir.cdUp();
    appDir.cd("blynk");
    appDir.cd("cursor");

    QStringList lFilters;
    lFilters << "*.png";

    QStringList lPngFiles = Utils::files(appDir.absolutePath(), lFilters);
    foreach (QString sPngFile, lPngFiles)
        m_vCursorImages << QImage(sPngFile);
}

// Play:
void AnimatedCursor::play()
{
    m_iCurrentImageIndex = 0;
    m_tTimer.stop();
    m_tTimer.start();
}

// Time out:
void AnimatedCursor::onTimeOut()
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
