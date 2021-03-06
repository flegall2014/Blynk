// Qt:
#include <QAction>
#include <QSettings>
#include <QMenu>
#include <QDebug>
#include <QtGui>
#include <QSystemTrayIcon>
#include <QApplication>
#include <QDesktopWidget>
#include <QPainter>
#include <QMessageBox>

// Application:
#include "controller.h"
#include "dimmerwidget.h"
#include "blynkwindow.h"
#include "aboutwindow.h"
#include "utils.h"
#include <monitor.h>

// Defines:
#define PARAMETERS_FILE "BlynkParameters.xml"
#define NSEC_PER_HOUR 3600
#define NSEC_IN_THREE_HOURS 10800
#define NSEC_IN_ONE_DAY 86400
#define NSEC_PER_MINUTE 60
#define VISION_AIDS_OVERSEA_URL "http://getblynk.org/visionaid-overseas-blynk"

// Constructor:
Controller::Controller(QObject *parent) : QObject(parent),
    // Create tray icon:
    m_pTrayIcon(new QSystemTrayIcon()),
    m_pTrayIconMenu(new QMenu()),
    m_pParameters(new Parameters()),
    m_pDimmerWidget(new DimmerWidget()),
    m_pBlynkWindow(new BlynkWindow("Blynk")),
    m_pAboutWindow(new AboutWindow("About Blynk...")),
    m_iBlynkCursorElapsedTime(0),
    m_iScreenBreakElapsedTime(0),
    m_iBlueLightReducerElapsedTime(0),
    m_bBlynkCursorRandomModeOn(false),
    m_iBlynkCursorDelay(0),
    m_iScreenBreakDelay(0),
    m_iBlueLightReducerDelay(0)
{
    #ifdef QT_DEBUG
    m_debugDialog.setParameters(m_pParameters);
    m_debugDialog.initialize(m_pDimmerWidget->monitor()->brightness(),
        m_pDimmerWidget->monitor()->redGamma(), m_pDimmerWidget->monitor()->greenGamma(),
            m_pDimmerWidget->monitor()->blueGamma());
    m_debugDialog.show();
    #endif

    // Context menu about to show:
    connect(m_pTrayIconMenu, &QMenu::aboutToShow, this, &Controller::onContextMenuAboutToShow);
    connect(m_pTrayIconMenu, &QMenu::aboutToHide, this, &Controller::onContextMenuAboutToHide);

    // Listen to parameter changed:
    connect(m_pParameters, &Parameters::parameterChanged, this, &Controller::onParameterChanged);
    #ifdef QT_DEBUG
    connect(m_pParameters, &Parameters::parameterChanged, &m_debugDialog, &DebugDialog::onParameterChanged);
    connect(&m_debugDialog, &DebugDialog::brightnessChanged, this, &Controller::onBrightnessChanged);
    connect(&m_debugDialog, &DebugDialog::redGammaChanged, this, &Controller::onRedGammaChanged);
    connect(&m_debugDialog, &DebugDialog::greenGammaChanged, this, &Controller::onGreenGammaChanged);
    connect(&m_debugDialog, &DebugDialog::blueGammaChanged, this, &Controller::onBlueGammaChanged);
    #endif

    // Parametrize timer:
    m_tApplicationTimer.setInterval(1000);
    connect(&m_tApplicationTimer, &QTimer::timeout, this, &Controller::onApplicationTimerTimeOut);
}

// Destructor:
Controller::~Controller()
{
    qDebug() << "DESTROY CONTROLLER";
    releaseAll();
}

// Release all:
void Controller::releaseAll()
{
    // Tray icon:
    delete m_pTrayIcon;

    // Tray icon menu:
    delete m_pTrayIconMenu;

    // Parameters:
    delete m_pParameters;

    // Dimmer widget:
    delete m_pDimmerWidget;

    // Blynk window:
    delete m_pBlynkWindow;

    // About window:
    delete m_pAboutWindow;
}

// Startup:
bool Controller::startup()
{
    // Load parameters:
    loadParameters();

    // Start GUI:
    startGUI();

    // Start application timer:
    m_tApplicationTimer.start();

    return true;
}

// Shutdown:
void Controller::shutdown()
{
    qDebug() << "SHUTDOWN";

    // Reset dimmer:
    m_pDimmerWidget->setTemperature(0);

    // Save parameters:
    saveParameters();
}

// Create actions:
void Controller::createMenu(const CXMLNode &menuNode, QMenu *pRootMenu)
{
    QFont myriadFont;
    myriadFont.setFamily(Utils::sOpenSansRegular);
    myriadFont.setPointSize(FONT_12);

    // Exclusive menu?
    bool bIsExclusiveMenu = false;
    QActionGroup *pActionGroup = NULL;
    if (pRootMenu)
    {
        bIsExclusiveMenu = pRootMenu->property("exclusiveMenu").toBool();
        if (bIsExclusiveMenu)
        {
            pActionGroup = new QActionGroup(this);
            pActionGroup->setExclusive(true);
        }
    }

    for (auto node : menuNode.nodes())
    {
        QString sTag = node.tag();
        QString sEnabled = node.attributes()["enabled"].simplified();
        bool bEnabled = sEnabled.isEmpty() ? true : (bool)sEnabled.toInt();
        QString sIsChecked = node.attributes()["checked"].simplified();
        bool bIsChecked = sIsChecked.isEmpty() ? false : (bool)sIsChecked.toInt();
        QString sIsBold = node.attributes()["bold"].simplified();
        bool bBold = sIsBold.isEmpty() ? false : (bool)sIsBold.toInt();

        // Menu item:
        if (sTag == "MenuItem")
        {
            // Set action name:
            QString sActionName = node.attributes()["name"];
            QString sObjectName = node.attributes()["objectName"];
            QAction *pAction = new QAction(sActionName, this);

            // Set font:
            myriadFont.setBold(bBold);
            pAction->setFont(myriadFont);

            // Store action:
            m_mActions[sObjectName] = pAction;

            // Set enabled state:
            pAction->setEnabled(bEnabled);
            connect(pAction, &QAction::triggered, this, &Controller::onActionTriggered);
            pAction->setObjectName(node.attributes()["objectName"]);
            pRootMenu->addAction(pAction);

            // Add to exclusive group:
            if (pActionGroup)
            {
                pAction->setCheckable(true);
                pAction->setChecked(bIsChecked);
                pActionGroup->addAction(pAction);
            }
        }
        else
        // Menu:
        if (sTag == "Menu")
        {
            // Set menu title:
            bool bIsExclusive = (bool)node.attributes()["exclusive"].toInt();
            QMenu *pSubMenu = new QMenu(node.attributes()["name"]);

            // Set font:
            myriadFont.setBold(bBold);

            // Set enabled state:
            pSubMenu->setEnabled(bEnabled);
            pSubMenu->setProperty("exclusiveMenu", bIsExclusive);
            pSubMenu->setEnabled(bEnabled);

            // Add menu:
            QAction *pSubMenuAction = pRootMenu->addMenu(pSubMenu);
            pSubMenuAction->setFont(myriadFont);
            createMenu(node, pSubMenu);
        }
        else
        // Separator:
        if (sTag == "Separator")
            pRootMenu->addSeparator();
    }
}

// Start GUI:
void Controller::startGUI()
{
    /*** LOAD FONTS ***/
    Utils::loadFonts();

    /*** TRAY ICON ***/

    initializeTrayIcon();

    /*** TOOLTIP ***/

    createTooltip();

    /*** BLYNK CURSOR ***/

    // Check blynk cursor random mode:
    bool bBlynkCursorRandomMode = (bool)m_pParameters->parameter(Parameters::BLYNK_CURSOR_RANDOM_MODE).toInt();
    if (bBlynkCursorRandomMode)
        enterBlynkCursorRandomMode();
    else
        enterBlynCursorRegularMode();

    // Show dimmer widget:
    m_pDimmerWidget->showFullScreen();
}

// Create tray icon:
void Controller::initializeTrayIcon()
{
    // Create actions:
    CXMLNode rootNode = CXMLNode::loadXMLFromFile(":/xml/BlynkMenu.xml");
    CXMLNode menuNode = rootNode.getNodeByTagName("Menu");
    createMenu(menuNode, m_pTrayIconMenu);

    // Set context menu:
    m_pTrayIcon->setContextMenu(m_pTrayIconMenu);

    // Set icon:
    m_pTrayIcon->setIcon(QIcon(":/icons/ico-tray.png"));

    // Show tray icon:
    m_pTrayIcon->setVisible(true);
}

// Create tooltip:
void Controller::createTooltip()
{
    // Create actions:
    CXMLNode rootNode = CXMLNode::loadXMLFromFile(":/xml/BlynkMenu.xml");
    CXMLNode tooltipNode = rootNode.getNodeByTagName("Tooltip");
    QVector<CXMLNode> vTooltipItems = tooltipNode.getNodesByTagName("TooltipItem");
    QMap<QString, QString> mTooltipValues;
    for (auto node : vTooltipItems)
    {
        QString sTooltipName = node.attributes()["name"];
        QString sTooltipValue = node.attributes()["value"];
        mTooltipValues[sTooltipName] = sTooltipValue;
    }
    m_pBlynkWindow->setTooltips(mTooltipValues);
}

// Action triggered:
void Controller::onActionTriggered()
{
    if (!m_pParameters)
        return;
    QAction *pSender = dynamic_cast<QAction *>(sender());
    if (pSender)
    {
        QString sObjectName = pSender->objectName();

        // Show preferences:
        if (sObjectName == "settings")
        {
            // Show window:
            m_pBlynkWindow->raise();
            m_pBlynkWindow->show();

            // Read blynk cursor state:
            QString sBlynkCursorState = m_pParameters->parameter(Parameters::BLYNK_CURSOR_STATE);
            bool bBlynkCursorEnabled = (sBlynkCursorState == BLYNK_CURSOR_ENABLED);
            bool bBlynkCursorRandomMode = (bool)m_pParameters->parameter(Parameters::BLYNK_CURSOR_RANDOM_MODE).toInt();
            if (bBlynkCursorEnabled)
            {
                if (bBlynkCursorRandomMode)
                    enterBlynkCursorRandomMode();
                else
                    enterBlynCursorRegularMode();
            }
        }
        else
        // Cursor disabled for an hour:
        if (sObjectName == "blynkCursorDisabledForOneHour")
        {
            m_iBlynkCursorDelay = NSEC_PER_HOUR;
            m_pParameters->setParameter(Parameters::BLYNK_CURSOR_STATE, BLYNK_CURSOR_DISABLED_FOR_ONE_HOUR);
            m_iBlynkCursorElapsedTime = 0;
        }
        else
        // Screen break disabled for three hour:
        if (sObjectName == "blynkCursorDisabledForThreeHours")
        {
            m_iBlynkCursorDelay = NSEC_IN_THREE_HOURS;
            m_pParameters->setParameter(Parameters::BLYNK_CURSOR_STATE, BLYNK_CURSOR_DISABLED_FOR_THREE_HOURS);
            m_iBlynkCursorElapsedTime = 0;
        }
        else
        // Screen break disabled until tomorrow:
        if (sObjectName == "blynkCursorDisabledUntilTomorrow")
        {
            m_iBlynkCursorDelay = NSEC_IN_ONE_DAY;
            m_pParameters->setParameter(Parameters::BLYNK_CURSOR_STATE, BLYNK_CURSOR_DISABLED_UNTIL_TOMORROW);
            m_iBlynkCursorElapsedTime = 0;
        }
        else
        // Screen break disabled for an hour:
        if (sObjectName == "screenBreakDisabledForOneHour")
        {
            m_iScreenBreakDelay = NSEC_PER_HOUR;
            m_pParameters->setParameter(Parameters::SCREEN_BREAK_STATE, SCREEN_BREAK_DISABLED_FOR_ONE_HOUR);
            m_iScreenBreakElapsedTime = 0;
        }
        else
        // Screen break disabled for three hour:
        if (sObjectName == "screenBreakDisabledForThreeHours")
        {
            m_iScreenBreakDelay = NSEC_IN_THREE_HOURS;
            m_pParameters->setParameter(Parameters::SCREEN_BREAK_STATE, SCREEN_BREAK_DISABLED_FOR_THREE_HOURS);
            m_iScreenBreakElapsedTime = 0;
        }
        else
        // Screen break disabled until tomorrow:
        if (sObjectName == "screenBreakDisabledUntilTomorrow")
        {
            m_iScreenBreakDelay = NSEC_IN_ONE_DAY;
            m_pParameters->setParameter(Parameters::SCREEN_BREAK_STATE, SCREEN_BREAK_DISABLED_UNTIL_TOMORROW);
            m_iScreenBreakElapsedTime = 0;
        }
        else
        // Blue light reducer disabled for an hour:
        if (sObjectName == "blueLightReducerDisabledForOneHour")
        {
            m_iBlueLightReducerDelay = NSEC_PER_HOUR;
            m_pParameters->setParameter(Parameters::BLUELIGHTREDUCER_STATE, BLUELIGHTREDUCER_DISABLED_FOR_ONE_HOUR);
            m_iBlueLightReducerElapsedTime = 0;
        }
        else
        // Blue light reducer disabled for three hour:
        if (sObjectName == "blueLightReducerDisabledForThreeHours")
        {
            m_iBlueLightReducerDelay = NSEC_IN_THREE_HOURS;
            m_pParameters->setParameter(Parameters::BLUELIGHTREDUCER_STATE, BLUELIGHTREDUCER_DISABLED_FOR_THREE_HOURS);
            m_iBlueLightReducerElapsedTime = 0;
        }
        else
        // Blue light reducer disabled until tomorrow:
        if (sObjectName == "blueLightReducerDisabledUntilTomorrow")
        {
            m_iBlueLightReducerDelay = NSEC_IN_ONE_DAY;
            m_pParameters->setParameter(Parameters::BLUELIGHTREDUCER_STATE, BLUELIGHTREDUCER_DISABLED_UNTIL_TOMORROW);
            m_iBlueLightReducerElapsedTime = 0;
        }
        else
        // Vision aids overseas:
        if (sObjectName == "visionAidsOverseas")
        {
            QDesktopServices::openUrl(QUrl(VISION_AIDS_OVERSEA_URL));
        }
        else
        // About Blynk:
        if (sObjectName == "aboutBlynk")
        {
            // Show window:
            m_pAboutWindow->raise();
            m_pAboutWindow->show();
        }
        // Quit:
        if (sObjectName == "quitBlynk")
        {
            qApp->quit();
        }

        // Update UI:
        m_pBlynkWindow->updateUI();
        m_pBlynkWindow->setEnabledState();
    }
}

// Load parameters:
void Controller::loadParameters()
{
    QString sParametersFile = Utils::appDir().absoluteFilePath(PARAMETERS_FILE);
    if (QFile::exists(sParametersFile))
    {
        CXMLNode xRootNode = CXMLNode::loadXMLFromFile(sParametersFile);
        m_pParameters->deserialize(xRootNode);
        m_bBlynkCursorRandomModeOn = (bool)m_pParameters->parameter(Parameters::BLYNK_CURSOR_RANDOM_MODE).toInt();
    }

    // Update UI:
    m_pDimmerWidget->setParameters(m_pParameters);
    m_pBlynkWindow->setParameters(m_pParameters);
}

// Save parameters:
void Controller::saveParameters()
{
    CXMLNode rootNode = m_pParameters->serialize();
    QString sParametersFile = Utils::appDir().absoluteFilePath(PARAMETERS_FILE);
    rootNode.saveXMLToFile(sParametersFile);
}

// Enter blynk cursor random mode:
void Controller::enterBlynkCursorRandomMode()
{
    m_tApplicationTimer.stop();
    m_iBlynkCursorElapsedTime = 0;
    int iBlynkPerMinuteRandom = m_pParameters->parameter(Parameters::BLYNK_PER_MINUTE_RANDOM).toInt();
    m_lBlynkCursorSequence = Utils::randomSequence(iBlynkPerMinuteRandom, 0, 59);
    m_tApplicationTimer.start();
}

// Enter blynk cursor regular mode:
void Controller::enterBlynCursorRegularMode()
{
    m_tApplicationTimer.stop();
    m_iBlynkCursorElapsedTime = 0;
    m_tApplicationTimer.start();
}

// Timeout:
void Controller::onApplicationTimerTimeOut()
{
    if (!m_pParameters)
        return;

    // Read blynk cursor state:
    QString sBlynkCursorState = m_pParameters->parameter(Parameters::BLYNK_CURSOR_STATE);
    bool bBlynkCursorEnabled = (sBlynkCursorState == BLYNK_CURSOR_ENABLED);

    // Read blynk cursor random mode:
    bool bBlynkCursorRandomMode = (bool)m_pParameters->parameter(Parameters::BLYNK_CURSOR_RANDOM_MODE).toInt();

    // Check blynk cursor delay:
    if ((m_iBlynkCursorDelay > 0) && (m_iBlynkCursorElapsedTime > m_iBlynkCursorDelay)) {
        m_pParameters->setParameter(Parameters::BLYNK_CURSOR_STATE, BLYNK_CURSOR_ENABLED);
        m_pBlynkWindow->updateBlynkCursorArea();
        bBlynkCursorEnabled = true;
    }

    if (bBlynkCursorEnabled && (m_bBlynkCursorRandomModeOn != bBlynkCursorRandomMode))
    {
        if (bBlynkCursorRandomMode)
            enterBlynkCursorRandomMode();
        else
            enterBlynCursorRegularMode();
        m_bBlynkCursorRandomModeOn = bBlynkCursorRandomMode;
    }

    // Read screen break state:
    QString sScreenBreakState = m_pParameters->parameter(Parameters::SCREEN_BREAK_STATE);
    bool bScreenBreakEnabled = (sScreenBreakState == SCREEN_BREAK_ENABLED);

    // Check screen break delay:
    if ((m_iScreenBreakDelay > 0) && (m_iScreenBreakElapsedTime > m_iScreenBreakDelay)) {
        m_pParameters->setParameter(Parameters::SCREEN_BREAK_STATE, SCREEN_BREAK_ENABLED);
        m_pBlynkWindow->updateScreenBreakArea();
        bScreenBreakEnabled = true;
    }

    // Blue light reducer enabled?
    QString sBlueLightReducerState = m_pParameters->parameter(Parameters::BLUELIGHTREDUCER_STATE);
    bool bBlueLightReducerEnabled = (sBlueLightReducerState == BLUELIGHTREDUCER_ENABLED);

    // Check blue light reducer delay:
    if ((m_iBlueLightReducerDelay > 0) && (m_iBlueLightReducerElapsedTime > m_iBlueLightReducerDelay)) {
        m_pParameters->setParameter(Parameters::BLUELIGHTREDUCER_STATE, BLUELIGHTREDUCER_ENABLED);
        m_pBlynkWindow->updateBlueLightReducerArea();
        bBlueLightReducerEnabled = true;
    }

    // Read blue light reducer state:
    if (m_iBlynkCursorElapsedTime > 0)
    {
        // Blynk cursor enabled:
        if (bBlynkCursorEnabled)
        {
            // Regular mode:
            if (!bBlynkCursorRandomMode)
            {
                int iBlynkCursorRegularity = m_pParameters->parameter(Parameters::BLYNK_CURSOR_REGULARITY).toInt();
                if ((iBlynkCursorRegularity > 0) && (m_iBlynkCursorElapsedTime%iBlynkCursorRegularity == 0)) {
                    m_pDimmerWidget->playCursor();
                }
            }
            // Random mode:
            else
            {
                if (!m_lBlynkCursorSequence.isEmpty())
                {
                    if ((m_lBlynkCursorSequence.first() > 0) && (m_iBlynkCursorElapsedTime%m_lBlynkCursorSequence.first() == 0))
                    {
                        m_pDimmerWidget->playCursor();
                        m_lBlynkCursorSequence.takeFirst();

                        if (m_lBlynkCursorSequence.isEmpty())
                        {
                            int iBlynkPerMinuteRandom = m_pParameters->parameter(Parameters::BLYNK_PER_MINUTE_RANDOM).toInt();
                            m_lBlynkCursorSequence = Utils::randomSequence(iBlynkPerMinuteRandom, 5, 50);
                        }
                    }
                }
            }
        }
    }

    // Screen break:
    if (bScreenBreakEnabled && (m_iScreenBreakElapsedTime > 0))
    {
        // Regularity:
        int iScreenBreakRegularity = m_pParameters->parameter(Parameters::SCREEN_BREAK_REGULARITY).toInt()*NSEC_PER_MINUTE;

        // Strength:
        if (iScreenBreakRegularity > 0)
        {
            Parameters::Strength eScreenBreakStrength = (Parameters::Strength)m_pParameters->parameter(Parameters::SCREEN_BREAK_STRENGTH).toInt();
            if ((iScreenBreakRegularity > 0) && (m_iScreenBreakElapsedTime%iScreenBreakRegularity == 0)) {
                m_pDimmerWidget->playAnimatedImage(eScreenBreakStrength);
            }
        }
    }

    // Blue light reducer:
    if (bBlueLightReducerEnabled && (m_iBlueLightReducerElapsedTime > 0))
    {
        QTime tTriggerTime = QTime::fromString(m_pParameters->parameter(Parameters::BLUE_LIGHT_REDUCER_START_TIME));

        bool bBlueLightReducedAlwaysOn = m_pParameters->parameter(Parameters::BLUE_LIGHT_REDUCER_ALWAYS_ON) == ON;

        // Always on, don't care about trigger time:
        if (bBlueLightReducedAlwaysOn) {
            // Set current temperature:
            int iCurrentTemperature = temperatureForStrength((Parameters::Strength)m_pParameters->parameter(Parameters::BLUE_LIGHT_REDUCER_STRENGTH).toInt());
            m_pDimmerWidget->setTemperature(iCurrentTemperature);
        }
        else {
            if (QTime::currentTime() < tTriggerTime)
                m_pDimmerWidget->setTemperature(0);
            else {
                // Set current temperature:
                int iCurrentTemperature = temperatureForStrength((Parameters::Strength)m_pParameters->parameter(Parameters::BLUE_LIGHT_REDUCER_STRENGTH).toInt());
                m_pDimmerWidget->setTemperature(iCurrentTemperature);
            }
        }
    }
    else m_pDimmerWidget->setTemperature(0);

    m_iBlynkCursorElapsedTime++;
    m_iScreenBreakElapsedTime++;
    m_iBlueLightReducerElapsedTime++;
}

// Context menu about to show:
void Controller::onContextMenuAboutToShow()
{
    // Read blynk cursor state:
    QString sBlynkCursorState = m_pParameters->parameter(Parameters::BLYNK_CURSOR_STATE);

    // Update blynk cursor options:
    if (m_mActions["blynkCursorDisabledForOneHour"])
        m_mActions["blynkCursorDisabledForOneHour"]->setChecked(sBlynkCursorState == BLYNK_CURSOR_DISABLED_FOR_ONE_HOUR);
    if (m_mActions["blynkCursorDisabledForThreeHours"])
        m_mActions["blynkCursorDisabledForThreeHours"]->setChecked(sBlynkCursorState == BLYNK_CURSOR_DISABLED_FOR_THREE_HOURS);
    if (m_mActions["blynkCursorDisabledUntilTomorrow"])
        m_mActions["blynkCursorDisabledUntilTomorrow"]->setChecked(sBlynkCursorState == BLYNK_CURSOR_DISABLED_UNTIL_TOMORROW);

    // Read screen break state:
    QString sScreenBreakState = m_pParameters->parameter(Parameters::SCREEN_BREAK_STATE);

    // Update screen break options:
    if (m_mActions["screenBreakDisabledForOneHour"])
        m_mActions["screenBreakDisabledForOneHour"]->setChecked(sScreenBreakState == SCREEN_BREAK_DISABLED_FOR_ONE_HOUR);
    if (m_mActions["screenBreakDisabledForThreeHours"])
        m_mActions["screenBreakDisabledForThreeHours"]->setChecked(sScreenBreakState == SCREEN_BREAK_DISABLED_FOR_THREE_HOURS);
    if (m_mActions["screenBreakDisabledUntilTomorrow"])
        m_mActions["screenBreakDisabledUntilTomorrow"]->setChecked(sScreenBreakState == SCREEN_BREAK_DISABLED_UNTIL_TOMORROW);

    // Read blue light reducer state:
    QString sBlueLightReducerState = m_pParameters->parameter(Parameters::BLUELIGHTREDUCER_STATE);

    // Update screen break options:
    if (m_mActions["blueLightReducerDisabledForOneHour"])
        m_mActions["blueLightReducerDisabledForOneHour"]->setChecked(sBlueLightReducerState == BLUELIGHTREDUCER_DISABLED_FOR_ONE_HOUR);
    if (m_mActions["blueLightReducerDisabledForThreeHours"])
        m_mActions["blueLightReducerDisabledForThreeHours"]->setChecked(sBlueLightReducerState == BLUELIGHTREDUCER_DISABLED_FOR_THREE_HOURS);
    if (m_mActions["blueLightReducerDisabledUntilTomorrow"])
        m_mActions["blueLightReducerDisabledUntilTomorrow"]->setChecked(sBlueLightReducerState == BLUELIGHTREDUCER_DISABLED_UNTIL_TOMORROW);
}

// Context menu about to hide:
void Controller::onContextMenuAboutToHide()
{
    if (m_mActions["settings"])
        m_mActions["settings"]->setVisible(true);
}

// Return temperature for strength:
int Controller::temperatureForStrength(const Parameters::Strength &eStrength)
{
    if (eStrength == Parameters::LIGHT)
        return m_pParameters->parameter(Parameters::LIGHT_TEMPERATURE).toInt();
    if (eStrength == Parameters::MEDIUM)
        return m_pParameters->parameter(Parameters::MEDIUM_TEMPERATURE).toInt();
    if (eStrength == Parameters::STRONG)
        return m_pParameters->parameter(Parameters::STRONG_TEMPERATURE).toInt();
    return 0;
}

// Show application menu at cursor pos:
void Controller::onShowApplicationMenuAtCursorPos()
{
    if (m_mActions["settings"])
        m_mActions["settings"]->setVisible(false);
    m_pTrayIconMenu->exec(QCursor::pos() + QPoint(0, -m_pTrayIconMenu->sizeHint().height()));
}

// Parameter changed:
void Controller::onParameterChanged(const Parameters::Parameter &parameter)
{
    // Make sure to reset blynk cursor delay in this case:
    if (parameter == Parameters::BLYNK_CURSOR_STATE)
    {
        qDebug() << "RESETTING BLYNK CURSOR DELAY";

        // Read blynk cursor state:
        QString sBlynkCursorState = m_pParameters->parameter(Parameters::BLYNK_CURSOR_STATE);
        bool bBlynkCursorEnabled = (sBlynkCursorState == BLYNK_CURSOR_ENABLED);
        if (bBlynkCursorEnabled)
            m_iBlynkCursorDelay = 0;
    }

    // Make sure to reset screen break delay in this case:
    if (parameter == Parameters::SCREEN_BREAK_STATE)
    {
        qDebug() << "RESETTING SCREEN BREAK DELAY";

        // Read screen break state:
        QString sScreenBreakState = m_pParameters->parameter(Parameters::BLYNK_CURSOR_STATE);
        bool bScreenBreakEnabled = (sScreenBreakState == SCREEN_BREAK_ENABLED);
        if (bScreenBreakEnabled)
            m_iScreenBreakDelay = 0;
    }
}

void Controller::onBrightnessChanged(double dBrightness)
{
    m_pDimmerWidget->monitor()->setBrightness(dBrightness);
}

void Controller::onRedGammaChanged(double dRedGamma)
{
    m_pDimmerWidget->monitor()->setRedGamma(dRedGamma);
}

void Controller::onGreenGammaChanged(double dGreenGamma)
{
    m_pDimmerWidget->monitor()->setGreenGamma(dGreenGamma);
}

void Controller::onBlueGammaChanged(double dBlueGamma)
{
    m_pDimmerWidget->monitor()->setBlueGamma(dBlueGamma);
}
