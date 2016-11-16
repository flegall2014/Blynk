// Qt:
#include <QDesktopWidget>
#include <QPainter>
#include <QMouseEvent>
#include <QPushButton>

// Application:
#include "customwindow.h"
#include "utils.h"

// Constructor:
CustomWindow::CustomWindow(const QString &sTitle, QWidget *parent) :
    QDialog(parent),
    m_sTitle(sTitle),
    m_backgroundColor(QColor(229, 229, 229, 216)),
    m_headerColor(QColor(196, 196, 196)),
    m_iHeaderHeight(25),
    m_bCloseButtonPressed(false)
{
    // Set attributes:
    setWindowFlags(Qt::FramelessWindowHint | Qt::SubWindow | Qt::WindowStaysOnTopHint);
    setAttribute(Qt::WA_TranslucentBackground);

    // Close button:
    m_pCloseButton = new QPushButton("X", this);
    m_pCloseButton->setFlat(true);
    m_pCloseButton->resize(20, 20);
    connect(m_pCloseButton, &QPushButton::clicked, this, &CustomWindow::onCloseButtonClicked);
}

// Set tooltips:
void CustomWindow::setTooltips(const QMap<QString, QString> &mTooltipValues)
{
    m_mTooltips = mTooltipValues;

    QList<QPushButton *> lChildButtons = findChildren<QPushButton *>();
    foreach (QPushButton *pButton, lChildButtons) {
        QString sObjectName = pButton->objectName();
        if (sObjectName.isEmpty())
            continue;
        QString sTooltipValue = mTooltipValues[sObjectName];
        if (!sTooltipValue.isEmpty())
            pButton->setToolTip(Utils::splitTooltip(sTooltipValue, 256));
    }
}

// Handle close event:
void CustomWindow::closeEvent(QCloseEvent *e)
{
    hide();
    e->ignore();
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

// Close button clicked:
void CustomWindow::onCloseButtonClicked()
{
    hide();
}
