// Qt:
#include <QPainter>
#include <QMovie>
#include <QDesktopWidget>
#include <QDebug>
#include <QShowEvent>

// Application:
#include "dimmerwidget.h"
#include "ui_dimmerwidget.h"
#include "animatedcursor.h"
#include "parameters.h"
#include <gammaramp.h>
#include "blynk.h"
#include "controller.h"
#include <fluxlib.h>
#define MAX_LOOP_TIMES 1

// Constructor:
DimmerWidget::DimmerWidget(const QString &sMoviePath, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DimmerWidget),
    m_bDone(false),
    m_pParameters(NULL),
    m_iMinRed(0),
    m_iMaxRed(0),
    m_iMinGreen(0),
    m_iMaxGreen(0),
    m_iMinBlue(0),
    m_iMaxBlue(0)
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
    setGeometry(screenSize);

    // Create movie:
    m_pBigEyeMovie = new QMovie(sMoviePath);
    m_pBigEyeMovie->setCacheMode(QMovie::CacheAll);
    connect(m_pBigEyeMovie, &QMovie::frameChanged, this, &DimmerWidget::onFrameChanged);
    ui->movieArea->hide();
    ui->movieArea->setMovie(m_pBigEyeMovie);

    // Animated cursor:
    m_pAnimatedCursor = new AnimatedCursor(this);
    connect(m_pAnimatedCursor, &AnimatedCursor::nextImage, this, &DimmerWidget::onNextImageAvailable);
    connect(m_pAnimatedCursor, &AnimatedCursor::done, this, &DimmerWidget::onDone);
}

// Destructor:
DimmerWidget::~DimmerWidget()
{
    delete ui;
}

// Frame changed:
void DimmerWidget::onFrameChanged(int iFrameIndex)
{
    static int loopCount = 0;
    if (iFrameIndex == (m_pBigEyeMovie->frameCount()-1))
        loopCount++;
    if (loopCount == 2)
    {
        ui->movieArea->hide();
        m_pBigEyeMovie->stop();
        loopCount = 0;
    }
}

// Update UI:
void DimmerWidget::updateUI()
{
    // No op.
}

// Paint event:
void DimmerWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    if (!m_bDone)
    {
        QPainter painter(this);
        QSize size = m_currentCursorImage.size();
        QPoint pos = QCursor::pos()-QPoint(size.width()/2, size.height()/2);
        painter.drawImage(pos, m_currentCursorImage);
    }
    else QDialog::paintEvent(event);
}

// Next image available:
void DimmerWidget::onNextImageAvailable(const QImage &image)
{
    m_currentCursorImage = image;
    update();
}

// Done:
void DimmerWidget::onDone()
{
    m_bDone = true;
    update();
}

// Play cursor:
void DimmerWidget::playCursor()
{
    m_bDone = false;
    m_pAnimatedCursor->play();
}

// Play big eye:
void DimmerWidget::playBigEye(const Parameters::Strength &eStrength)
{
    QString sStyleSheet("QLabel { color: rgba(0, 0, 0, 100%) }");

    if (eStrength == Parameters::LIGHT)
        sStyleSheet = "QLabel { color: rgba(0, 0, 0, 25%) }";
    else
        if (eStrength == Parameters::MEDIUM)
            sStyleSheet = "QLabel { color: rgba(0, 0, 0, 75%) }";

    ui->movieArea->setStyleSheet(sStyleSheet);
    ui->movieArea->show();
    m_pBigEyeMovie->start();
}


// Set strength:
void DimmerWidget::setStrength(const Parameters::Strength &eStrength)
{
    // Set strength:
    m_eStrength = eStrength;
    setColor(Blynk::instance()->controller()->colorForStrength(eStrength));
}

// Set color:
void DimmerWidget::setColor(const QColor &targetColor)
{
    // Default:
    int iMinRed = 0;
    int iMaxRed = targetColor.red();
    int iMinGreen = 0;
    int iMaxGreen = targetColor.green();
    int iMinBlue = 0;
    int iMaxBlue = targetColor.blue();

    if ((m_iMinRed != iMinRed) || (m_iMaxRed != iMaxRed) ||
            (m_iMinGreen != iMinGreen) || (m_iMaxGreen != iMaxGreen) ||
                (m_iMinBlue != iMinBlue) || (m_iMaxBlue != iMaxBlue))
    {
        m_iMinRed = iMinRed;
        m_iMaxRed = iMaxRed;
        m_iMinGreen = iMinGreen;
        m_iMaxGreen = iMaxGreen;
        m_iMinBlue = iMinBlue;
        m_iMaxBlue = iMaxBlue;
        GammaRamp gammaRamp;
        gammaRamp.setBlueLightReducerParameters(iMinRed, iMaxRed, iMinGreen, iMaxGreen, iMinBlue, iMaxBlue);
    }
}

// Set temperature:
void DimmerWidget::setTemperature(int iTemperature)
{
    setColor(Fluxlib::colorForTemperature(iTemperature));
}

// Return strength:
const Parameters::Strength &DimmerWidget::strength() const
{
    return m_eStrength;
}

// Set parameters:
void DimmerWidget::setParameters(Parameters *pParameters)
{
    m_pParameters = pParameters;
}
