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
class PreferenceDialog;

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

    // Preferences dialog:
    PreferenceDialog *m_pPreferenceDialog;

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
    int m_iDelay;

public slots:
    // Action triggered:
    void onActionTriggered();

    // Context menu about to show:
    void onContextMenuAboutToShow();
};

#endif // CONTROLLER_H
