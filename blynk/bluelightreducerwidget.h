#ifndef BLUELIGHTREDUCERWIDGET_H
#define BLUELIGHTREDUCERWIDGET_H

// Qt:
#include <QWidget>

// Application:
#include "parameters.h"

class QPushButton;

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
    // Button light clicked:
    void onButtonLightClicked();

    // Button medium clicked:
    void onButtonMediumClicked();

    // Button strong clicked:
    void onButtonStrongClicked();

private:
    // Light button:
    QPushButton *m_pLightButton;

    // Medium
    QPushButton *m_pMediumButton;

    // Strong:
    QPushButton *m_pStrongButton;

    // Parameters:
    Parameters *m_pParameters;

signals:
    // Notify:
    void buttonClicked(const Parameters::Strength &eStrength);
};

#endif // BLUELIGHTREDUCERWIDGET_H
