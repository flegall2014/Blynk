// Qt:
#include <QTime>
#include <QTimeEdit>
#include <QDebug>
#include <QCloseEvent>
#include <QToolTip>

// Application:
#include "preferencedialog.h"
#include "ui_preferencedialog.h"
#include "controller.h"
#include "customtooltip.h"

// Constructor:
PreferenceDialog::PreferenceDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PreferenceDialog),
    m_pCustomTooltip(NULL)
{
    // Initialize:
    ui->setupUi(this);

    // Create custom tooltip:
    m_pCustomTooltip = new CustomTooltip();
    m_pCustomTooltip->hide();

    // Set style sheet:
    setStyleSheet(":/css/style.css");

    // Setup splash widget:
    ui->splashWidget->setText("BLYNK", QColor("#1FBBE5"));
    ui->splashWidget->setImage(":/icons/ico-splash.png");

    // Blynk cursor:
    connect(ui->wBlynkCursorSlider, &CustomSlider::valueChanged, this, &PreferenceDialog::onBlynkCursorSliderChanged);
    connect(ui->wBlynkCursorEnabled, &QCheckBox::toggled, this, &PreferenceDialog::onBlynkCursorEnabledToggled);
    connect(ui->wBlynkPerMinuteValues, SIGNAL(activated(int)), this, SLOT(onBlynkPerMinuteChanged(int)));
    connect(ui->wBlynkRandomCheckbox, &QCheckBox::toggled, this, &PreferenceDialog::onSetBlynkCursorRandomMode);

    // Cursor break:
    connect(ui->wScreenBreakSlider, &CustomSlider::valueChanged, this, &PreferenceDialog::onScreenBreakSliderChanged);
    connect(ui->wScreenBreakEnabled, &QCheckBox::toggled, this, &PreferenceDialog::onScreenBreakEnabledToggled);
    connect(ui->wScreenBreakStrengthCombo, SIGNAL(activated(int)), this, SLOT(onScreenBreakStrengthComboChanged(int)));

    // Blue light reducer:
    connect(ui->wBlueLightReducerWidget, &BlueLightReducerWidget::buttonClicked, this, &PreferenceDialog::onBlueLightReducerValueChanged);
    connect(ui->wBlueLightReducerEnabled, &QCheckBox::toggled, this, &PreferenceDialog::onBlueLightReducerEnabledToggled);
    connect(ui->wStartTimeEdit, &QTimeEdit::timeChanged, this, &PreferenceDialog::onStartTimeEditChanged);

    // Start Blynk at login:
    connect(ui->wStartBlynkAtLogin, &QCheckBox::toggled, this, &PreferenceDialog::onStartBlynkAtLoginChanged);

    // Done:
    connect(ui->wDoneButton, &QPushButton::clicked, this, &PreferenceDialog::onDone);
}

// Destructor:
PreferenceDialog::~PreferenceDialog()
{
    delete ui;
}

// Update blynk cursor:
void PreferenceDialog::updateBlynkCursorArea()
{
    // Check enabled state:
    bool bBlynkCursorEnabled = (bool)m_pParameters->parameter(Parameters::BLYNK_CURSOR_ENABLED).toInt();

    // Check random mdode:
    bool bRandomModeOn = (bool)m_pParameters->parameter(Parameters::BLYNK_CURSOR_RANDOM_MODE).toInt();

    // Update slider:
    ui->wBlynkCursorSlider->setEnabled(bBlynkCursorEnabled && !bRandomModeOn);
    ui->wBlynkCursorSlider->setTitle(tr("Regularity (sec)"));
    ui->wBlynkCursorSlider->setRange(m_pParameters->parameter(Parameters::BLYNK_CURSOR_MIN).toInt(),
                                     m_pParameters->parameter(Parameters::BLYNK_CURSOR_MAX).toInt());
    ui->wBlynkCursorSlider->setValue(m_pParameters->parameter(Parameters::BLYNK_CURSOR_REGULARITY).toInt());

    // Update blynk random checkbox:
    ui->wBlynkRandomCheckbox->setEnabled(bBlynkCursorEnabled);
    ui->wBlynkRandomCheckbox->setChecked(bRandomModeOn);

    // Update blynk cursor enabled checkbox:
    QString sBlynkCursorState = "(%1/%2)";
    ui->wBlynkCursorState->setText(bRandomModeOn ?
                                       sBlynkCursorState.arg("Random").arg(bBlynkCursorEnabled ? "Enabled" : "Disabled") :
                                       sBlynkCursorState.arg("Regular").arg(bBlynkCursorEnabled ? "Enabled" : "Disabled"));
    ui->wBlynkCursorEnabled->setChecked(!bBlynkCursorEnabled);

    // Update blynk per minute:
    QString sBlynkPerMinuteRange = m_pParameters->parameter(Parameters::BLYNK_PER_MINUTE_RANGE);
    QStringList lBlynkPerMinuteRange = sBlynkPerMinuteRange.split(",");

    ui->wBlynkPerMinuteValues->setEnabled(bBlynkCursorEnabled && bRandomModeOn);
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
}

// Update screen break:
void PreferenceDialog::updateScreenBreakArea()
{
    // Screen break enabled:
    QString sScreenBreakState = m_pParameters->parameter(Parameters::SCREEN_BREAK_STATE);
    ui->wScreenBreakState->setText(QString("(%1)").arg(sScreenBreakState));
    bool bScreenBreakEnabled = (sScreenBreakState == SCREEN_BREAK_ENABLED);

    // Screen break slider:
    ui->wScreenBreakSlider->setEnabled(bScreenBreakEnabled);
    ui->wScreenBreakSlider->setTitle(tr("Regularity (min)"));
    ui->wScreenBreakSlider->setRange(m_pParameters->parameter(Parameters::SCREEN_BREAK_MIN).toInt(),
                                     m_pParameters->parameter(Parameters::SCREEN_BREAK_MAX).toInt());
    ui->wScreenBreakSlider->setValue(m_pParameters->parameter(Parameters::SCREEN_BREAK_REGULARITY).toInt());

    // Screen break strength combo:
    ui->wScreenBreakStrengthCombo->setEnabled(bScreenBreakEnabled);
    ui->wScreenBreakStrengthCombo->setCurrentIndex(m_pParameters->parameter(Parameters::SCREEN_BREAK_STRENGTH).toInt()-1);

    // Check state:
    ui->wScreenBreakEnabled->setChecked(!bScreenBreakEnabled);
}

// Update blue light reducer:
void PreferenceDialog::updateBlueLightReducerArea()
{
    // Blue light reducer enabled:
    bool bBlueLightReducerEnabled = (bool)m_pParameters->parameter(Parameters::BLUE_LIGHT_REDUCER_ENABLED).toInt();

    // Blue light buttons:
    Parameters::Strength eBlueLightReducerStrength =
            (Parameters::Strength)m_pParameters->parameter(Parameters::BLUE_LIGHT_REDUCER_STRENGTH).toInt();

    QString sBlueLightReducerState = "(%1/%2)";
    ui->wBlueLightReducerState->setText(sBlueLightReducerState.arg(eBlueLightReducerStrength).arg(bBlueLightReducerEnabled ? "Enabled" : "Disabled"));
    ui->wBlueLightReducerWidget->setEnabled(bBlueLightReducerEnabled);
    ui->wBlueLightReducerWidget->setParameters(m_pParameters);
    ui->wBlueLightReducerEnabled->setChecked(!bBlueLightReducerEnabled);

    // Blue light reducer start time:
    ui->wStartTimeEdit->setEnabled(bBlueLightReducerEnabled);
    ui->wStartTimeEdit->setTime(QTime::fromString(m_pParameters->parameter(Parameters::BLUE_LIGHT_REDUCER_START_TIME)));
}

// Update start blynk at login:
void PreferenceDialog::updateStartBlynkAtLoginArea()
{
    ui->wStartBlynkAtLogin->setChecked((bool)m_pParameters->parameter(Parameters::START_BLYNK_AT_LOGIN).toInt());
}

// Update UI:
void PreferenceDialog::updateUI()
{
    // Update blynk cursor:
    updateBlynkCursorArea();

    // Update screen break:
    updateScreenBreakArea();

    // Update blue light reducer:
    updateBlueLightReducerArea();

    // Update login:
    updateStartBlynkAtLoginArea();
}

// Blynk cursor:
void PreferenceDialog::onBlynkCursorSliderChanged(int iValue)
{
    m_pParameters->setParameter(Parameters::BLYNK_CURSOR_REGULARITY, QString::number(iValue));
}

void PreferenceDialog::onBlynkCursorEnabledToggled(bool bChecked)
{
    m_pParameters->setParameter(Parameters::BLYNK_CURSOR_ENABLED, QString::number((int)!bChecked));
    updateBlynkCursorArea();
}

void PreferenceDialog::onBlynkPerMinuteChanged(int iIndex)
{
    QStringList lBlynkPerMinuteValues = ui->wBlynkPerMinuteValues->property("values").toStringList();
    m_pParameters->setParameter(Parameters::BLYNK_PER_MINUTE_RANDOM, QString::number(lBlynkPerMinuteValues[iIndex].toInt()));
}

void PreferenceDialog::onSetBlynkCursorRandomMode(bool bChecked)
{
    m_pParameters->setParameter(Parameters::BLYNK_CURSOR_RANDOM_MODE, QString::number(bChecked));
    ui->wBlynkCursorSlider->setEnabled(!bChecked);
}

// Cursor break:
void PreferenceDialog::onScreenBreakSliderChanged(int iValue)
{
    m_pParameters->setParameter(Parameters::SCREEN_BREAK_REGULARITY, QString::number(iValue));
}

void PreferenceDialog::onScreenBreakEnabledToggled(bool bChecked)
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

void PreferenceDialog::onScreenBreakStrengthComboChanged(int iIndex)
{
    QVector<Parameters::Strength> vStrengths;
    vStrengths << Parameters::LIGHT << Parameters::MEDIUM << Parameters::STRONG;
    m_pParameters->setParameter(Parameters::SCREEN_BREAK_STRENGTH, QString::number(vStrengths[iIndex]));
}

// Blue light reducer:
void PreferenceDialog::onBlueLightReducerValueChanged(const Parameters::Strength &eStrength)
{
    m_pParameters->setParameter(Parameters::BLUE_LIGHT_REDUCER_STRENGTH, QString::number(eStrength));
}

void PreferenceDialog::onBlueLightReducerEnabledToggled(bool bChecked)
{
    m_pParameters->setParameter(Parameters::BLUE_LIGHT_REDUCER_ENABLED, QString::number((int)!bChecked));
    updateBlueLightReducerArea();
}

void PreferenceDialog::onStartTimeEditChanged(const QTime &tTime)
{
    m_pParameters->setParameter(Parameters::BLUE_LIGHT_REDUCER_START_TIME, tTime.toString());
}

// Start Blynk at login:
void PreferenceDialog::onStartBlynkAtLoginChanged(bool bChecked)
{
    m_pParameters->setParameter(Parameters::START_BLYNK_AT_LOGIN, QString::number((int)bChecked));
}

// Done:
void PreferenceDialog::onDone()
{
    m_pCustomTooltip->hide();
    hide();
}

// Handle close event:
void PreferenceDialog::closeEvent(QCloseEvent *e)
{
    hide();
    e->ignore();
}

// Set parameters:
void PreferenceDialog::setParameters(Parameters *pParameters)
{
    m_pParameters = pParameters;
}

// Set tooltips:
void PreferenceDialog::setTooltips(const QMap<QString, QString> &mTooltipValues)
{
    m_mTooltips = mTooltipValues;
}

// Handle event for tooltip display:
bool PreferenceDialog::event(QEvent *event)
{
    // Handle tooltip:
    if (event->type() == QEvent::ToolTip)
    {
        QHelpEvent *helpEvent = static_cast<QHelpEvent *>(event);
        QPushButton *pButtonUnderMouse = dynamic_cast<QPushButton *>(childAt(helpEvent->pos()));

        if (pButtonUnderMouse != NULL)
        {
            if (m_pCustomTooltip)
            {
                QString sTooltipText = m_mTooltips[pButtonUnderMouse->objectName()];
                if (!sTooltipText.isEmpty())
                {
                    m_pCustomTooltip->setText(sTooltipText);
                    m_pCustomTooltip->move(mapToGlobal(helpEvent->pos()));
                    m_pCustomTooltip->show();
                }
            }
        }
        else
        {
            m_pCustomTooltip->hide();
            event->ignore();
        }

        return true;
    }

    if (!containsMouse(QCursor::pos()) && m_pCustomTooltip && m_pCustomTooltip->isVisible())
        m_pCustomTooltip->hide();

    return QWidget::event(event);
}

bool PreferenceDialog::containsMouse(const QPoint &point)
{
    return rect().contains(mapFromGlobal(point));
}
