#include "debugdialog.h"
#include "ui_debugdialog.h"
#include "parameterdebug.h"

// Constructor:
DebugDialog::DebugDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DebugDialog),
    m_pParameterDebug(new ParameterDebug(this))
{
    ui->setupUi(this);
    setWindowFlags(Qt::WindowTitleHint | Qt::WindowStaysOnTopHint);
    ui->debugView->setModel(m_pParameterDebug);

    connect(ui->wBrightnessSlider, &QSlider::valueChanged, this, &DebugDialog::onBrightnessChanged);
    connect(ui->wRedGammaSlider, &QSlider::valueChanged, this, &DebugDialog::onRedGammaChanged);
    connect(ui->wGreenGammaSlider, &QSlider::valueChanged, this, &DebugDialog::onGreenGammaChanged);
    connect(ui->wBlueGammaSlider, &QSlider::valueChanged, this, &DebugDialog::onBlueGammaChanged);
}

// Constructor:
DebugDialog::~DebugDialog()
{
    delete ui;
}

// Set parameters:
void DebugDialog::setParameters(Parameters *pParameters)
{
    m_pParameterDebug->setParameters(pParameters);
}

// Initialize:
void DebugDialog::initialize(double dBrightness, double dRedGamma, double dGreenGamma, double dBlueGamma)
{
    ui->wBrightnessSlider->setValue(qRound(dBrightness*10));
    ui->wRedGammaSlider->setValue(qRound(dRedGamma*10));
    ui->wGreenGammaSlider->setValue(qRound(dGreenGamma*10));
    ui->wBlueGammaSlider->setValue(qRound(dBlueGamma*10));
}

// Parameter changed:
void DebugDialog::onParameterChanged(const Parameters::Parameter &parameter)
{
    Q_UNUSED(parameter);
    m_pParameterDebug->resetModel();
}

// Brightness changed:
void DebugDialog::onBrightnessChanged(int iBrightness)
{
    double dBrightness = (double)iBrightness/10.;
    ui->wBrightnessLabel->setText(QString::number(iBrightness));
    emit brightnessChanged(dBrightness);

}

// Gamma changed:
void DebugDialog::onRedGammaChanged(int iValue)
{
    double dRedGamma = (double)iValue/10.;
    ui->wRedGammaLabel->setText(QString::number(iValue));
    emit redGammaChanged(dRedGamma);
}

void DebugDialog::onGreenGammaChanged(int iValue)
{
    double dGreenGamma = (double)iValue/10.;
    ui->wGreenGammaLabel->setText(QString::number(iValue));
    emit greenGammaChanged(dGreenGamma);
}

void DebugDialog::onBlueGammaChanged(int iValue)
{
    double dBlueGamma = (double)iValue/10.;
    ui->wBlueGammaLabel->setText(QString::number(iValue));
    emit blueGammaChanged(dBlueGamma);
}
