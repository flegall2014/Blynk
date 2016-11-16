// Application:
#include "customslider.h"
#include "ui_customslider.h"

// Constructor:
CustomSlider::CustomSlider(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CustomSlider)
{
    ui->setupUi(this);
    connect(ui->wSlider, &QSlider::valueChanged, this, &CustomSlider::onSliderValueChanged);
}

// Destructor:
CustomSlider::~CustomSlider()
{
    delete ui;
}

// Set range:
void CustomSlider::setRange(int iMin, int iMax)
{
    ui->wSlider->blockSignals(true);
    ui->wSlider->setMinimum(iMin);
    ui->wSlider->setMaximum(iMax);
    ui->wSlider->blockSignals(false);
}

// Set value:
void CustomSlider::setValue(int iValue)
{
    ui->wSlider->setValue(iValue);
    ui->wSliderValue->setText(QString::number(iValue));
}

// Slider value changed:
void CustomSlider::onSliderValueChanged(int iValue)
{
    ui->wSliderValue->setText(QString::number(iValue));
    emit valueChanged(iValue);
}

// Set title:
void CustomSlider::setTitle(const QString &sTitle)
{
    ui->wSliderTitle->setText(sTitle);
}

// Set orientation:
void CustomSlider::setOrientation(const Qt::Orientation &orientation)
{
    ui->wSlider->setOrientation(orientation);
}

int CustomSlider::value() const
{
    return ui->wSlider->value();
}

