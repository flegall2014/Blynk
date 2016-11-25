#ifndef BLYNKWINDOW_H
#define BLYNKWINDOW_H

// Qt:
#include <QWidget>

// Application:
#include "customwindow.h"

namespace Ui {
class BlynkWindow;
}
class Parameters;

class BlynkWindow : public CustomWindow
{
    Q_OBJECT

public:
    // Constructor:
    explicit BlynkWindow(const QString &sTitle, QWidget *parent = 0);

    // Destructor:
    ~BlynkWindow();

    // Set parameters:
    void setParameters(Parameters *pParameters);

    // Update UI:
    void updateUI();

protected:
    // Paint event:
    virtual void paintEvent(QPaintEvent *event);

private:
    // Update blynk cursor area:
    void updateBlynkCursorArea();

    // Update screen break area:
    void updateScreenBreakArea();

    // Update blue light reducer area:
    void updateBlueLightReducerArea();

    // Update start blynk at login area:
    void updateStartBlynkAtLoginArea();

private:
    // UI:
    Ui::BlynkWindow *ui;

    // Parameters:
    Parameters *m_pParameters;

public slots:
    // Blynk regularity slider changed:
    void onBlynkRegularitySliderChanged(int iRegularity);

    // Random checkbox toggled:
    void onRandomCheckBoxToggled(bool bChecked);

    // Blue light reducer enabled changed:
    void onBlynkCursorEnabledChanged(bool bChecked);

    // Blynk per minute value changed:
    void onBlynkPerMinuteValueChanged(int iBlynkPerMinute);

    // Screen break regularity changed:
    void onScreenBreakRegularityChanged(int iScreenBreakRegularity);

    // Screen break enabled changed:
    void onScreenBreakEnabledChanged(bool bChecked);

    // Screen break strength changed:
    void onScreenBreakStrengthChanged(int iIndex);

    // Blue light reducer value changed:
    void onBlueLightReducerValueChanged(int iBlueLightReducerValue);

    // Blue light reducer enabled changed:
    void onBlueLightReducerEnabledChanged(bool bChecked);

    // Start time changed:
    void onStartTimeEditChanged(const QTime &tTime);

    // Start blynk at login changed:
    void onStartBlynkAtLoginChanged(bool bChecked);

    // Done:
    void onDone();
};

#endif // BLYNKWINDOW_H
