#ifndef CUSTOMWINDOW_H
#define CUSTOMWINDOW_H

// Qt:
#include <QDialog>

// Application:
#include "parameters.h"

class QPushButton;

class CustomWindow : public QDialog
{
    Q_OBJECT

public:
    // Constructor:
    CustomWindow(const QString &sTitle, QWidget *parent = 0);

    // Set tooltips:
    void setTooltips(const QMap<QString, QString> &mTooltipValues);

protected:
    // Event handlers:
    virtual void showEvent(QShowEvent *event);
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void paintEvent(QPaintEvent *event);
    virtual void closeEvent(QCloseEvent *e);

private:
    // Center on screen:
    void positionOnScreen();

    // Contains mouse?
    bool containsMouse(const QPoint &point);

    // Close button clicked:
    void onCloseButtonClicked();

private:
    QString m_sTitle;
    QPoint m_diff;
    QColor m_backgroundColor;
    QColor m_headerColor;
    int m_iHeaderHeight;
    bool m_bCloseButtonPressed;
    QMap<QString, QString> m_mTooltips;
    QPushButton *m_pCloseButton;
};

#endif
