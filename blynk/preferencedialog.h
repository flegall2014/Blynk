#ifndef PREFERENCEDIALOG_H
#define PREFERENCEDIALOG_H

// Qt:
#include <QDialog>

// Application:
#include "parameters.h"

namespace Ui {
class PreferenceDialog;
}

class PreferenceDialog : public QDialog
{
    Q_OBJECT

public:
    // Constructor:
    explicit PreferenceDialog(QWidget *parent = 0);

    // Destructor:
    ~PreferenceDialog();

    // Set parameters:
    void setParameters(Parameters *pParameters);

protected:
    // Close event:
    void closeEvent(QCloseEvent *);

private:
    // Update blynk cursor:
    void updateBlynkCursorArea();

    // Update screen break:
    void updateScreenBreakArea();

    // Update blue light reducer:
    void updateBlueLightReducerArea();

    // Update start blynk at login:
    void updateStartBlynkAtLoginArea();

private:
    // UI:
    Ui::PreferenceDialog *ui;

    // Parameters:
    Parameters *m_pParameters;

public slots:
    // Blynk cursor:
    void onBlynkCursorSliderChanged(int iValue);
    void onBlynkCursorEnabledToggled(bool bChecked);
    void onBlynkPerMinuteChanged(int iIndex);
    void onSetBlynkCursorRandomMode(bool bChecked);

    // Cursor break:
    void onScreenBreakSliderChanged(int iValue);
    void onScreenBreakEnabledToggled(bool bChecked);
    void onScreenBreakStrengthComboChanged(int index);

    // Blue light reducer:
    void onBlueLightReducerValueChanged(const Parameters::Strength &eStrength);
    void onBlueLightReducerEnabledToggled(bool bChecked);
    void onStartTimeEditChanged(const QTime &tTime);

    // Start Blynk at login:
    void onStartBlynkAtLoginChanged(bool bChecked);

    // Done:
    void onDone();

    // Update UI:
    void updateUI();
};

#endif // PREFERENCEDIALOG_H
