#ifndef CONTROLLER_H
#define CONTROLLER_H

// Qt:
#include <QObject>
#include <QTimer>
#include <QElapsedTimer>

// Application:
#include "iservice.h"
#include <cxmlnode.h>
#include "parameters.h"

class QAction;
class QSystemTrayIcon;
class QMenu;
class DimmerWidget;
class SettingsWindow;
class AboutWindow;

class Controller : public QObject, public IService
{
    Q_OBJECT

public:
    enum ScreenBreakState {ENABLED=0, DISABLED, DISABLED_FOR_ONE_HOUR, DISABLED_FOR_THREE_HOURS, DISABLED_UNTIL_TOMORROW};

    friend class Blynk;

    // Destructor:
    virtual ~Controller();

    // Startup:
    virtual bool startup();

    // Shutdown:
    virtual void shutdown();

    // Return color for temperature:
    QColor colorForTemperature(int iTemperature) const;

    // Color for strength:
    QColor colorForStrength(const Parameters::Strength &eStrength);

    // Return temperature for strength:
    int temperatureForStrength(const Parameters::Strength &eStrength);

    // Set temperature:
    void setTemperature(int iTemperature);

protected:
    // Constructor:
    explicit Controller(QObject *parent = 0);

private:
    // Register types:
    void registerTypes();

    // Create menu:
    void createMenu(const CXMLNode &menuNode, QMenu *pRootMenu);

    // Start GUI:
    void startGUI();

    // Create tray icon:
    void initializeTrayIcon();

    // Create tooltip:
    void createTooltip();

    // Save parameters:
    void saveParameters();

    // Load parameters:
    void loadParameters();

    // Release all:
    void releaseAll();

    // Enter blynk cursor random mode:
    void enterBlynkCursorRandomMode();

    // Enter blynk cursor regular mode:
    void enterBlynCursorRegularMode();

    // Time out:
    void onApplicationTimerTimeOut();

private:
    // Tray icon:
    QSystemTrayIcon *m_pTrayIcon;

    // Tray icon menu:
    QMenu *m_pTrayIconMenu;

    // Parameters:
    Parameters *m_pParameters;

    // Dimmer widget:
    DimmerWidget *m_pDimmerWidget;

    // Custom window:
    SettingsWindow *m_pSettingsWindow;

    // About window:
    AboutWindow *m_pAboutWindow;

    // Application timer:
    QTimer m_tApplicationTimer;

    // Blynk cursor elapsed time:
    int m_iBlynkCursorElapsedTime;

    // Screen break elapsed time:
    int m_iScreenBreakElapsedTime;

    // Blue light reducer elapsed time:
    int m_iBlueLightReducerElapsedTime;

    // Blynk cursor random sequence:
    QList<int> m_lBlynkCursorSequence;

    // Blynk cursor random mode on?
    bool m_bBlynkCursorRandomModeOn;

    // Actions:
    QMap<QString, QAction *> m_mActions;

    // Screen break state:
    int m_iScreenBreakDelay;

    // Current temperature:
    int m_iCurrentTemperature;

public slots:
    // Action triggered:
    void onActionTriggered();

    // Context menu about to show:
    void onContextMenuAboutToShow();

    // Context menu about to hide:
    void onContextMenuAboutToHide();

    // Show application menu at cursor pos:
    void onShowApplicationMenuAtCursorPos();
};

#endif // CONTROLLER_H
