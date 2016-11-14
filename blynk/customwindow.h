#ifndef CUSTOMWINDOW_H
#define CUSTOMWINDOW_H

// Qt:
#include <QDialog>

// Application:
#include "parameters.h"

namespace Ui {
class CustomWindow;
}
class CustomTooltip;
class CursorPage;
class ScreenBreakPage;
class BlueLightReducerPage;
class QPushButton;

class CustomWindow : public QDialog
{
    Q_OBJECT

public:
    // Constructor:
    CustomWindow(const QString &sTitle, QWidget *parent = 0);

    // Title:
    ~CustomWindow();

    // Set parameters:
    void setParameters(Parameters *pParameters);

    // Set tooltips:
    void setTooltips(const QMap<QString, QString> &mTooltipValues);

    // Update blynk cursor area:
    void updateBlynkCursorArea();

    // Update screen break area:
    void updateScreenBreakArea();

    // Update blue light reducer area:
    void updateBlueLightReducerArea();

    // Update UI:
    void updateUI();

protected:
    // Event handlers:
    virtual void showEvent(QShowEvent *event);
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void paintEvent(QPaintEvent *event);
    virtual void closeEvent(QCloseEvent *e);
    virtual bool event(QEvent *event);

private:
    // Center on screen:
    void positionOnScreen();

    // Contains mouse?
    bool containsMouse(const QPoint &point);

    // Close button clicked:
    void onCloseButtonClicked();

private:
    Ui::CustomWindow *ui;
    QString m_sTitle;
    QPoint m_diff;
    QColor m_backgroundColor;
    QColor m_headerColor;
    int m_iHeaderHeight;
    bool m_bCloseButtonPressed;
    CursorPage *m_pCursorPage;
    ScreenBreakPage *m_pScreenBreakPage;
    BlueLightReducerPage *m_pBlueLightReducerPage;
    Parameters *m_pParameters;
    CustomTooltip *m_pCustomTooltip;
    QMap<QString, QString> m_mTooltips;
    QPushButton *m_pCloseButton;

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
