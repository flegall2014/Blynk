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
void CustomWindow::closeEvent(QCloseEvent *event)
{
    hide();
    event->ignore();
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
    int iScreenCenterX = (screenGeom.width()-width())/2;
    int iScreenCenterY = (screenGeom.height()-height())/2;
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
    painter.fillRect(background, QColor("#F9F9F9"));

    // Draw inner rect:
    QRect innerRect = background.adjusted(7, 32, -7, -7);
    painter.setBrush(QBrush(QColor("#E2E2E2")));
    painter.drawRoundedRect(innerRect, 32, 32);

    // Draw header:
    painter.setBrush(QBrush(m_headerColor));
    painter.drawRect(0, 0, rect().width(), m_iHeaderHeight);

    // Draw blynk icon:
    QImage img(":/icons/ico-eye.png");
    QImage scaledImage = img.scaled(28, 28, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    painter.drawImage(4, (m_iHeaderHeight-scaledImage.height())/2, scaledImage);
    QRect textRect(scaledImage.width()+8, 0, rect().width()-(scaledImage.width()+8), m_iHeaderHeight);
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
