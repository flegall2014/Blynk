#ifndef SETTINGSWINDOW_H
#define SETTINGSWINDOW_H

// Qt:
#include <QDialog>

// Application:
#include "parameters.h"
#include "customwindow.h"

namespace Ui {
class SettingsWindow;
}
class CursorPage;
class ScreenBreakPage;
class BlueLightReducerPage;
class QPushButton;

class SettingsWindow : public CustomWindow
{
    Q_OBJECT

public:
    // Constructor:
    SettingsWindow(const QString &sTitle, QWidget *parent = 0);

    // Title:
    ~SettingsWindow();

    // Set parameters:
    void setParameters(Parameters *pParameters);

    // Update blynk cursor area:
    void updateBlynkCursorArea();

    // Update screen break area:
    void updateScreenBreakArea();

    // Update blue light reducer area:
    void updateBlueLightReducerArea();

    // Update UI:
    void updateUI();

private:
    Ui::SettingsWindow *ui;
    CursorPage *m_pCursorPage;
    ScreenBreakPage *m_pScreenBreakPage;
    BlueLightReducerPage *m_pBlueLightReducerPage;
    Parameters *m_pParameters;

public slots:
    // Change tab:
    void onCursorButtonClicked();
    void onScreenBreakButtonClicked();
    void onBlueLightReducerButtonClicked();

    // Menu button:
    void onMenuButtonClicked();

signals:
    // Show application menu at cursor pos:
    void showApplicationMenuAtCursorPos();
};

#endif
