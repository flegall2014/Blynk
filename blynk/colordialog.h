#ifndef COLORDIALOG_H
#define COLORDIALOG_H

// Qt:
#include <QDialog>

// Application:
#include "parameters.h"

namespace Ui {
class ColorDialog;
}
class Parameters;

class ColorDialog : public QDialog
{
    Q_OBJECT

public:
    // Constructor:
    explicit ColorDialog(QWidget *parent = 0);

    // Destructor:
    ~ColorDialog();

    // Set parameters:
    void setParameters(Parameters *pParameters);

protected:
    // Close event:
    void closeEvent(QCloseEvent *);

private:
    // Update desktop:
    void updateDesktop(const Parameters::Strength &eStrength);

    // Update UI:
    void updateUIForStrength(const Parameters::Strength &eStrength);

private:
    // UI:
    Ui::ColorDialog *ui;

    // Strength:
    Parameters::Strength m_eStrength;

    // Parameters:
    Parameters *m_pParameters;

public slots:
    void onMinBlueValueChanged(int iValue);
    void onMaxBlueValueChanged(int iValue);
    void onSetBlueLightMode();
    void onSetBlueMediumMode();
    void onSetBlueStrongMode();
    void updateUI();
};

#endif // COLORDIALOG_H
