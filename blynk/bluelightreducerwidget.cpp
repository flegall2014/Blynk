// Qt:
#include <QHBoxLayout>
#include <QButtonGroup>
#include <QPushButton>
#include <QDebug>

// Application:
#include "bluelightreducerwidget.h"

// Constructor:
BlueLightReducerWidget::BlueLightReducerWidget(QWidget *parent) : QWidget(parent),
    m_pParameters(NULL)
{
    // Horizontal layout:
    QHBoxLayout *pLayout = new QHBoxLayout(this);
    setLayout(pLayout);

    QButtonGroup *pGroup = new QButtonGroup(this);
    pGroup->setExclusive(true);

    m_pLightButton = new QPushButton(tr("LIGHT"), this);
    m_pLightButton->setCheckable(true);
    pLayout->addWidget(m_pLightButton);
    connect(m_pLightButton, &QPushButton::clicked, this, &BlueLightReducerWidget::onButtonLightClicked);

    // MEDIUM:
    m_pMediumButton = new QPushButton(tr("MEDIUM"), this);
    m_pMediumButton->setCheckable(true);
    pLayout->addWidget(m_pMediumButton);
    connect(m_pMediumButton, &QPushButton::clicked, this, &BlueLightReducerWidget::onButtonMediumClicked);

    // STRONG:
    m_pStrongButton = new QPushButton(tr("STRONG"), this);
    m_pStrongButton->setCheckable(true);
    pLayout->addWidget(m_pStrongButton);
    connect(m_pStrongButton, &QPushButton::clicked, this, &BlueLightReducerWidget::onButtonStrongClicked);

    pGroup->addButton(m_pLightButton);
    pGroup->addButton(m_pMediumButton);
    pGroup->addButton(m_pStrongButton);
}

// Button light clicked:
void BlueLightReducerWidget::onButtonLightClicked()
{
    emit buttonClicked(Parameters::LIGHT);
}

// Button medium clicked:
void BlueLightReducerWidget::onButtonMediumClicked()
{
    emit buttonClicked(Parameters::MEDIUM);
}

// Button strong clicked:
void BlueLightReducerWidget::onButtonStrongClicked()
{
    emit buttonClicked(Parameters::STRONG);
}

// Update:
void BlueLightReducerWidget::update(int iStrength)
{
    Parameters::Strength eStrength = (Parameters::Strength)iStrength;
    QColor lightButtonColor = QColor(m_pParameters->parameter(Parameters::RED_CHANNEL_MAX_LIGHT).toInt()/255,
        m_pParameters->parameter(Parameters::GREEN_CHANNEL_MAX_LIGHT).toInt()/255,
        m_pParameters->parameter(Parameters::BLUE_CHANNEL_MAX_LIGHT).toInt()/255);
    QColor mediumButtonColor = QColor(m_pParameters->parameter(Parameters::RED_CHANNEL_MAX_MEDIUM).toInt()/255,
        m_pParameters->parameter(Parameters::GREEN_CHANNEL_MAX_MEDIUM).toInt()/255,
        m_pParameters->parameter(Parameters::BLUE_CHANNEL_MAX_MEDIUM).toInt()/255);
    QColor strongButtonColor = QColor(m_pParameters->parameter(Parameters::RED_CHANNEL_MAX_STRONG).toInt()/255,
        m_pParameters->parameter(Parameters::GREEN_CHANNEL_MAX_STRONG).toInt()/255,
        m_pParameters->parameter(Parameters::BLUE_CHANNEL_MAX_STRONG).toInt()/255);

    setButtonColor(m_pLightButton, lightButtonColor);
    setButtonColor(m_pMediumButton, mediumButtonColor);
    setButtonColor(m_pStrongButton, strongButtonColor);

    m_pLightButton->setChecked(eStrength == Parameters::LIGHT);
    m_pMediumButton->setChecked(eStrength == Parameters::MEDIUM);
    m_pStrongButton->setChecked(eStrength == Parameters::STRONG);
}

// Set parameters:
void BlueLightReducerWidget::setParameters(Parameters *pParameters)
{
    m_pParameters = pParameters;
    update(m_pParameters->parameter(Parameters::BLUE_LIGHT_REDUCER_STRENGTH).toInt());
}

// Set button color:
void BlueLightReducerWidget::setButtonColor(QPushButton *pButton, const QColor &color)
{
    if (pButton)
    {
        QString sRed = QString::number(color.red(), 16).toUpper();
        QString sGreen = QString::number(color.green(), 16).toUpper();
        QString sBlue = QString::number(color.blue(), 16).toUpper();
        qDebug() << sRed << sGreen << sBlue;
        pButton->setStyleSheet(QString("background-color: #%1%2%3").arg(sRed).arg(sGreen).arg(sBlue));
    }
}
