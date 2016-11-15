// Qt:
#include <QDesktopWidget>
#include <QPainter>
#include <QMouseEvent>
#include <QPushButton>
#include <QDebug>

// Application:
#include "settingswindow.h"
#include "ui_settingswindow.h"
#include "cursorpage.h"
#include "screenbreakpage.h"
#include "bluelightreducerpage.h"
#include "utils.h"

// Constructor:
SettingsWindow::SettingsWindow(const QString &sTitle, QWidget *parent) :
    CustomWindow(sTitle, parent),
    ui(new Ui::SettingsWindow),
    m_pCursorPage(NULL),
    m_pScreenBreakPage(NULL),
    m_pBlueLightReducerPage(NULL),
    m_pParameters(NULL)
{
    // Setup UI:
    ui->setupUi(this);
    ui->wBlynkSupport->setText("<a href=\"http://www.google.com/\">Blynk support</a>");
    ui->wBlynkSupport->setTextFormat(Qt::RichText);
    ui->wBlynkSupport->setTextInteractionFlags(Qt::TextBrowserInteraction);
    ui->wBlynkSupport->setOpenExternalLinks(true);

    // Add pages:
    m_pCursorPage = new CursorPage(this);
    ui->stackedWidget->addWidget(m_pCursorPage);
    m_pScreenBreakPage = new ScreenBreakPage(this);
    ui->stackedWidget->addWidget(m_pScreenBreakPage);
    m_pBlueLightReducerPage = new BlueLightReducerPage(this);
    ui->stackedWidget->addWidget(m_pBlueLightReducerPage);

    // Change tab:
    QButtonGroup *pButtonGroup = new QButtonGroup(this);
    pButtonGroup->setExclusive(true);
    pButtonGroup->addButton(ui->wCursorButton);
    connect(ui->wCursorButton, &QPushButton::clicked, this, &SettingsWindow::onCursorButtonClicked);
    pButtonGroup->addButton(ui->wScreenBreakButton);
    connect(ui->wScreenBreakButton, &QPushButton::clicked, this, &SettingsWindow::onScreenBreakButtonClicked);
    pButtonGroup->addButton(ui->wBlueLightReducerButton);
    connect(ui->wBlueLightReducerButton, &QPushButton::clicked, this, &SettingsWindow::onBlueLightReducerButtonClicked);

    // Menu button:
    connect(ui->wMenuButton, &QPushButton::clicked, this, &SettingsWindow::onMenuButtonClicked);
}

// Destructor:
SettingsWindow::~SettingsWindow()
{
    delete ui;
}

// Set parameters:
void SettingsWindow::setParameters(Parameters *pParameters)
{
    m_pParameters = pParameters;
    m_pCursorPage->setParameters(pParameters);
    m_pScreenBreakPage->setParameters(pParameters);
    m_pBlueLightReducerPage->setParameters(pParameters);
}

// Update blynk cursor area:
void SettingsWindow::updateBlynkCursorArea()
{
    m_pCursorPage->updateUI();
}

// Update screen break area:
void SettingsWindow::updateScreenBreakArea()
{
    m_pScreenBreakPage->updateUI();
}

// Update blue light reducer area:
void SettingsWindow::updateBlueLightReducerArea()
{
    m_pBlueLightReducerPage->updateUI();
}

// Update UI:
void SettingsWindow::updateUI()
{
    m_pCursorPage->updateUI();
    m_pScreenBreakPage->updateUI();
    m_pBlueLightReducerPage->updateUI();
}

// Cursor button clicked:
void SettingsWindow::onCursorButtonClicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

// Screen break button clicked:
void SettingsWindow::onScreenBreakButtonClicked()
{
    ui->stackedWidget->setCurrentIndex(1);
}

// Blue light reducer button clicked:
void SettingsWindow::onBlueLightReducerButtonClicked()
{
    ui->stackedWidget->setCurrentIndex(2);
}

// Menu button clicked:
void SettingsWindow::onMenuButtonClicked()
{
    emit showApplicationMenuAtCursorPos();
}
