#ifndef DEBUGDIALOG_H
#define DEBUGDIALOG_H
#include <QDialog>
#include "parameters.h"

namespace Ui {
class DebugDialog;
}
class ParameterDebug;
class Parameters;

class DebugDialog : public QDialog
{
    Q_OBJECT

public:
    // Constructor:
    explicit DebugDialog(QWidget *parent = 0);

    // Destructor:
    ~DebugDialog();

    // Set parameters:
    void setParameters(Parameters *pParameters);

private:
    // UI:
    Ui::DebugDialog *ui;

    // Model:
    ParameterDebug *m_pParameterDebug;

public slots:
    // Parameter changed:
    void onParameterChanged(const Parameters::Parameter &parameter);
};

#endif // DEBUGDIALOG_H
