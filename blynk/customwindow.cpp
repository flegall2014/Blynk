// Qt:
#include <QDesktopWidget>
#include <QPainter>
#include <QMouseEvent>
#include <QPushButton>
#include <QDebug>

// Application:
#include "customwindow.h"
#include "ui_CustomWindow.h"
#include "cursorpage.h"
#include "screenbreakpage.h"
#include "bluelightreducerpage.h"
#include "customtooltip.h"

// Constructor:
CustomWindow::CustomWindow(const QString &sTitle, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CustomWindow),
    m_sTitle(sTitle),
    m_backgroundColor(QColor(229, 229, 229, 216)),
    m_headerColor(QColor(196, 196, 196)),
    m_iHeaderHeight(25),
    m_bCloseButtonPressed(false),
    m_pParameters(NULL),
    m_pCustomTooltip(NULL)
{
    // Setup UI:
    ui->setupUi(this);
    ui->wBlynkSupport->setText("<a href=\"http://www.google.com/\">Blynk support</a>");
    ui->wBlynkSupport->setTextFormat(Qt::RichText);
    ui->wBlynkSupport->setTextInteractionFlags(Qt::TextBrowserInteraction);
    ui->wBlynkSupport->setOpenExternalLinks(true);

    // Create custom tooltip:
    m_pCustomTooltip = new CustomTooltip();
    m_pCustomTooltip->hide();

    // Add pages:
    m_pCursorPage = new CursorPage(this);
    ui->stackedWidget->addWidget(m_pCursorPage);
    m_pScreenBreakPage = new ScreenBreakPage(this);
    ui->stackedWidget->addWidget(m_pScreenBreakPage);
    m_pBlueLightReducerPage = new BlueLightReducerPage(this);
    ui->stackedWidget->addWidget(m_pBlueLightReducerPage);

    // Set title:
    setWindowTitle(m_sTitle);

    // Set attributes:
    setWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);

    // Close button:
    m_pCloseButton = new QPushButton("X", this);
    m_pCloseButton->setFlat(true);
    m_pCloseButton->resize(20, 20);
    connect(m_pCloseButton, &QPushButton::clicked, this, &CustomWindow::onCloseButtonClicked);

    // Change tab:
    connect(ui->wCursorButton, &QPushButton::clicked, this, &CustomWindow::onCursorButtonClicked);
    connect(ui->wScreenBreakButton, &QPushButton::clicked, this, &CustomWindow::onScreenBreakButtonClicked);
    connect(ui->wBlueLightReducerButton, &QPushButton::clicked, this, &CustomWindow::onBlueLightReducerButtonClicked);

    // Menu button:
    connect(ui->wMenuButton, &QPushButton::clicked, this, &CustomWindow::onMenuButtonClicked);
}

// Destructor:
CustomWindow::~CustomWindow()
{
    delete ui;
    delete m_pCustomTooltip;
}

// Set parameters:
void CustomWindow::setParameters(Parameters *pParameters)
{
    qDebug() << "PROUT " << pParameters->parameter(Parameters::SCREEN_BREAK_REGULARITY).toInt();

    m_pParameters = pParameters;
    m_pCursorPage->setParameters(pParameters);
    m_pScreenBreakPage->setParameters(pParameters);
    m_pBlueLightReducerPage->setParameters(pParameters);
}

// Update blynk cursor area:
void CustomWindow::updateBlynkCursorArea()
{
    m_pCursorPage->updateUI();
}

// Update screen break area:
void CustomWindow::updateScreenBreakArea()
{
    m_pScreenBreakPage->updateUI();
}

// Update blue light reducer area:
void CustomWindow::updateBlueLightReducerArea()
{
    m_pBlueLightReducerPage->updateUI();
}

// Update UI:
void CustomWindow::updateUI()
{
    m_pCursorPage->updateUI();
    m_pScreenBreakPage->updateUI();
    m_pBlueLightReducerPage->updateUI();
}

// Set tooltips:
void CustomWindow::setTooltips(const QMap<QString, QString> &mTooltipValues)
{
    m_mTooltips = mTooltipValues;
}

// Handle close event:
void CustomWindow::closeEvent(QCloseEvent *e)
{
    hide();
    e->ignore();
}

// Handle event for tooltip display:
bool CustomWindow::event(QEvent *event)
{
    // Handle tooltip:
    if (event->type() == QEvent::ToolTip)
    {
        QHelpEvent *pHelpEvent = static_cast<QHelpEvent *>(event);
        if (pHelpEvent)
        {
            QPushButton *pButtonUnderMouse = dynamic_cast<QPushButton *>(childAt(pHelpEvent->pos()));
            if (pButtonUnderMouse != NULL)
            {
                if (m_pCustomTooltip)
                {
                    QString sTooltipText = m_mTooltips[pButtonUnderMouse->objectName()];
                    if (!sTooltipText.isEmpty())
                    {
                        m_pCustomTooltip->setText(sTooltipText);
                        m_pCustomTooltip->move(mapToGlobal(pHelpEvent->pos()));
                        m_pCustomTooltip->show();
                    }
                    else m_pCustomTooltip->hide();
                }
            }
            else m_pCustomTooltip->hide();
        }

        return true;
    }

    if (!containsMouse(QCursor::pos()) && m_pCustomTooltip && m_pCustomTooltip->isVisible())
        m_pCustomTooltip->hide();

    return QWidget::event(event);
}

// Contains mouse?
bool CustomWindow::containsMouse(const QPoint &point)
{
    return rect().contains(mapFromGlobal(point));
}

// Handle show event:
void CustomWindow::showEvent(QShowEvent *event)
{
    Q_UNUSED(event);
    m_pCloseButton->move(width()-28, 2);
    positionOnScreen();
}

// Position on screen:
void CustomWindow::positionOnScreen()
{
    QDesktopWidget screen;
    QRect screenGeom = screen.screenGeometry(this);
    int iScreenCenterX = screenGeom.width()-width()-8;
    int iScreenCenterY = screenGeom.height()-height()-64-8;
    move(iScreenCenterX, iScreenCenterY);
}

// Handle paint events:
void CustomWindow::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setPen(Qt::NoPen);

    // Draw window background:
    QRect background = rect();
    painter.setBrush(QBrush(m_backgroundColor));
    painter.drawRoundedRect(background, 8, 8);

    // Draw header:
    QRect headerRect(0, 0, rect().width(), m_iHeaderHeight);
    painter.setBrush(QBrush(m_headerColor));
    painter.drawRect(0, 12, rect().width(), 13);
    painter.drawRoundedRect(headerRect, 8, 8);

    // Draw blynk icon:
    QImage img(":/icons/ico-eye.png");
    painter.drawImage(4, (m_iHeaderHeight-img.height())/2, img);
    QRect textRect(img.width()+8, 0, rect().width()-(img.width()+8), m_iHeaderHeight);
    painter.setPen(QPen(QColor(72, 72, 72)));
    QFont segoeUIFont("Segoe-UI", 10);
    segoeUIFont.setBold(true);
    painter.setFont(segoeUIFont);
    painter.drawText(textRect, Qt::AlignVCenter, m_sTitle);
}

// Handle mouse press:
void CustomWindow::mousePressEvent(QMouseEvent *event)
{
    if (event->y() <= m_iHeaderHeight)
    {
        m_diff = event->pos();
        setCursor(QCursor(Qt::SizeAllCursor));
        m_bCloseButtonPressed = true;
    }
}

// Handle mouse release:
void CustomWindow::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    if (event->y() < m_iHeaderHeight) {
        setCursor(QCursor(Qt::ArrowCursor));
        m_bCloseButtonPressed = false;
    }
}

// Handle mouse move:
void CustomWindow::mouseMoveEvent(QMouseEvent *event)
{
    if (m_bCloseButtonPressed)
    {
        QPoint p = event->globalPos();
        move(p - m_diff);
    }
}

// Cursor button clicked:
void CustomWindow::onCursorButtonClicked()
{
    m_pCustomTooltip->hide();
    ui->stackedWidget->setCurrentIndex(0);
}

// Screen break button clicked:
void CustomWindow::onScreenBreakButtonClicked()
{
    m_pCustomTooltip->hide();
    ui->stackedWidget->setCurrentIndex(1);
}

// Blue light reducer button clicked:
void CustomWindow::onBlueLightReducerButtonClicked()
{
    m_pCustomTooltip->hide();
    ui->stackedWidget->setCurrentIndex(2);
}

// Close button clicked:
void CustomWindow::onCloseButtonClicked()
{
    hide();
}

// Menu button clicked:
void CustomWindow::onMenuButtonClicked()
{
    emit showApplicationMenuAtCursorPos();
}
