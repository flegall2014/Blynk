// Qt:
#include <QButtonGroup>
#include <QCloseEvent>
#include <QDebug>

// Application:
#include "colordialog.h"
#include "ui_colordialog.h"
#include <gammaramp.h>
#include "parameters.h"

// Constructor:
ColorDialog::ColorDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ColorDialog),
    m_eStrength(Parameters::LIGHT),
    m_pParameters(NULL)
{
    // Setup UI:
    ui->setupUi(this);

    // Listen to slider changes:
    connect(ui->minBlueSlider, &QSlider::valueChanged, this, &ColorDialog::onMinBlueValueChanged);
    connect(ui->maxBlueSlider, &QSlider::valueChanged, this, &ColorDialog::onMaxBlueValueChanged);

    QButtonGroup *pButtonGroup = new QButtonGroup(this);
    pButtonGroup->setExclusive(true);
    pButtonGroup->addButton(ui->wSetLight);
    pButtonGroup->addButton(ui->wSetMedium);
    pButtonGroup->addButton(ui->wSetStrong);

    // List to button clicks:
    connect(ui->wSetLight, &QPushButton::clicked, this, &ColorDialog::onSetBlueLightMode);
    connect(ui->wSetMedium, &QPushButton::clicked, this, &ColorDialog::onSetBlueMediumMode);
    connect(ui->wSetStrong, &QPushButton::clicked, this, &ColorDialog::onSetBlueStrongMode);
}

// Destructor:
ColorDialog::~ColorDialog()
{
    delete ui;
}

// Min blue changed:
void ColorDialog::onMinBlueValueChanged(int iValue)
{
    ui->minBlueLabel->setText(QString::number(iValue));

    if (m_eStrength == Parameters::LIGHT)
        m_pParameters->setParameter(Parameters::BLUE_CHANNEL_MIN_LIGHT, QString::number(iValue));
    else
    if (m_eStrength == Parameters::MEDIUM)
        m_pParameters->setParameter(Parameters::BLUE_CHANNEL_MIN_MEDIUM, QString::number(iValue));
    else
    if (m_eStrength == Parameters::STRONG)
        m_pParameters->setParameter(Parameters::BLUE_CHANNEL_MIN_STRONG, QString::number(iValue));

    // Update desktop colors:
    updateDesktop(m_eStrength);
}

// Max blue changed:
void ColorDialog::onMaxBlueValueChanged(int iValue)
{
    ui->maxBlueLabel->setText(QString::number(iValue));

    if (m_eStrength == Parameters::LIGHT)
        m_pParameters->setParameter(Parameters::BLUE_CHANNEL_MAX_LIGHT, QString::number(iValue));
    else
    if (m_eStrength == Parameters::MEDIUM)
        m_pParameters->setParameter(Parameters::BLUE_CHANNEL_MAX_MEDIUM, QString::number(iValue));
    else
    if (m_eStrength == Parameters::STRONG)
        m_pParameters->setParameter(Parameters::BLUE_CHANNEL_MAX_STRONG, QString::number(iValue));

    updateDesktop(m_eStrength);
}

// Set blue light:
void ColorDialog::onSetBlueLightMode()
{
    m_eStrength = Parameters::LIGHT;
    ui->minBlueSlider->setValue(m_pParameters->parameter(Parameters::BLUE_CHANNEL_MIN_LIGHT).toInt());
    ui->maxBlueSlider->setValue(m_pParameters->parameter(Parameters::BLUE_CHANNEL_MAX_LIGHT).toInt());
}

// Set blue medium:
void ColorDialog::onSetBlueMediumMode()
{
    m_eStrength = Parameters::MEDIUM;
    ui->minBlueSlider->setValue(m_pParameters->parameter(Parameters::BLUE_CHANNEL_MIN_MEDIUM).toInt());
    ui->maxBlueSlider->setValue(m_pParameters->parameter(Parameters::BLUE_CHANNEL_MAX_MEDIUM).toInt());
}

// Set blue strong:
void ColorDialog::onSetBlueStrongMode()
{
    m_eStrength = Parameters::STRONG;
    ui->minBlueSlider->setValue(m_pParameters->parameter(Parameters::BLUE_CHANNEL_MIN_STRONG).toInt());
    ui->maxBlueSlider->setValue(m_pParameters->parameter(Parameters::BLUE_CHANNEL_MAX_STRONG).toInt());
}

// Update desktop:
void ColorDialog::updateDesktop(const Parameters::Strength &eStrength)
{
    unsigned short iMinBlue = 0;
    unsigned short iMaxBlue = 0;
    if (eStrength == Parameters::LIGHT)
    {
        iMinBlue = m_pParameters->parameter(Parameters::BLUE_CHANNEL_MIN_LIGHT).toUShort();
        iMaxBlue = m_pParameters->parameter(Parameters::BLUE_CHANNEL_MAX_LIGHT).toUShort();
    }
    else
    if (eStrength == Parameters::MEDIUM)
    {
        iMinBlue = m_pParameters->parameter(Parameters::BLUE_CHANNEL_MIN_MEDIUM).toUShort();
        iMaxBlue = m_pParameters->parameter(Parameters::BLUE_CHANNEL_MAX_MEDIUM).toUShort();
    }
    else
    if (m_eStrength == Parameters::STRONG)
    {
        iMinBlue = m_pParameters->parameter(Parameters::BLUE_CHANNEL_MIN_STRONG).toUShort();
        iMaxBlue = m_pParameters->parameter(Parameters::BLUE_CHANNEL_MAX_STRONG).toUShort();
    }

    int iDeltaBlue = abs(iMaxBlue-iMinBlue);
    double iRedStep = (double)65535/(double)256;
    double iGreenStep = (double)65535/(double)256;
    double iBlueStep = (double)iDeltaBlue/(double)256;

    std::vector<unsigned short> vRed, vGreen, vBlue;
    for (int i=0; i<256; i++)
    {
        vRed.push_back((unsigned short)qRound(i*iRedStep));
        vGreen.push_back((unsigned short)qRound(i*iGreenStep));
        vBlue.push_back((unsigned short)qRound(qMin(iMinBlue, iMaxBlue) + i*iBlueStep));
    }

    // Apply color palette:
    GammaRamp gammaRamp;
    gammaRamp.setColorPalette(NULL, vRed, vGreen, vBlue);
}

// Update UI:
void ColorDialog::updateUIForStrength(const Parameters::Strength &eStrength)
{
    m_eStrength = eStrength;
    if (eStrength == Parameters::LIGHT) {
        unsigned short iMinBlueLight = m_pParameters->parameter(Parameters::BLUE_CHANNEL_MIN_LIGHT).toUShort();
        unsigned short iMaxBlueLight = m_pParameters->parameter(Parameters::BLUE_CHANNEL_MAX_LIGHT).toUShort();
        ui->minBlueSlider->setValue(iMinBlueLight);
        ui->maxBlueSlider->setValue(iMaxBlueLight);
        ui->wSetLight->setChecked(true);
        ui->wSetMedium->setChecked(false);
        ui->wSetStrong->setChecked(false);
    }
    else
    if (eStrength == Parameters::MEDIUM) {
        unsigned short iMinBlueMedium = m_pParameters->parameter(Parameters::BLUE_CHANNEL_MIN_MEDIUM).toUShort();
        unsigned short iMaxBlueMedium = m_pParameters->parameter(Parameters::BLUE_CHANNEL_MAX_MEDIUM).toUShort();
        ui->minBlueSlider->setValue(iMinBlueMedium);
        ui->maxBlueSlider->setValue(iMaxBlueMedium);
        ui->wSetLight->setChecked(false);
        ui->wSetMedium->setChecked(true);
        ui->wSetStrong->setChecked(false);
    }
    else
    if (eStrength == Parameters::STRONG) {
        unsigned short iMinBlueStrong = m_pParameters->parameter(Parameters::BLUE_CHANNEL_MIN_STRONG).toUShort();
        unsigned short iMaxBlueStrong = m_pParameters->parameter(Parameters::BLUE_CHANNEL_MAX_STRONG).toUShort();
        ui->minBlueSlider->setValue(iMinBlueStrong);
        ui->maxBlueSlider->setValue(iMaxBlueStrong);
        ui->wSetLight->setChecked(false);
        ui->wSetMedium->setChecked(false);
        ui->wSetStrong->setChecked(true);
    }
}

// Update UI:
void ColorDialog::updateUI()
{
    updateUIForStrength((Parameters::Strength)m_pParameters->parameter(Parameters::BLUE_LIGHT_REDUCER_STRENGTH).toInt());
}

// Handle close event:
void ColorDialog::closeEvent(QCloseEvent *e)
{
    hide();
    e->ignore();
}

// Set parameters:
void ColorDialog::setParameters(Parameters *pParameters)
{
    m_pParameters = pParameters;
}
