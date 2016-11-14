#ifndef CURSORPAGE_H
#define CURSORPAGE_H

#include <QWidget>

namespace Ui {
class CursorPage;
}
class QAbstractButton;
class Parameters;
class QButtonGroup;

class CursorPage : public QWidget
{
    Q_OBJECT

public:
    // Constructor:
    explicit CursorPage(QWidget *parent = 0);

    // Destructor:
    ~CursorPage();

    // Set parameters:
    void setParameters(Parameters *pParameters);

    // Update UI:
    void updateUI();

private:
    // UI:
    Ui::CursorPage *ui;

    // Parameters:
    Parameters *m_pParameters;

    // Button group:
    QButtonGroup *m_pButtonGroup;

public slots:
    // Button clicked:
    void onButtonClicked(QAbstractButton *);

    // Blynk cursor enabled changed:
    void onBlynkCursorEnabledChanged(bool bChecked);
};

#endif // CURSORPAGE_H
