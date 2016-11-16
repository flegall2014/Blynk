// Qt:
#include <QButtonGroup>
#include <QDebug>

// Application:
#include "cursorpage.h"
#include "ui_cursorpage.h"
#include "parameters.h"
#include "utils.h"

// Constructor:
CursorPage::CursorPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CursorPage),
    m_pParameters(NULL),
    m_pButtonGroup(NULL)
{
    ui->setupUi(this);

    m_pButtonGroup = new QButtonGroup(this);
    m_pButtonGroup->addButton(ui->wRadio2);
    m_pButtonGroup->addButton(ui->wRadio4);
    m_pButtonGroup->addButton(ui->wRadio6);
    m_pButtonGroup->addButton(ui->wRadio8);
    m_pButtonGroup->addButton(ui->wRadio10);
    m_pButtonGroup->addButton(ui->wBlynkRandomRadioButton);

    connect(m_pButtonGroup, SIGNAL(buttonClicked(QAbstractButton*)), this, SLOT(onButtonClicked(QAbstractButton*)));
    connect(ui->wBlynkCursorEnabled, &QCheckBox::toggled, this, &CursorPage::onBlynkCursorEnabledChanged);
}

// Destructor:
CursorPage::~CursorPage()
{
    delete ui;
}

// Set parameters:
void CursorPage::setParameters(Parameters *pParameters)
{
    m_pParameters = pParameters;
}

// Update UI:
void CursorPage::updateUI()
{
    // Check enabled state:
    bool bBlynkCursorEnabled = (bool)m_pParameters->parameter(Parameters::BLYNK_CURSOR_ENABLED).toInt();

    // Check random mdode:
    bool bRandomModeOn = (bool)m_pParameters->parameter(Parameters::BLYNK_CURSOR_RANDOM_MODE).toInt();

    // Update radio button:
    int iBlynkRegularity = m_pParameters->parameter(Parameters::BLYNK_CURSOR_REGULARITY).toInt();
    ui->wRadio2->setChecked(iBlynkRegularity == 2);
    ui->wRadio2->setEnabled(bBlynkCursorEnabled);
    ui->wRadio4->setChecked(iBlynkRegularity == 4);
    ui->wRadio4->setEnabled(bBlynkCursorEnabled);
    ui->wRadio6->setChecked(iBlynkRegularity == 6);
    ui->wRadio6->setEnabled(bBlynkCursorEnabled);
    ui->wRadio8->setChecked(iBlynkRegularity == 8);
    ui->wRadio8->setEnabled(bBlynkCursorEnabled);
    ui->wRadio10->setChecked(iBlynkRegularity == 10);
    ui->wRadio10->setEnabled(bBlynkCursorEnabled);

    // Update blynk random checkbox:
    ui->wBlynkRandomRadioButton->setChecked(bRandomModeOn);
    ui->wBlynkRandomRadioButton->setEnabled(bBlynkCursorEnabled);

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

// Button clicked:
void CursorPage::onButtonClicked(QAbstractButton *pButton)
{
    QString sObjectName = pButton->objectName();
    int iRegularity = 0;
    if (sObjectName == "wRadio2") {
        iRegularity = 2;
        m_pParameters->setParameter(Parameters::BLYNK_CURSOR_RANDOM_MODE, "0");
        m_pParameters->setParameter(Parameters::BLYNK_CURSOR_REGULARITY, QString::number(iRegularity));
    }
    else
    if (sObjectName == "wRadio4") {
        iRegularity = 4;
        m_pParameters->setParameter(Parameters::BLYNK_CURSOR_RANDOM_MODE, "0");
        m_pParameters->setParameter(Parameters::BLYNK_CURSOR_REGULARITY, QString::number(iRegularity));
    }
    else
    if (sObjectName == "wRadio6") {
        iRegularity = 6;
        m_pParameters->setParameter(Parameters::BLYNK_CURSOR_RANDOM_MODE, "0");
        m_pParameters->setParameter(Parameters::BLYNK_CURSOR_REGULARITY, QString::number(iRegularity));
    }
    else
    if (sObjectName == "wRadio8") {
        iRegularity = 8;
        m_pParameters->setParameter(Parameters::BLYNK_CURSOR_RANDOM_MODE, "0");
        m_pParameters->setParameter(Parameters::BLYNK_CURSOR_REGULARITY, QString::number(iRegularity));
    }
    else
    if (sObjectName == "wRadio10") {
        iRegularity = 10;
        m_pParameters->setParameter(Parameters::BLYNK_CURSOR_RANDOM_MODE, "0");
        m_pParameters->setParameter(Parameters::BLYNK_CURSOR_REGULARITY, QString::number(iRegularity));
    }
    else
    if (sObjectName == "wBlynkRandomRadioButton") {
        m_pParameters->setParameter(Parameters::BLYNK_CURSOR_RANDOM_MODE, "1");
    }
}

// Blynk cursor enabled changed:
void CursorPage::onBlynkCursorEnabledChanged(bool bChecked)
{
    m_pParameters->setParameter(Parameters::BLYNK_CURSOR_ENABLED, QString::number((int)!bChecked));
    updateUI();
}
