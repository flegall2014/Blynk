#include "customtooltip.h"
#include "ui_customtooltip.h"
#include <QDebug>

// Constructor:
CustomTooltip::CustomTooltip(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CustomTooltip)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::SubWindow);
}

// Destructor:
CustomTooltip::~CustomTooltip()
{
    delete ui;
}

// Set tooltip text:
void CustomTooltip::setText(const QString &sTooltipText)
{
    ui->wTooltipLabel->setText(sTooltipText);
}
