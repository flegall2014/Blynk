#ifndef BLUELIGHTREDUCERWIDGET_H
#define BLUELIGHTREDUCERWIDGET_H

// Qt:
#include <QWidget>

// Application:
#include "parameters.h"

class QPushButton;
class CustomSlider;

class BlueLightReducerWidget : public QWidget
{
    Q_OBJECT

public:
    // Constructor:
    explicit BlueLightReducerWidget(QWidget *parent = 0);

    // Update:
    void update(const Parameters::Strength &eStrength);

    // Set parameters:
    void setParameters(Parameters *pParameters);

    // Set button color:
    void setButtonColor(QPushButton *pButton, const QColor &color);

public slots:
    // Strength changed:
    void onStrengthChanged(int iValue);

private:
    // Strength slider:
    CustomSlider *m_pStrengthSlider;

    // Parameters:
    Parameters *m_pParameters;

signals:
    // Notify:
    void buttonClicked(const Parameters::Strength &eStrength);
};

#endif // BLUELIGHTREDUCERWIDGET_H
