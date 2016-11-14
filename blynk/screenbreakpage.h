#ifndef SCREENBREAKPAGE_H
#define SCREENBREAKPAGE_H

// Qt:
#include <QWidget>

namespace Ui {
class ScreenBreakPage;
}
class Parameters;
class QButtonGroup;
class QAbstractButton;

class ScreenBreakPage : public QWidget
{
    Q_OBJECT

public:
    // Constructor:
    explicit ScreenBreakPage(QWidget *parent = 0);

    // Destructor:
    ~ScreenBreakPage();

    // Set parameters:
    void setParameters(Parameters *pParameters);

    // Update UI:
    void updateUI();

private:
    // UI:
    Ui::ScreenBreakPage *ui;

    // Button group:
    QButtonGroup *m_pButtonGroup;

    // Parameters:
    Parameters *m_pParameters;

public slots:
    // Button clicked:
    void onButtonClicked(QAbstractButton *);

    // Screen break enabled toggled:
    void onScreenBreakEnabledToggled(bool bChecked);

    // Cursor break:
    void onScreenBreakSliderChanged(int iValue);
};

#endif // SCREENBREAKPAGE_H
