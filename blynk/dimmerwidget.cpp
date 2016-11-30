// Qt:
#include <QPainter>
#include <QMovie>
#include <QDesktopWidget>
#include <QDebug>
#include <QShowEvent>

// Application:
#include "dimmerwidget.h"
#include "ui_dimmerwidget.h"
#include "animatedimage.h"
#include "parameters.h"
#ifdef Q_OS_WIN
#include <windowsmonitor.h>
#elif defined(Q_OS_OSX)
#include <macmonitor.h>
#endif
#include "blynk.h"
#include "controller.h"
#include <utils.h>
#define MAX_LOOP_TIMES 1

// Constructor:
DimmerWidget::DimmerWidget(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DimmerWidget),
    m_pAnimatedCursor(NULL),
    m_pAnimatedImage(NULL),
    m_bCursorDone(false),
    m_bAnimatedImageDone(false),
    m_pParameters(NULL),
    m_pMonitor(NULL),
    m_bSupportGammaRamp(false)
{
    // Setup UI:
    ui->setupUi(this);

    // Define window modality:
    setWindowModality(Qt::NonModal);

    // Window attribute:
    setAttribute(Qt::WA_TranslucentBackground);
    setAttribute(Qt::WA_TransparentForMouseEvents);
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::SubWindow);

    // Retrieve desktop information:
    QDesktopWidget qDesktopWidget;
    QRect screenSize = qDesktopWidget.screenGeometry();
    QSize sSingleImageSize(770, 441);
    setGeometry((screenSize.width()-sSingleImageSize.width())/2,
        (screenSize.height()-sSingleImageSize.height())/2,
                sSingleImageSize.width(), sSingleImageSize.height());

    // Animated cursor:
    m_pAnimatedCursor = new AnimatedImage(this);
    m_pAnimatedCursor->loadImages(AnimatedImage::CURSOR_IMAGES);
    connect(m_pAnimatedCursor, &AnimatedImage::nextImage, this, &DimmerWidget::onNextCursorImageAvailable);
    connect(m_pAnimatedCursor, &AnimatedImage::done, this, &DimmerWidget::onCursorDone);

    // Animated image:
    m_pAnimatedImage = new AnimatedImage(this);
    m_pAnimatedImage->loadImages(AnimatedImage::BIGEYE_IMAGES);
    connect(m_pAnimatedImage, &AnimatedImage::nextImage, this, &DimmerWidget::onNextAnimatedImageAvailable);
    connect(m_pAnimatedImage, &AnimatedImage::done, this, &DimmerWidget::onAnimatedImageDone);

    // Create monitor:
#ifdef Q_OS_WIN
    m_pMonitor = new WindowsMonitor();
#elif defined(Q_OS_OSX)
     m_pMonitor = new MacMonitor();
#endif
    m_bSupportGammaRamp = m_pMonitor->start();
}

// Destructor:
DimmerWidget::~DimmerWidget()
{
    delete ui;
    delete m_pMonitor;
}

// Paint event:
void DimmerWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    if (!m_bCursorDone)
    {
        QPainter painter(this);
        QSize size = m_currentCursorImage.size();
        QPoint pos = QCursor::pos()-QPoint(size.width()/2, size.height()/2);
        painter.setOpacity(1);
        painter.drawImage(pos, m_currentCursorImage);
    }
    if (!m_bAnimatedImageDone)
    {
        QPainter painter(this);
        QSize size = m_currentAnimatedImage.size();
        QDesktopWidget qDesktopWidget;
        QRect screenSize = qDesktopWidget.screenGeometry();
        QPoint pos = QPoint((screenSize.width()-size.width())/2, (screenSize.height()-size.height())/2);

        Parameters::Strength eStrength = (Parameters::Strength)m_pParameters->parameter(Parameters::SCREEN_BREAK_STRENGTH).toInt();
        double dOpacity = 1.0;
        if (eStrength == Parameters::LIGHT)
            dOpacity = .25;
        else
        if (eStrength == Parameters::MEDIUM)
            dOpacity = .5;
        else
        if (eStrength == Parameters::STRONG)
            dOpacity = 1;

        painter.setOpacity(dOpacity);
        painter.drawImage(pos, m_currentAnimatedImage);
    }
    QDialog::paintEvent(event);
}

// Next cursor image available:
void DimmerWidget::onNextCursorImageAvailable(const QImage &image)
{
    m_currentCursorImage = image;
    update();
}

// Next animated image available:
void DimmerWidget::onNextAnimatedImageAvailable(const QImage &image)
{
    m_currentAnimatedImage = image;
    update();
}

// Animated cursor:
void DimmerWidget::onCursorDone()
{
    m_bCursorDone = true;
    update();
}

// Animated image done:
void DimmerWidget::onAnimatedImageDone()
{
    m_bAnimatedImageDone = true;
    update();
}

// Play cursor:
void DimmerWidget::playCursor()
{
    m_bCursorDone = false;
    m_pAnimatedCursor->play();
}

// Play animated image:
void DimmerWidget::playAnimatedImage(const Parameters::Strength &eStrength)
{
    Q_UNUSED(eStrength);
    m_bAnimatedImageDone = false;
    m_pAnimatedImage->play();
}

// Set temperature:
bool DimmerWidget::setTemperature(int iTemperature)
{
    if (m_bSupportGammaRamp)
        return m_pMonitor->setTemperature(iTemperature);
    return false;
}

// Set parameters:
void DimmerWidget::setParameters(Parameters *pParameters)
{
    m_pParameters = pParameters;
}

// Return monitor:
Monitor *DimmerWidget::monitor()
{
    return m_pMonitor;
}
