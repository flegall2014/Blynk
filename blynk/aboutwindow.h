#ifndef ABOUTWINDOW_H
#define ABOUTWINDOW_H

// Application:
#include "customwindow.h"

namespace Ui {
class AboutWindow;
}

class AboutWindow : public CustomWindow
{
public:
    // Constructor:
    AboutWindow(const QString &sTitle, QWidget *parent=0);

protected:
    // Paint:
    virtual void paintEvent(QPaintEvent *event);

private:
    // UI:
    Ui::AboutWindow *ui;

public slots:
    // Done:
    void onDone();

    // Show vision aid overseas:
    void onShowVisionAidOverseas();

    // Show facebook:
    void onShowFaceBook();
};

#endif // ABOUTWINDOW_H
