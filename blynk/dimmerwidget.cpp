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

    // Default:
    unsigned short iMinRed = m_pParameters->parameter(Parameters::BLUE_CHANNEL_MIN_DEFAULT).toUShort();
    unsigned short iMaxRed = m_pParameters->parameter(Parameters::BLUE_CHANNEL_MAX_DEFAULT).toUShort();
    unsigned short iMinGreen = m_pParameters->parameter(Parameters::BLUE_CHANNEL_MIN_DEFAULT).toUShort();
    unsigned short iMaxGreen = m_pParameters->parameter(Parameters::BLUE_CHANNEL_MAX_DEFAULT).toUShort();
    unsigned short iMinBlue = m_pParameters->parameter(Parameters::BLUE_CHANNEL_MIN_DEFAULT).toUShort();
    unsigned short iMaxBlue = m_pParameters->parameter(Parameters::BLUE_CHANNEL_MAX_DEFAULT).toUShort();

    // Set iMinBlue/iMaxBlue based on strength:
    switch (eStrength) {
    case Parameters::LIGHT: {
        iMinRed = m_pParameters->parameter(Parameters::RED_CHANNEL_MIN_LIGHT).toInt();
        iMaxRed = m_pParameters->parameter(Parameters::RED_CHANNEL_MAX_LIGHT).toInt();
        iMinGreen = m_pParameters->parameter(Parameters::GREEN_CHANNEL_MIN_LIGHT).toInt();
        iMaxGreen = m_pParameters->parameter(Parameters::GREEN_CHANNEL_MAX_LIGHT).toInt();
        iMinBlue = m_pParameters->parameter(Parameters::BLUE_CHANNEL_MIN_LIGHT).toInt();
        iMaxBlue = m_pParameters->parameter(Parameters::BLUE_CHANNEL_MAX_LIGHT).toInt();
        break;
    }
    case Parameters::MEDIUM: {
        iMinRed = m_pParameters->parameter(Parameters::RED_CHANNEL_MIN_MEDIUM).toInt();
        iMaxRed = m_pParameters->parameter(Parameters::RED_CHANNEL_MAX_MEDIUM).toInt();
        iMinGreen = m_pParameters->parameter(Parameters::GREEN_CHANNEL_MIN_MEDIUM).toInt();
        iMaxGreen = m_pParameters->parameter(Parameters::GREEN_CHANNEL_MAX_MEDIUM).toInt();
        iMinBlue = m_pParameters->parameter(Parameters::BLUE_CHANNEL_MIN_MEDIUM).toInt();
        iMaxBlue = m_pParameters->parameter(Parameters::BLUE_CHANNEL_MAX_MEDIUM).toInt();
        break;
    }
    case Parameters::STRONG: {
        iMinRed = m_pParameters->parameter(Parameters::RED_CHANNEL_MIN_STRONG).toInt();
        iMaxRed = m_pParameters->parameter(Parameters::RED_CHANNEL_MAX_STRONG).toInt();
        iMinGreen = m_pParameters->parameter(Parameters::GREEN_CHANNEL_MIN_STRONG).toInt();
        iMaxGreen = m_pParameters->parameter(Parameters::GREEN_CHANNEL_MAX_STRONG).toInt();
        iMinBlue = m_pParameters->parameter(Parameters::BLUE_CHANNEL_MIN_STRONG).toInt();
        iMaxBlue = m_pParameters->parameter(Parameters::BLUE_CHANNEL_MAX_STRONG).toInt();
        break;
    }
    default:
        break;
    }

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
