// Qt:
#include <QWidget>
#include <QFontDatabase>
#include <QPainter>
#include <QDebug>

// Application:
#include "blynkwindow.h"
#include "parameters.h"
#include "utils.h"

// Create monitor:
#ifdef Q_OS_WIN
#include "ui_blynkwindow.h"
#elif defined(Q_OS_OSX)
#include "ui_blynkwindow-mac.h"
#endif

// Constructor:
BlynkWindow::BlynkWindow(const QString &sTitle, QWidget *parent) :
    CustomWindow(sTitle, parent),
    ui(new Ui::BlynkWindow),
    m_pParameters(NULL)
{
    ui->setupUi(this);
    ui->mainLayout->setContentsMargins(16, 32, 16, 16);

    // Set font for widget:
    Utils::setFontForWidget(this);

    ui->wBlynkRegularitySlider->setPageStep(1);
    ui->wScreenBreakSlider->setPageStep(5);

    // Set blynk slider labels:
    QStringList lLabels1;
    lLabels1 << "2 sec" << "4" << "6" << "8" << "10";
    ui->wBlynkRegularitySlider->setMajorTickLabels(lLabels1);

    // Set screen break slider labels:
    QStringList lLabels2;
    lLabels2 << "20 min" << "40" << "60" << "80" << "100";
    ui->wScreenBreakSlider->setMajorTickLabels(lLabels2);

    // Set logo:
    ui->wLogoArea->setImage(":/icons/ico-blynklogo.png");

    // Blynk cursor:
    connect(ui->wBlynkRegularitySlider, &Slider::valueChanged, this, &BlynkWindow::onBlynkRegularitySliderChanged);
    connect(ui->wBlynkRandomCheckBox, &QCheckBox::toggled, this, &BlynkWindow::onRandomCheckBoxToggled);
    connect(ui->wBlynkCursorEnabled, &QCheckBox::toggled, this, &BlynkWindow::onBlynkCursorEnabledChanged);
    connect(ui->wBlynkPerMinuteValues, SIGNAL(activated(QString)), this, SLOT(onBlynkPerMinuteValueChanged(QString)));

    // Screen break area:
    connect(ui->wScreenBreakSlider, &Slider::valueChanged, this, &BlynkWindow::onScreenBreakRegularityChanged);
    connect(ui->wScreenBreakEnabled, &QCheckBox::toggled, this, &BlynkWindow::onScreenBreakEnabledChanged);
    connect(ui->wScreenBreakStrengthCombo, SIGNAL(activated(int)), this, SLOT(onScreenBreakStrengthChanged(int)));

    // Blue light reducer:
    connect(ui->wBlueLightReducerSlider, &QSlider::valueChanged, this, &BlynkWindow::onBlueLightReducerValueChanged);
    connect(ui->wBlueLightReducerEnabled, &QCheckBox::toggled, this, &BlynkWindow::onBlueLightReducerEnabledChanged);
    connect(ui->wStartTimeEdit, &QTimeEdit::timeChanged, this, &BlynkWindow::onStartTimeEditChanged);
    connect(ui->wAlwaysOnCheckBox, &QCheckBox::clicked, this, &BlynkWindow::onAlwaysOnChanged);

    // Start blynk at login:
    connect(ui->wStartBlynkAtLoginCheckBox, &QCheckBox::toggled, this, &BlynkWindow::onStartBlynkAtLoginChanged);
    connect(ui->wDoneButton, &QPushButton::clicked, this, &BlynkWindow::onDone);
}

// Destructor:
BlynkWindow::~BlynkWindow()
{
    delete ui;
}

// Set parameters:
void BlynkWindow::setParameters(Parameters *pParameters)
{
    // Set parameters:
    m_pParameters = pParameters;

    // Set blynk cursor range:
    QString sBlynkRegularityRange = m_pParameters->parameter(Parameters::BLYNK_REGULARITY_RANGE);
    QStringList lBlynkRegularityRange = sBlynkRegularityRange.split(",");

    // Read min and max values:
    int iMinValue = qMin(lBlynkRegularityRange.first().toInt(), lBlynkRegularityRange[1].toInt());
    int iMaxValue = qMax(lBlynkRegularityRange.first().toInt(), lBlynkRegularityRange[1].toInt());
    int iBlynkRegularity = m_pParameters->parameter(Parameters::BLYNK_CURSOR_REGULARITY).toInt();
    ui->wBlynkRegularitySlider->blockSignals(true);
    ui->wBlynkRegularitySlider->setRange(iMinValue, iMaxValue);
    ui->wBlynkRegularitySlider->blockSignals(false);
    ui->wBlynkRegularitySlider->setValue(iBlynkRegularity);

    // Screen break regularity:
    int iScreenBreakRegularity = m_pParameters->parameter(Parameters::SCREEN_BREAK_REGULARITY).toInt();
    ui->wScreenBreakSlider->blockSignals(true);
    ui->wScreenBreakSlider->setRange(m_pParameters->parameter(Parameters::SCREEN_BREAK_MIN).toInt(),
                                     m_pParameters->parameter(Parameters::SCREEN_BREAK_MAX).toInt());
    ui->wScreenBreakSlider->blockSignals(false);
    ui->wScreenBreakSlider->setValue(iScreenBreakRegularity);

    setEnabledState();
}

// Update UI:
void BlynkWindow::updateUI()
{
    updateBlynkCursorArea();
    updateScreenBreakArea();
    updateBlueLightReducerArea();
    updateStartBlynkAtLoginArea();
}

// Paint:
void BlynkWindow::paintEvent(QPaintEvent *event)
{
    CustomWindow::paintEvent(event);
    QPainter painter(this);
    QPoint mapped = ui->wSeparator->mapToParent(QPoint(0, 0));
    painter.setPen(QPen(QColor("#A5A5A7")));
    painter.drawLine(QPoint(20, mapped.y()), QPoint(rect().width()-20, mapped.y()));
}

// Update blynk cursor area:
void BlynkWindow::updateBlynkCursorArea()
{
    // Blynk cursor enabled:
    QString sBlynkCursorState = m_pParameters->parameter(Parameters::BLYNK_CURSOR_STATE);
    bool bBlynkCursorEnabled = (sBlynkCursorState == BLYNK_CURSOR_ENABLED);

    // Check random mode:
    bool bRandomModeOn = (bool)m_pParameters->parameter(Parameters::BLYNK_CURSOR_RANDOM_MODE).toInt();

    // Update blynk random checkbox:
    ui->wBlynkRandomCheckBox->setChecked(bRandomModeOn);

    // Update blynk cursor enabled checkbox:
    ui->wBlynkCursorEnabled->setChecked(!bBlynkCursorEnabled);

    // Update blynk per minute:
    QString sBlynkPerMinuteRange = m_pParameters->parameter(Parameters::BLYNK_PER_MINUTE_RANGE);
    QStringList lBlynkPerMinuteRange = sBlynkPerMinuteRange.split(",");
    QStringList lBlynkPerMinuteValues;
    for (int i=lBlynkPerMinuteRange[0].toInt(); i<=lBlynkPerMinuteRange[1].toInt(); i++)
        lBlynkPerMinuteValues << QString::number(i);
    if ((ui->wBlynkPerMinuteValues->count() == 0) && (lBlynkPerMinuteRange.size() > 1)) {
        ui->wBlynkPerMinuteValues->insertItems(0, lBlynkPerMinuteValues);
    }
    QString sCurrentBlynkPerMinuteRandom = QString::number(m_pParameters->parameter(Parameters::BLYNK_PER_MINUTE_RANDOM).toInt());
    ui->wBlynkPerMinuteValues->setCurrentIndex(lBlynkPerMinuteValues.indexOf(sCurrentBlynkPerMinuteRandom));

    int iBlynkCursorFreq = m_pParameters->parameter(Parameters::BLYNK_CURSOR_REGULARITY).toInt();
    ui->wBlynkRegularitySlider->setValue(iBlynkCursorFreq);
}

// Update screen break area:
void BlynkWindow::updateScreenBreakArea()
{
    // Screen break enabled:
    QString sScreenBreakState = m_pParameters->parameter(Parameters::SCREEN_BREAK_STATE);
    bool bScreenBreakEnabled = (sScreenBreakState == SCREEN_BREAK_ENABLED);

    // Screen break strength combo:
    Parameters::Strength eScreenBreakStrength = (Parameters::Strength)m_pParameters->parameter(Parameters::SCREEN_BREAK_STRENGTH).toInt();
    ui->wScreenBreakStrengthCombo->setCurrentIndex((int)eScreenBreakStrength);

    // Check state:
    ui->wScreenBreakEnabled->setChecked(!bScreenBreakEnabled);
}

// Update blue light reducer area:
void BlynkWindow::updateBlueLightReducerArea()
{
    // Blue light reducer enabled:
    QString sBlueLightReducerState = m_pParameters->parameter(Parameters::BLUELIGHTREDUCER_STATE);
    bool bBlueLightReducerEnabled = (sBlueLightReducerState == BLUELIGHTREDUCER_ENABLED);

    // Blue light reducer start time:
    ui->wStartTimeEdit->setEnabled(bBlueLightReducerEnabled);
    ui->wStartTimeEdit->setTime(QTime::fromString(m_pParameters->parameter(Parameters::BLUE_LIGHT_REDUCER_START_TIME)));

    // Blue light reducer slider:
    Parameters::Strength eBlueLightReducerStrength = (Parameters::Strength)m_pParameters->parameter(Parameters::BLUE_LIGHT_REDUCER_STRENGTH).toInt();
    ui->wBlueLightReducerSlider->setEnabled(bBlueLightReducerEnabled);
    ui->wBlueLightReducerSlider->setValue(eBlueLightReducerStrength);

    // Always on checkbox:
    ui->wAlwaysOnCheckBox->setChecked(m_pParameters->parameter(Parameters::BLUE_LIGHT_REDUCER_ALWAYS_ON) == ON);

    // Check state:
    ui->wBlueLightReducerEnabled->setChecked(!bBlueLightReducerEnabled);
}

// Update start blynk at login area:
void BlynkWindow::updateStartBlynkAtLoginArea()
{
    bool bStartBlynkAtLogin = (bool)m_pParameters->parameter(Parameters::START_BLYNK_AT_LOGIN).toInt();
    ui->wStartBlynkAtLoginCheckBox->setChecked(bStartBlynkAtLogin);
}

// Update title:
void BlynkWindow::updateTitle(int iTemperature)
{
    QString sTitle = QString("BLUE LIGHT REDUCER: %1").arg(iTemperature);
    ui->wBlueLightReducerLabel->setText(sTitle);
}

// Blynk regularity slider changed:
void BlynkWindow::onBlynkRegularitySliderChanged(int iRegularity)
{
    m_pParameters->setParameter(Parameters::BLYNK_CURSOR_REGULARITY, QString::number(iRegularity));
}

// Random checkbox toggled:
void BlynkWindow::onRandomCheckBoxToggled(bool bChecked)
{
    m_pParameters->setParameter(Parameters::BLYNK_CURSOR_RANDOM_MODE, bChecked ? ON : OFF);
    setEnabledState();
}

// Blue light reducer enabled changed:
void BlynkWindow::onBlynkCursorEnabledChanged(bool bChecked)
{
    bool bEnabled = !bChecked;
    if (bEnabled)
        m_pParameters->setParameter(Parameters::BLYNK_CURSOR_STATE, BLYNK_CURSOR_ENABLED);
    else
    {
        QString sBlynkCursorState = m_pParameters->parameter(Parameters::BLYNK_CURSOR_STATE);
        if ((sBlynkCursorState != BLYNK_CURSOR_DISABLED_FOR_ONE_HOUR) &&
                (sBlynkCursorState != BLYNK_CURSOR_DISABLED_FOR_THREE_HOURS) &&
                (sBlynkCursorState != BLYNK_CURSOR_DISABLED_UNTIL_TOMORROW))
            m_pParameters->setParameter(Parameters::BLYNK_CURSOR_STATE, BLYNK_CURSOR_DISABLED);
    }
    setEnabledState();
}

// Blynk per minute value changed:
void BlynkWindow::onBlynkPerMinuteValueChanged(const QString &sBlynkPerMinute)
{
    m_pParameters->setParameter(Parameters::BLYNK_PER_MINUTE_RANDOM, sBlynkPerMinute);
}

// Screen break regularity changed:
void BlynkWindow::onScreenBreakRegularityChanged(int iScreenBreakRegularity)
{
    m_pParameters->setParameter(Parameters::SCREEN_BREAK_REGULARITY, QString::number(iScreenBreakRegularity));
}

// Screen break enabled changed:
void BlynkWindow::onScreenBreakEnabledChanged(bool bChecked)
{
    bool bEnabled = !bChecked;
    if (bEnabled)
        m_pParameters->setParameter(Parameters::SCREEN_BREAK_STATE, SCREEN_BREAK_ENABLED);
    else
    {
        QString sScreenBreakState = m_pParameters->parameter(Parameters::SCREEN_BREAK_STATE);
        if ((sScreenBreakState != SCREEN_BREAK_DISABLED_FOR_ONE_HOUR) &&
                (sScreenBreakState != SCREEN_BREAK_DISABLED_FOR_THREE_HOURS) &&
                (sScreenBreakState != SCREEN_BREAK_DISABLED_UNTIL_TOMORROW))
            m_pParameters->setParameter(Parameters::SCREEN_BREAK_STATE, SCREEN_BREAK_DISABLED);
    }
    setEnabledState();
}

// Screen break strength changed:
void BlynkWindow::onScreenBreakStrengthChanged(int iIndex)
{
    Parameters::Strength eStrength = (Parameters::Strength)(iIndex);
    m_pParameters->setParameter(Parameters::SCREEN_BREAK_STRENGTH, QString::number(eStrength));
}

// Blue light reducer value changed:
void BlynkWindow::onBlueLightReducerValueChanged(int iBlueLightReducerValue)
{
    Parameters::Strength eStrength = (Parameters::Strength)(iBlueLightReducerValue);
    m_pParameters->setParameter(Parameters::BLUE_LIGHT_REDUCER_STRENGTH, QString::number(eStrength));
}

// Blue light reducer enabled changed:
void BlynkWindow::onBlueLightReducerEnabledChanged(bool bChecked)
{
    bool bEnabled = !bChecked;
    if (bEnabled)
        m_pParameters->setParameter(Parameters::BLUELIGHTREDUCER_STATE, BLUELIGHTREDUCER_ENABLED);
    else
    {
        QString sBlueLightReducerState = m_pParameters->parameter(Parameters::BLUELIGHTREDUCER_STATE);
        if ((sBlueLightReducerState != BLUELIGHTREDUCER_DISABLED_FOR_ONE_HOUR) &&
                (sBlueLightReducerState != BLUELIGHTREDUCER_DISABLED_FOR_THREE_HOURS) &&
                (sBlueLightReducerState != BLUELIGHTREDUCER_DISABLED_UNTIL_TOMORROW))
            m_pParameters->setParameter(Parameters::BLUELIGHTREDUCER_STATE, BLUELIGHTREDUCER_DISABLED);
    }
    setEnabledState();
}

// Start time changed:
void BlynkWindow::onStartTimeEditChanged(const QTime &tTime)
{
    m_pParameters->setParameter(Parameters::BLUE_LIGHT_REDUCER_START_TIME, tTime.toString());
}

// Always on changed:
void BlynkWindow::onAlwaysOnChanged(bool bChecked)
{
    m_pParameters->setParameter(Parameters::BLUE_LIGHT_REDUCER_ALWAYS_ON,
        bChecked ? ON : OFF);
}

// Start blynk at login changed:
void BlynkWindow::onStartBlynkAtLoginChanged(bool bChecked)
{
    m_pParameters->setParameter(Parameters::START_BLYNK_AT_LOGIN, QString::number((int)bChecked));
}

// Done:
void BlynkWindow::onDone()
{
    onCloseButtonClicked();
}

// Set enabled state:
void BlynkWindow::setEnabledState()
{
    /*** BLYNK CURSOR AREA ***/

    // Blynk cursor enabled:
    QString sBlynkCursorState = m_pParameters->parameter(Parameters::BLYNK_CURSOR_STATE);
    bool bBlynkCursorEnabled = (sBlynkCursorState == BLYNK_CURSOR_ENABLED);

    // Check random mode:
    bool bRandomModeOn = (bool)m_pParameters->parameter(Parameters::BLYNK_CURSOR_RANDOM_MODE).toInt();
    ui->wBlynkRegularitySlider->setEnabled(bBlynkCursorEnabled && !bRandomModeOn);
    ui->wBlynkRandomCheckBox->setEnabled(bBlynkCursorEnabled);
    ui->wBlynkPerMinuteValues->setEnabled(bBlynkCursorEnabled && bRandomModeOn);

    /*** SCREEN BREAK AREA ***/

    // Screen break enabled:
    QString sScreenBreakState = m_pParameters->parameter(Parameters::SCREEN_BREAK_STATE);
    bool bScreenBreakEnabled = (sScreenBreakState == SCREEN_BREAK_ENABLED);
    ui->wScreenBreakSlider->setEnabled(bScreenBreakEnabled);
    ui->wScreenBreakStrengthCombo->setEnabled(bScreenBreakEnabled);

    /*** BLUE LIGHT REDUCER ***/

    // Blue light reducer enabled:
    QString sBlueLightReducerState = m_pParameters->parameter(Parameters::BLUELIGHTREDUCER_STATE);
    bool bBlueLightReducerEnabled = (sBlueLightReducerState == BLUELIGHTREDUCER_ENABLED);
    ui->wBlueLightReducerSlider->setEnabled(bBlueLightReducerEnabled);
    ui->wStartTimeEdit->setEnabled(bBlueLightReducerEnabled);
    ui->wAlwaysOnCheckBox->setEnabled(bBlueLightReducerEnabled);
}
