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

    // Initialize:
    void initialize(double dBrightness, double dRedGamma, double dGreenGamma, double dBlueGamma);

private:
    // UI:
    Ui::DebugDialog *ui;

    // Model:
    ParameterDebug *m_pParameterDebug;

public slots:
    // Parameter changed:
    void onParameterChanged(const Parameters::Parameter &parameter);

    // Brightness changed:
    void onBrightnessChanged(int iBrightness);

    // Gamma changed:
    void onRedGammaChanged(int iValue);
    void onGreenGammaChanged(int iValue);
    void onBlueGammaChanged(int iValue);

signals:
    void brightnessChanged(double dBrightness);
    void redGammaChanged(double dRedGamma);
    void greenGammaChanged(double dGreenGamma);
    void blueGammaChanged(double dBlueGamma);
};

#endif // DEBUGDIALOG_H
