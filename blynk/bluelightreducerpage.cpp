// Application:
#include "bluelightreducerpage.h"
#include "ui_bluelightreducerpage.h"
#include <QDebug>

// Constructor:
BlueLightReducerPage::BlueLightReducerPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BlueLightReducerPage),
    m_pParameters(NULL)
{
    ui->setupUi(this);

    // Blue light reducer:
    connect(ui->wBlueLightReducerWidget, &BlueLightReducerWidget::buttonClicked, this, &BlueLightReducerPage::onBlueLightReducerValueChanged);
    connect(ui->wBlueLightReducerEnabled, &QCheckBox::toggled, this, &BlueLightReducerPage::onBlueLightReducerEnabledToggled);
    connect(ui->wStartTimeEdit, &QTimeEdit::timeChanged, this, &BlueLightReducerPage::onStartTimeEditChanged);
}

// Destructor:
BlueLightReducerPage::~BlueLightReducerPage()
{
    delete ui;
}

// Set parameters:
void BlueLightReducerPage::setParameters(Parameters *pParameters)
{
    m_pParameters = pParameters;
}

// Update UI:
void BlueLightReducerPage::updateUI()
{
    // Blue light reducer enabled:
    bool bBlueLightReducerEnabled = (bool)m_pParameters->parameter(Parameters::BLUE_LIGHT_REDUCER_ENABLED).toInt();

    ui->wBlueLightReducerWidget->setEnabled(bBlueLightReducerEnabled);
    ui->wBlueLightReducerWidget->setParameters(m_pParameters);
    ui->wBlueLightReducerEnabled->setChecked(!bBlueLightReducerEnabled);

    // Blue light reducer start time:
    ui->wStartTimeEdit->setEnabled(bBlueLightReducerEnabled);
    ui->wStartTimeEdit->setTime(QTime::fromString(m_pParameters->parameter(Parameters::BLUE_LIGHT_REDUCER_START_TIME)));
}

// Blue light reducer:
void BlueLightReducerPage::onBlueLightReducerValueChanged(const Parameters::Strength &eStrength)
{
    qDebug() << "SET STRENGTH TO: " << eStrength;
    m_pParameters->setParameter(Parameters::BLUE_LIGHT_REDUCER_STRENGTH, QString::number(eStrength));
}

// Enabled state changed:
void BlueLightReducerPage::onBlueLightReducerEnabledToggled(bool bChecked)
{
    m_pParameters->setParameter(Parameters::BLUE_LIGHT_REDUCER_ENABLED, QString::number((int)!bChecked));
    updateUI();
}

// Start time changed:
void BlueLightReducerPage::onStartTimeEditChanged(const QTime &tTime)
{
    m_pParameters->setParameter(Parameters::BLUE_LIGHT_REDUCER_START_TIME, tTime.toString());
}
