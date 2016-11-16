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

// Application:
#include "controller.h"
#include "dimmerwidget.h"
#include "settingswindow.h"
#include "aboutwindow.h"
#include "utils.h"
#include "blynk.h"
#include "fluxlib.h"

// Defines:
#define PARAMETERS_FILE "parameters.xml"
#define ONE_HOUR 3600
#define THREE_HOURS 10800
#define ONE_DAY 86400

// Constructor:
Controller::Controller(QObject *parent) : QObject(parent),
    // Create tray icon:
    m_pTrayIcon(new QSystemTrayIcon()),
    m_pTrayIconMenu(new QMenu()),
    m_pParameters(new Parameters()),
    m_pDimmerWidget(new DimmerWidget(":/icons/ico-bigeye.gif")),
    m_pSettingsWindow(new SettingsWindow("Blynk")),
    m_pAboutWindow(new AboutWindow("About Blynk...")),
    m_iBlynkCursorElapsedTime(0),
    m_iScreenBreakElapsedTime(0),
    m_iBlueLightReducerElapsedTime(0),
    m_bBlynkCursorRandomModeOn(false),
    m_iScreenBreakDelay(0),
    m_iCurrentTemperature(0)
{
    // Set parameters on settings window:
    connect(m_pSettingsWindow, &SettingsWindow::showApplicationMenuAtCursorPos, this, &Controller::onShowApplicationMenuAtCursorPos);

    // Context menu about to show:
    connect(m_pTrayIconMenu, &QMenu::aboutToShow, this, &Controller::onContextMenuAboutToShow);
    connect(m_pTrayIconMenu, &QMenu::aboutToHide, this, &Controller::onContextMenuAboutToHide);

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

    // Settings window:
    delete m_pSettingsWindow;

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
    m_pDimmerWidget->setStrength(Parameters::NO_STRENGTH);

    // Save parameters:
    saveParameters();
}

// Create actions:
void Controller::createMenu(const CXMLNode &menuNode, QMenu *pRootMenu)
{
    // Exclusive menu?
    bool bIsExclusiveMenu = false;
    QFont segoeUIFont("Segoe-UI", 10);
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
            segoeUIFont.setBold(bBold);
            pAction->setFont(segoeUIFont);

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
            segoeUIFont.setBold(bBold);
            pSubMenu->setFont(segoeUIFont);

            // Set enabled state:
            pSubMenu->setEnabled(bEnabled);
            pSubMenu->setProperty("exclusiveMenu", bIsExclusive);
            pSubMenu->setEnabled(bEnabled);

            // Add menu:
            pRootMenu->addMenu(pSubMenu);
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
    m_pTrayIcon->setIcon(QIcon(":/icons/ico-eye.png"));

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
    m_pSettingsWindow->setTooltips(mTooltipValues);
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
            m_pSettingsWindow->raise();
            m_pSettingsWindow->updateUI();
            m_pSettingsWindow->show();

            // Check blynk cursor random mode:
            bool bBlynkCursorEnabled = (bool)m_pParameters->parameter(Parameters::BLYNK_CURSOR_ENABLED).toInt();
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
        // Screen break disabled for an hour:
        if (sObjectName == "screenBreakDisabledForOneHour")
        {
            m_iScreenBreakDelay = ONE_HOUR;
            m_pParameters->setParameter(Parameters::SCREEN_BREAK_STATE, SCREEN_BREAK_DISABLED_FOR_ONE_HOUR);
            m_iScreenBreakElapsedTime = 0;
        }
        else
        // Screen break disabled for three hour:
        if (sObjectName == "screenBreakDisabledForThreeHours")
        {
            m_iScreenBreakDelay = THREE_HOURS;
            m_pParameters->setParameter(Parameters::SCREEN_BREAK_STATE, SCREEN_BREAK_DISABLED_FOR_THREE_HOURS);
            m_iScreenBreakElapsedTime = 0;
        }
        else
        // Screen break disabled until tomorrow:
        if (sObjectName == "screenBreakDisabledUntilTomorrow")
        {
            m_iScreenBreakDelay = ONE_DAY;
            m_pParameters->setParameter(Parameters::SCREEN_BREAK_STATE, SCREEN_BREAK_DISABLED_UNTIL_TOMORROW);
            m_iScreenBreakElapsedTime = 0;
        }
        else
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
    m_pSettingsWindow->setParameters(m_pParameters);
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
    m_lBlynkCursorSequence = Utils::randomSequence(iBlynkPerMinuteRandom, 5, 50);
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
    bool bBlynkCursorEnabled = (bool)m_pParameters->parameter(Parameters::BLYNK_CURSOR_ENABLED).toInt();
    bool bBlynkCursorRandomMode = (bool)m_pParameters->parameter(Parameters::BLYNK_CURSOR_RANDOM_MODE).toInt();

    if (m_bBlynkCursorRandomModeOn != bBlynkCursorRandomMode)
    {
        if (bBlynkCursorRandomMode)
            enterBlynkCursorRandomMode();
        else
            enterBlynCursorRegularMode();
        m_bBlynkCursorRandomModeOn = bBlynkCursorRandomMode;
    }

    QString sScreenBreakState = m_pParameters->parameter(Parameters::SCREEN_BREAK_STATE);
    bool bScreenBreakEnabled = (sScreenBreakState == SCREEN_BREAK_ENABLED);

    // Read screen break state:
    if ((m_iScreenBreakDelay > 0) && (m_iScreenBreakElapsedTime > m_iScreenBreakDelay))
        m_pParameters->setParameter(Parameters::SCREEN_BREAK_STATE, SCREEN_BREAK_ENABLED);

    bScreenBreakEnabled &= (m_iScreenBreakElapsedTime >= m_iScreenBreakDelay);

    // Blue light reducer enabled?
    bool bBlueLightReducerEnabled = (bool)m_pParameters->parameter(Parameters::BLUE_LIGHT_REDUCER_ENABLED).toInt();

    // Blynk cursor only:
    if (m_iBlynkCursorElapsedTime > 0)
    {
        // Blynk cursor enabled:
        if (bBlynkCursorEnabled)
        {
            // Regular mode:
            if (!bBlynkCursorRandomMode)
            {
                int iBlynkCursorRegularity = m_pParameters->parameter(Parameters::BLYNK_CURSOR_REGULARITY).toInt();
                if (m_iBlynkCursorElapsedTime%iBlynkCursorRegularity == 0) {
                    m_pDimmerWidget->playCursor();
                }
            }
            // Random mode:
            else
            {
                if (!m_lBlynkCursorSequence.isEmpty())
                {
                    if (m_iBlynkCursorElapsedTime%m_lBlynkCursorSequence.first() == 0)
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
        int iScreenBreakRegularity = m_pParameters->parameter(Parameters::SCREEN_BREAK_REGULARITY).toInt()*60;

        // Strength:
        Parameters::Strength eScreenBreakStrength = (Parameters::Strength)m_pParameters->parameter(Parameters::SCREEN_BREAK_STRENGTH).toInt();
        if (m_iScreenBreakElapsedTime%iScreenBreakRegularity == 0) {
            m_pDimmerWidget->playBigEye(eScreenBreakStrength);
        }
    }

    // Blue light reducer:
    if (m_iBlueLightReducerElapsedTime > 0)
    {
        // Blynk light reducer:
        if (bBlueLightReducerEnabled)
        {
            QTime tTriggerTime = QTime::fromString(m_pParameters->parameter(Parameters::BLUE_LIGHT_REDUCER_START_TIME));

            if (QTime::currentTime() < tTriggerTime)
                m_pDimmerWidget->setTemperature(0);
            else {
                // Set current temperature:
                m_iCurrentTemperature = temperatureForStrength((Parameters::Strength)m_pParameters->parameter(Parameters::BLUE_LIGHT_REDUCER_STRENGTH).toInt());
                m_pDimmerWidget->setTemperature(m_iCurrentTemperature);
            }
        }
        else m_pDimmerWidget->setTemperature(0);
    }

    m_iBlynkCursorElapsedTime++;
    m_iScreenBreakElapsedTime++;
    m_iBlueLightReducerElapsedTime++;
}

// Context menu about to show:
void Controller::onContextMenuAboutToShow()
{
    QString sScreenBreakState = m_pParameters->parameter(Parameters::SCREEN_BREAK_STATE);
    if (m_mActions["screenBreakDisabledForOneHour"])
        m_mActions["screenBreakDisabledForOneHour"]->setChecked(sScreenBreakState == SCREEN_BREAK_DISABLED_FOR_ONE_HOUR);
    if (m_mActions["screenBreakDisabledForThreeHours"])
        m_mActions["screenBreakDisabledForThreeHours"]->setChecked(sScreenBreakState == SCREEN_BREAK_DISABLED_FOR_THREE_HOURS);
    if (m_mActions["screenBreakDisabledUntilTomorrow"])
        m_mActions["screenBreakDisabledUntilTomorrow"]->setChecked(sScreenBreakState == SCREEN_BREAK_DISABLED_UNTIL_TOMORROW);
}

// Context menu about to hide:
void Controller::onContextMenuAboutToHide()
{
    if (m_mActions["settings"])
        m_mActions["settings"]->setVisible(true);
}

// Return color for temperature:
QColor Controller::colorForTemperature(int iTemperature) const
{
    return Fluxlib::colorForTemperature1(iTemperature);

    /* Keep this as a reference
    if (iTemperature <= 1700)
        return QColor(255, 122, 0);
    if ((iTemperature > 1700) && (iTemperature <= 1850))
        return QColor(255, 130, 0);
    if ((iTemperature > 1850) && (iTemperature <= 2400))
        return QColor(255, 157, 60);
    if ((iTemperature > 2400) && (iTemperature <= 2550))
        return QColor(255, 164, 72);
    if ((iTemperature > 2550) && (iTemperature <= 2700))
        return QColor(255, 170, 84);
    if ((iTemperature > 2700) && (iTemperature <= 3000))
        return QColor(255, 181, 105);
    if ((iTemperature > 3000) && (iTemperature <= 3200))
        return QColor(255, 188, 118);
    if ((iTemperature > 3200) && (iTemperature <= 3350))
        return QColor(255, 192, 127);
    if ((iTemperature > 3350) && (iTemperature <= 4100))
        return QColor(255, 212, 168);
    if ((iTemperature > 4100) && (iTemperature <= 5000))
        return QColor(255, 229, 206);
    if ((iTemperature > 5000) && (iTemperature <= 6000))
        return QColor(255, 240, 232);
    if ((iTemperature > 6000) && (iTemperature <= 6200))
        return QColor(255, 246, 246);
    if (iTemperature > 6200)
        return QColor(255, 249, 255);
    return QColor(255, 255, 255);
    */
}

// Return color for strength:
QColor Controller::colorForStrength(const Parameters::Strength &eStrength)
{
    int iTemperature = temperatureForStrength(eStrength);
    return colorForTemperature(iTemperature);
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

// Set temperature:
void Controller::setTemperature(int iTemperature)
{
    m_iCurrentTemperature = iTemperature;
}

// Show application menu at cursor pos:
void Controller::onShowApplicationMenuAtCursorPos()
{
    if (m_mActions["settings"])
        m_mActions["settings"]->setVisible(false);
    m_pTrayIconMenu->exec(QCursor::pos() + QPoint(0, -m_pTrayIconMenu->sizeHint().height()));
}
