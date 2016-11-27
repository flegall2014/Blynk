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
    ui->debugView->setModel(m_pParameterDebug);
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

// Parameter changed:
void DebugDialog::onParameterChanged(const Parameters::Parameter &parameter)
{
    m_pParameterDebug->resetModel();
}
