#ifndef BLUELIGHTREDUCERPAGE_H
#define BLUELIGHTREDUCERPAGE_H

// Qt:
#include <QWidget>

// Application:
#include "parameters.h"

namespace Ui {
class BlueLightReducerPage;
}

class BlueLightReducerPage : public QWidget
{
    Q_OBJECT

public:
    // Constructor:
    explicit BlueLightReducerPage(QWidget *parent = 0);

    // Destructor:
    ~BlueLightReducerPage();

    // Set parameters:
    void setParameters(Parameters *pParameters);

    // Update UI:
    void updateUI();

private:
    // UI:
    Ui::BlueLightReducerPage *ui;

    // Parameters:
    Parameters *m_pParameters;

public slots:
    // Blue light reducer value changed:
    void onBlueLightReducerValueChanged(const Parameters::Strength &eStrength);

    // Enabled state changed:
    void onBlueLightReducerEnabledToggled(bool bChecked);

    // Start time changed:
    void onStartTimeEditChanged(const QTime &tTime);
};

#endif // BLUELIGHTREDUCERPAGE_H
