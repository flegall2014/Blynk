// Qt:
#include <QDebug>

// Application:
#include "screenbreakpage.h"
#include "ui_screenbreakpage.h"
#include "parameters.h"

// Constructor:
ScreenBreakPage::ScreenBreakPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ScreenBreakPage),
    m_pButtonGroup(NULL),
    m_pParameters(NULL)
{
    ui->setupUi(this);
    m_pButtonGroup = new QButtonGroup(this);
    m_pButtonGroup->addButton(ui->wLightRadioButton);
    m_pButtonGroup->addButton(ui->wMediumRadioButton);
    m_pButtonGroup->addButton(ui->wStrongRadioButton);

    connect(ui->wScreenBreakSlider, &CustomSlider::valueChanged, this, &ScreenBreakPage::onScreenBreakSliderChanged);
    connect(ui->wScreenBreakEnabled, &QCheckBox::toggled, this, &ScreenBreakPage::onScreenBreakEnabledToggled);
    connect(m_pButtonGroup, SIGNAL(buttonClicked(QAbstractButton*)), this, SLOT(onButtonClicked(QAbstractButton*)));
}

// Destructor:
ScreenBreakPage::~ScreenBreakPage()
{
    delete ui;
}

// Set parameters:
void ScreenBreakPage::setParameters(Parameters *pParameters)
{
    m_pParameters = pParameters;
    ui->wScreenBreakSlider->setRange(m_pParameters->parameter(Parameters::SCREEN_BREAK_MIN).toInt(),
        m_pParameters->parameter(Parameters::SCREEN_BREAK_MAX).toInt());
    ui->wScreenBreakSlider->setValue(m_pParameters->parameter(Parameters::SCREEN_BREAK_REGULARITY).toInt());
}

// Update UI:
void ScreenBreakPage::updateUI()
{
    // Screen break enabled:
    QString sScreenBreakState = m_pParameters->parameter(Parameters::SCREEN_BREAK_STATE);
    bool bScreenBreakEnabled = (sScreenBreakState == SCREEN_BREAK_ENABLED);

    // Screen break slider:
    ui->wScreenBreakSlider->setEnabled(bScreenBreakEnabled);
    ui->wScreenBreakSlider->setTitle(tr("Regularity (min):"));

    // Screen break strength combo:
    Parameters::Strength eScreenBreakStrength = (Parameters::Strength)m_pParameters->parameter(Parameters::SCREEN_BREAK_STRENGTH).toInt();
    ui->wLightRadioButton->setEnabled(bScreenBreakEnabled);
    ui->wLightRadioButton->setChecked(eScreenBreakStrength == Parameters::LIGHT);
    ui->wMediumRadioButton->setEnabled(bScreenBreakEnabled);
    ui->wMediumRadioButton->setChecked(eScreenBreakStrength == Parameters::MEDIUM);
    ui->wStrongRadioButton->setEnabled(bScreenBreakEnabled);
    ui->wStrongRadioButton->setChecked(eScreenBreakStrength == Parameters::STRONG);

    // Check state:
    ui->wScreenBreakEnabled->setChecked(!bScreenBreakEnabled);
}

// Button clicked:
void ScreenBreakPage::onButtonClicked(QAbstractButton *pButton)
{
    QString sButtonName = pButton->objectName();
    if (sButtonName == "wLightRadioButton") {
        m_pParameters->setParameter(Parameters::SCREEN_BREAK_STRENGTH, QString::number(Parameters::LIGHT));
    }
    else
    if (sButtonName == "wMediumRadioButton") {
        m_pParameters->setParameter(Parameters::SCREEN_BREAK_STRENGTH, QString::number(Parameters::MEDIUM));
    }
    else
    if (sButtonName == "wStrongRadioButton") {
        m_pParameters->setParameter(Parameters::SCREEN_BREAK_STRENGTH, QString::number(Parameters::STRONG));
    }
}

// Screen break enabled toggled:
void ScreenBreakPage::onScreenBreakEnabledToggled(bool bChecked)
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
    updateUI();
}

// Cursor break:
void ScreenBreakPage::onScreenBreakSliderChanged(int iValue)
{
    m_pParameters->setParameter(Parameters::SCREEN_BREAK_REGULARITY, QString::number(iValue));
}
