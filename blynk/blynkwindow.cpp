// Qt:
#include <QWidget>
#include <QFontDatabase>

// Application:
#include "blynkwindow.h"
#include "ui_blynkwindow.h"
#include "parameters.h"
#include <QDebug>

// Constructor:
BlynkWindow::BlynkWindow(const QString &sTitle, QWidget *parent) :
    CustomWindow(sTitle, parent),
    ui(new Ui::BlynkWindow),
    m_pParameters(NULL)
{
    ui->setupUi(this);
    ui->wBlynkRegularitySlider->setPageStep(1);
    ui->wScreenBreakSlider->setPageStep(5);

    QList<QLabel *> lLabels = findChildren<QLabel *>();;
    qDebug() << lLabels;

    // Myriad font:
    int id = QFontDatabase::addApplicationFont(":/fonts/Myriad-Pro_31655.ttf");
    qDebug() << id;
    QString sMyriadFamily = QFontDatabase::applicationFontFamilies(id).at(0);
    QFont myriadFont;
    myriadFont.setFamily(sMyriadFamily);
    myriadFont.setPixelSize(48);

    foreach (QLabel *pLabel, lLabels) {
        pLabel->setFont(myriadFont);
    }
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
    connect(ui->wBlynkPerMinuteValues, SIGNAL(activated(int)), this, SLOT(onBlynkPerMinuteValueChanged(int)));

    // Screen break area:
    connect(ui->wScreenBreakSlider, &Slider::valueChanged, this, &BlynkWindow::onScreenBreakRegularityChanged);
    connect(ui->wScreenBreakEnabled, &QCheckBox::toggled, this, &BlynkWindow::onScreenBreakEnabledChanged);
    connect(ui->wScreenBreakStrengthCombo, SIGNAL(activated(int)), this, SLOT(onScreenBreakStrengthChanged(int)));

    // Blue light reducer:
    connect(ui->wBlueLightReducerSlider, &QSlider::valueChanged, this, &BlynkWindow::onBlueLightReducerValueChanged);
    connect(ui->wBlueLightReducerEnabled, &QCheckBox::toggled, this, &BlynkWindow::onBlueLightReducerEnabledChanged);
    connect(ui->wStartTimeEdit, &QTimeEdit::timeChanged, this, &BlynkWindow::onStartTimeEditChanged);

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
    m_pParameters = pParameters;

    ui->wScreenBreakSlider->setRange(m_pParameters->parameter(Parameters::SCREEN_BREAK_MIN).toInt(),
        m_pParameters->parameter(Parameters::SCREEN_BREAK_MAX).toInt());
    ui->wScreenBreakSlider->setValue(m_pParameters->parameter(Parameters::SCREEN_BREAK_REGULARITY).toInt());
}

// Update UI:
void BlynkWindow::updateUI()
{
    updateBlynkCursorArea();
    updateScreenBreakArea();
    updateBlueLightReducerArea();
    updateStartBlynkAtLoginArea();
}

// Update blynk cursor area:
void BlynkWindow::updateBlynkCursorArea()
{
    // Blynk cursor enabled:
    QString sBlynkCursorState = m_pParameters->parameter(Parameters::BLYNK_CURSOR_STATE);
    bool bBlynkCursorEnabled = (sBlynkCursorState == BLYNK_CURSOR_ENABLED);

    // Check random mode:
    bool bRandomModeOn = (bool)m_pParameters->parameter(Parameters::BLYNK_CURSOR_RANDOM_MODE).toInt();

    // Read blynk regularity range:
    QString sBlynkRegularityRange = m_pParameters->parameter(Parameters::BLYNK_REGULARITY_RANGE);
    QStringList lBlynkRegularityRange = sBlynkRegularityRange.split(",");
    if (lBlynkRegularityRange.size() > 1)
    {
        int iFirst = lBlynkRegularityRange.first().toInt();
        int iSecond = lBlynkRegularityRange[1].toInt();
        int iMinValue = qMin(iFirst, iSecond);
        int iMaxValue = qMax(iFirst, iSecond);
        ui->wBlynkRegularitySlider->setRange(iMinValue, iMaxValue);

        int iBlynkRegularity = m_pParameters->parameter(Parameters::BLYNK_CURSOR_REGULARITY).toInt();
        ui->wBlynkRegularitySlider->setValue(iBlynkRegularity);
    }

    // Update blynk random checkbox:
    ui->wBlynkRandomCheckBox->setChecked(bRandomModeOn);
    ui->wBlynkRandomCheckBox->setEnabled(bBlynkCursorEnabled);

    // Update blynk cursor enabled checkbox:
    ui->wBlynkCursorEnabled->setChecked(!bBlynkCursorEnabled);

    // Update blynk per minute:
    QString sBlynkPerMinuteRange = m_pParameters->parameter(Parameters::BLYNK_PER_MINUTE_RANGE);
    QStringList lBlynkPerMinuteRange = sBlynkPerMinuteRange.split(",");

    if (lBlynkPerMinuteRange.size() > 1)
    {
        QStringList lBlynkPerMinuteValues;
        for (int i=lBlynkPerMinuteRange[0].toInt(); i<=lBlynkPerMinuteRange[1].toInt(); i++)
            lBlynkPerMinuteValues << QString::number(i);
        ui->wBlynkPerMinuteValues->insertItems(0, lBlynkPerMinuteValues);
        ui->wBlynkPerMinuteValues->setProperty("values", lBlynkPerMinuteValues);
        QString sCurrentBlynkPerMinuteRandom = QString::number(m_pParameters->parameter(Parameters::BLYNK_PER_MINUTE_RANDOM).toInt());
        ui->wBlynkPerMinuteValues->setCurrentIndex(lBlynkPerMinuteValues.indexOf(sCurrentBlynkPerMinuteRandom));
    }
    ui->wBlynkPerMinuteValues->setEnabled(bBlynkCursorEnabled);
}

// Update screen break area:
void BlynkWindow::updateScreenBreakArea()
{
    // Screen break enabled:
    QString sScreenBreakState = m_pParameters->parameter(Parameters::SCREEN_BREAK_STATE);
    bool bScreenBreakEnabled = (sScreenBreakState == SCREEN_BREAK_ENABLED);

    // Screen break slider:
    ui->wScreenBreakSlider->setEnabled(bScreenBreakEnabled);

    // Screen break strength combo:
    Parameters::Strength eScreenBreakStrength = (Parameters::Strength)m_pParameters->parameter(Parameters::SCREEN_BREAK_STRENGTH).toInt();
    ui->wScreenBreakStrengthCombo->setEnabled(bScreenBreakEnabled);
    if (eScreenBreakStrength > Parameters::NO_STRENGTH)
        ui->wScreenBreakStrengthCombo->setCurrentIndex((int)eScreenBreakStrength-1);

    // Check state:
    ui->wScreenBreakEnabled->setChecked(!bScreenBreakEnabled);
}

// Update blue light reducer area:
void BlynkWindow::updateBlueLightReducerArea()
{
    // Blue light reducer enabled:
    bool bBlueLightReducerEnabled = (bool)m_pParameters->parameter(Parameters::BLUE_LIGHT_REDUCER_ENABLED).toInt();

    ui->wBlueLightReducerSlider->setEnabled(bBlueLightReducerEnabled);
    ui->wBlueLightReducerEnabled->setChecked(!bBlueLightReducerEnabled);

    // Blue light reducer start time:
    ui->wStartTimeEdit->setEnabled(bBlueLightReducerEnabled);
    ui->wStartTimeEdit->setTime(QTime::fromString(m_pParameters->parameter(Parameters::BLUE_LIGHT_REDUCER_START_TIME)));
}

// Update start blynk at login area:
void BlynkWindow::updateStartBlynkAtLoginArea()
{
    bool bStartBlynkAtLogin = (bool)m_pParameters->parameter(Parameters::START_BLYNK_AT_LOGIN).toInt();
    ui->wStartBlynkAtLoginCheckBox->setChecked(bStartBlynkAtLogin);
}

// Blynk regularity slider changed:
void BlynkWindow::onBlynkRegularitySliderChanged(int iRegularity)
{
    m_pParameters->setParameter(Parameters::BLYNK_CURSOR_RANDOM_MODE, "0");
    ui->wBlynkRandomCheckBox->setChecked(false);
    m_pParameters->setParameter(Parameters::BLYNK_CURSOR_REGULARITY, QString::number(iRegularity));
}

// Random checkbox toggled:
void BlynkWindow::onRandomCheckBoxToggled(bool bChecked)
{
    m_pParameters->setParameter(Parameters::BLYNK_CURSOR_RANDOM_MODE, bChecked ? "1" : "0");
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
    updateBlynkCursorArea();
}

// Blynk per minute value changed:
void BlynkWindow::onBlynkPerMinuteValueChanged(int iBlynkPerMinute)
{
    m_pParameters->setParameter(Parameters::BLYNK_PER_MINUTE_RANDOM, QString::number(iBlynkPerMinute));
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
    updateScreenBreakArea();
}


// Screen break strength changed:
void BlynkWindow::onScreenBreakStrengthChanged(int iIndex)
{
    Parameters::Strength eStrength = (Parameters::Strength)(iIndex+1);
    m_pParameters->setParameter(Parameters::SCREEN_BREAK_STRENGTH, QString::number(eStrength));
}

// Blue light reducer value changed:
void BlynkWindow::onBlueLightReducerValueChanged(int iBlueLightReducerValue)
{
    Parameters::Strength eStrength = (Parameters::Strength)(iBlueLightReducerValue+1);
    m_pParameters->setParameter(Parameters::BLUE_LIGHT_REDUCER_STRENGTH, QString::number(eStrength));
}

// Blue light reducer enabled changed:
void BlynkWindow::onBlueLightReducerEnabledChanged(bool bChecked)
{
    m_pParameters->setParameter(Parameters::BLUE_LIGHT_REDUCER_ENABLED, QString::number((int)!bChecked));
    updateBlueLightReducerArea();
}

// Start time changed:
void BlynkWindow::onStartTimeEditChanged(const QTime &tTime)
{
    m_pParameters->setParameter(Parameters::BLUE_LIGHT_REDUCER_START_TIME, tTime.toString());
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

