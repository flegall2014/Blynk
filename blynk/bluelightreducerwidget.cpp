// Qt:
#include <QHBoxLayout>
#include <QButtonGroup>
#include <QPushButton>
#include <QDebug>

// Application:
#include "blynk.h"
#include "controller.h"
#include "bluelightreducerwidget.h"
#include "customslider.h"

// Constructor:
BlueLightReducerWidget::BlueLightReducerWidget(QWidget *parent) : QWidget(parent),
    m_pStrengthSlider(NULL),
    m_pParameters(NULL)
{
    setContentsMargins(0, 0, 0, 0);

    // Horizontal layout:
    QHBoxLayout *pLayout = new QHBoxLayout(this);
    pLayout->setContentsMargins(0, 0, 0, 0);
    setLayout(pLayout);

    m_pStrengthSlider = new CustomSlider(this);
    m_pStrengthSlider->setRange(0, 2);
    pLayout->addWidget(m_pStrengthSlider);
    connect(m_pStrengthSlider, &CustomSlider::valueChanged, this, &BlueLightReducerWidget::onStrengthChanged);
}

// Strength changed:
void BlueLightReducerWidget::onStrengthChanged(int iValue)
{
    Parameters::Strength eStrength = (Parameters::Strength)(iValue+1);
    emit buttonClicked(eStrength);
}

// Update:
void BlueLightReducerWidget::update(const Parameters::Strength &eStrength)
{
    /*
    Controller *pController = Blynk::instance()->controller();

    QColor lightButtonColor = pController->colorForStrength(Parameters::LIGHT);
    QColor mediumButtonColor = pController->colorForStrength(Parameters::MEDIUM);
    QColor strongButtonColor = pController->colorForStrength(Parameters::STRONG);

    setButtonColor(m_pLightButton, lightButtonColor);
    setButtonColor(m_pMediumButton, mediumButtonColor);
    setButtonColor(m_pStrongButton, strongButtonColor);
    */

    m_pStrengthSlider->setValue((int)eStrength - 1);
}

// Set parameters:
void BlueLightReducerWidget::setParameters(Parameters *pParameters)
{
    m_pParameters = pParameters;
    update((Parameters::Strength)m_pParameters->parameter(Parameters::BLUE_LIGHT_REDUCER_STRENGTH).toInt());
}

// Set button color:
void BlueLightReducerWidget::setButtonColor(QPushButton *pButton, const QColor &color)
{
    if (pButton)
    {
        QString sRed = QString::number(color.red(), 16).toUpper();
        if (sRed.length() < 2)
            sRed = "0"+sRed;
        QString sGreen = QString::number(color.green(), 16).toUpper();
        if (sGreen.length() < 2)
            sGreen = "0"+sGreen;
        QString sBlue = QString::number(color.blue(), 16).toUpper();
        if (sBlue.length() < 2)
            sBlue = "0"+sBlue;
        pButton->setStyleSheet(QString("background-color: #%1%2%3").arg(sRed).arg(sGreen).arg(sBlue));
    }
}

